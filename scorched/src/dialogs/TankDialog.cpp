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
	GLWWindow("Player", 110, 10, 215, 75, eTransparent | eSmallTitle)
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

	{
		if (model)
		{
			GLWToolTip::instance()->addToolTip(model->getTip(), 
				x_ + 7.0f, y_ + 3.0f, 55.0f, 55.0f);
		}

		// Draw the round window containing the tank model
		GLState newState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF | GLState::BLEND_ON);
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x_ + 27.0f, y_ + 23.0f);
			glVertex2f(x_ + 27.0f, y_ + 3.0f);
			drawRoundBox(x_ + 7.0f, y_ + 3.0f, 55.0f, 55.0f, 10.0f);
			glVertex2f(x_ + 27.0f, y_ + 3.0f);
		glEnd();

		glColor3f(0.0f, 0.0f, 0.0f);
		GLState neState(GLState::BLEND_OFF);
		glBegin(GL_LINE_LOOP);
			drawRoundBox(x_ + 7.0f, y_ + 3.0f, 55.0f, 55.0f, 10.0f);
		glEnd();

		// Draw the life bar
		float lifePart = 14.0f + 40.0f * current->getState().getLife() / 100.0f;		
		glBegin(GL_QUADS);
			if (current->getState().getLife() > 0.0f)
			{
				glColor3f(0.0f, 0.8f, 0.0f);
				glVertex2f(x_ + 14.0f, y_ + 8.0f);
				glVertex2f(x_ + lifePart, y_ + 8.0f);
				glVertex2f(x_ + lifePart, y_ + 11.0f);
				glVertex2f(x_ + 14.0f, y_ + 11.0f);
			}
	
			if (current->getState().getLife() < 100.0f)
			{
				glColor3f(0.8f, 0.0f, 0.0f);
				glVertex2f(x_ + lifePart, y_ + 8.0f);
				glVertex2f(x_ + 54, y_ + 8.0f);
				glVertex2f(x_ + 54, y_ + 11.0f);
				glVertex2f(x_ + lifePart, y_ + 11.0f);
			}
		glEnd();
	}

	if (!model) return;

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
		glTranslatef(x_ + 34.0f, y_ + 23.0f, 0.0f);
		glRotatef(angYZ, 1.0f, 0.0f, 0.0f);
		glRotatef(angXY, 0.0f, 0.0f, 1.0f);

		// Draw the tank model
		glScalef(20.0f, 20.0f, 20.0f);
		GLState tankState(GLState::TEXTURE_OFF); // For no tank skins
		Vector position;
		model->getModel()->draw(
			false, 0.0f, position, 0.0f,
			current->getPhysics().getRotationGunXY(),
			current->getPhysics().getRotationGunYZ());
	glPopMatrix();
}

void TankDialog::drawTankDetails(Tank *current)
{
	GLState newState(GLState::DEPTH_OFF);

	// Draw the name of the tank
	GLWFont::instance()->getFont()->drawLen(
		22,
		current->getColor(),
		14.0f,
		x_ + 5.0f,
		y_ + h_ - 17.0f,
		0.0f,
		current->getName());

	// Draw the elevation rotation etc...
	Vector yellow(0.7f, 0.7f, 0.0f);
	float rotDiff = (360.0f - current->getPhysics().getRotationGunXY()) - 
		(360.0f - current->getPhysics().getOldRotationGunXY());
	if (rotDiff > 180.0f) rotDiff -= 360.0f;
	else if (rotDiff < -180.0f) rotDiff += 360.0f;
	GLWFont::instance()->getFont()->draw(
		yellow,
		12.0f,
		x_ + 67.0f,
		y_ + h_ - 32.0f,
		0.0f,
		"Rot:%s",
		current->getPhysics().getRotationString());
	GLWFont::instance()->getFont()->draw(
		yellow,
		12.0f,
		x_ + 67.0f,
		y_ + h_ - 42.0f,
		0.0f,
		"Ele:%s",
		current->getPhysics().getElevationString());
	GLWFont::instance()->getFont()->draw(
		yellow,
		12.0f,
		x_ + 67.0f,
		y_ + h_ - 52.0f,
		0.0f,
		"Pwr:%s",
		current->getState().getPowerString());

	GLWToolTip::instance()->addToolTip(
		&current->getAccessories().getWeapons().getCurrent()->getToolTip(),
		x_ + 67.0f, y_ + h_ - 62.0f, 150.0f, 10.0f);
	int count = current->getAccessories().getWeapons().getWeaponCount(
		current->getAccessories().getWeapons().getCurrent());
	GLWFont::instance()->getFont()->draw(
		yellow,
		12.0f,
		x_ + 67.0f,
		y_ + h_ - 62.0f,
		0.0f,
		current->getAccessories().getWeapons().getWeaponString());

	GLWFont::instance()->getFont()->draw(
		yellow,
		12.0f,
		x_ + 67.0f,
		y_ + h_ - 72.0f,
		0.0f,
		current->getScore().getScoreString());
}
