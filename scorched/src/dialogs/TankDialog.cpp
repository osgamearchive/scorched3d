////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <dialogs/TankDialog.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <weapons/Weapon.h>
#include <tankgraph/TankModelRenderer.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLBitmap.h>

TankDialog *TankDialog::instance_ = 0;

TankDialog *TankDialog::instance()
{
	if (!instance_)
	{
		instance_ = new TankDialog;
	}
	return instance_;
}

TankDialog::TankDialog() :
	GLWWindow("Player", 130, 30, 120, 120, eCircle | eSmallTitle)
{
}

TankDialog::~TankDialog()
{
}

void TankDialog::draw()
{
	static bool init = false;
	if (!init)
	{
		setX(MainCamera::instance()->getCamera().getWidth() - w_ - 10.0f);
		init = true;
	}

	disabled_ = true;
	Tank *current = 
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (current &&
		current->getState().getState() == TankState::sNormal)
	{
		disabled_ = false;
		GLWWindow::draw();

		drawTankModel(current);
		drawTankDetails(current);
	}
}

void TankDialog::drawTankModel(Tank *current)
{
	TankModelRenderer *model = (TankModelRenderer *) 
		current->getModel().getModelIdRenderer();
	if (!model) return;

	// Add the tooltip for the model name+attributes
	GLWToolTip::instance()->addToolTip(model->getTip(), 
		x_ + 10.0f, y_ + 10.0f, 100.0f, 100.0f);

	// Find the angles to rotate so the tank is at the
	// same angle as the "real" tank on the landscape
	Vector &lookFrom = MainCamera::instance()->getCamera().getCurrentPos();
	Vector &lookAt = MainCamera::instance()->getCamera().getLookAt();
	Vector dir = (lookAt - lookFrom).Normalize();
	float angXY = atan2f(dir[0], dir[1]) / 3.14f * 180.0f;
	float angYZ = acosf(dir[2]) / 3.14f * 180.0f + 180.0f;
	if (angYZ < 280.0f) angYZ = 280.0f;

	// Draw the tank
	glPushMatrix();
		// Set the tank angle
		glTranslatef(x_ + 64.0f, y_ + 64.0f, 0.0f);
		glRotatef(angYZ, 1.0f, 0.0f, 0.0f);
		glRotatef(angXY, 0.0f, 0.0f, 1.0f);

		// Draw the tank model
		glScalef(32.0f, 32.0f, 32.0f);
		GLState tankState(GLState::TEXTURE_OFF); // For no tank skins
		Vector position;
		model->getModel()->draw(
			false, 0.0f, position, 0.0f,
			current->getPhysics().getRotationGunXY(),
			current->getPhysics().getRotationGunYZ(),
			true);
	glPopMatrix();
}

void TankDialog::drawTankDetails(Tank *current)
{
	GLState newState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF | GLState::BLEND_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	Vector yellow(0.9f, 0.9f, 1.0f);

	// Name
	float namewidth = (float) GLWFont::instance()->getSmallPtFont()->getWidth(
		10.0f,
		current->getName());
	GLWFont::instance()->getSmallPtFont()->draw(
		current->getColor(),
		10.0f,
		x_ + 60.0f - (namewidth / 2.0f),
		y_ + 21.0f,
		0.0f,
		current->getName());

	// AutoDefense
	drawInfoBox(x_- 21.0f, y_ + 126.0f, 50.0f);
	drawJoin(x_ + 28.0f, y_ + 112.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 4.0f,
		y_ + 111.0f,
		0.0f,
		"%i",
		(current->getAccessories().getAutoDefense().haveDefense()?1:0));

	// Parachutes
	drawInfoBox(x_- 41.0f, y_ + 103.0f, 50.0f);
	drawJoin(x_ + 8.0f, y_ + 92.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 24.0f,
		y_ + 88.0f,
		0.0f,
		"%i",
		current->getAccessories().getParachutes().getNoParachutes());

	// Health
	drawInfoBox(x_- 49.0f, y_ + 80.0f, 50.0f);
	drawJoin(x_ + 1.0f, y_ + 69.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 32.0f,
		y_ + 65.0f,
		0.0f,
		"%.0f",
		current->getState().getLife());

	// Shield
	drawInfoBox(x_- 49.0f, y_ + 57.0f, 50.0f);
	drawJoin(x_ + 1.0f, y_ + 46.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 32.0f,
		y_ + 42.0f,
		0.0f,
		"%.0f",
		current->getAccessories().getShields().getShieldPower());

	// Batteries
	drawInfoBox(x_- 41.0f, y_ + 34.0f, 50.0f);
	drawJoin(x_ + 8.0f, y_ + 29.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 24.0f,
		y_ + 19.0f,
		0.0f,
		"%i",
		current->getAccessories().getBatteries().getNoBatteries());

	// Fuel
	drawInfoBox(x_- 21.0f, y_ + 11.0f, 50.0f);
	drawJoin(x_ + 28.0f, y_ + 9.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 4.0f,
		y_ - 4.0f,
		0.0f,
		"%i",
		current->getAccessories().getFuel().getNoFuel());

	GLState currentStateBlend(GLState::BLEND_ON | GLState::TEXTURE_ON);

	// Load textures
	if (!healthTexture_.textureValid())
	{
		GLBitmap healthMap(getDataFile("data/windows/health.bmp"), true);
		healthTexture_.create(healthMap, GL_RGBA, false);

		GLBitmap sheildMap(getDataFile("data/textures/wicons/shield.bmp"), true);
		shieldTexture_.create(sheildMap, GL_RGBA, false);

		GLBitmap batteryMap(getDataFile("data/textures/wicons/battery.bmp"), true);
		batteryTexture_.create(batteryMap, GL_RGBA, false);

		GLBitmap parachuteMap(getDataFile("data/textures/wicons/para.bmp"), true);
		parachuteTexture_.create(parachuteMap, GL_RGBA, false);

		GLBitmap autoaMap(getDataFile("data/textures/wicons/autod.bmp"), true);
		autodefenseTexture_.create(autoaMap, GL_RGBA, false);
		
		GLBitmap fuelMap(getDataFile("data/textures/wicons/fuel.bmp"), true);
		fuelTexture_.create(fuelMap, GL_RGBA, false);
	}

	// Fuel
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	autodefenseTexture_.draw();
	drawIconBox(x_ - 21.0f, y_ + 109.0f);

	// Batteries
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	parachuteTexture_.draw();
	drawIconBox(x_ - 41.0f, y_ + 86.0f);

	// Health
	glColor4f(0.9f, 0.0f, 0.0f, 0.8f);
	healthTexture_.draw();
	drawIconBox(x_ - 49.0f, y_ + 63.0f);

	// Shield
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	shieldTexture_.draw();
	drawIconBox(x_ - 49.0f, y_ + 40.0f);

	// Batteries
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	batteryTexture_.draw();
	drawIconBox(x_ - 41.0f, y_ + 17.0f);

	// Fuel
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	fuelTexture_.draw();
	drawIconBox(x_ - 21.0f, y_ - 6.0f);
}
