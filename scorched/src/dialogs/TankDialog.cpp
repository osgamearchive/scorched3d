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
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <client/ClientState.h>
#include <weapons/Weapon.h>
#include <tankgraph/TankModelRenderer.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWSelector.h>
#include <GLEXT/GLViewPort.h>
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
	GLWWindow("Player", 130, 30, 120, 120, eCircle | eSmallTitle,
		"Displays indicators for the current player,\n"
		"including its health, and defense status.")
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
		setX(GLViewPort::getWidth() - w_ - 10.0f);
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
	GLWToolTip::instance()->addToolTip(&model->getTips()->tankTip, 
		x_ + 20.0f, y_ + 20.0f, 80.0f, 80.0f);

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
	TankModelRenderer *model = (TankModelRenderer *) 
		current->getModel().getModelIdRenderer();
	if (!model) return;

	GLWTankTips *tankTips = model->getTips();

	GLState newState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF | GLState::BLEND_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	Vector yellow(0.9f, 0.9f, 1.0f);

	// Name
	float namewidth = (float) GLWFont::instance()->getSmallPtFont()->getWidth(
		10.0f,
		current->getName());
	GLWToolTip::instance()->addToolTip(&model->getTips()->nameTip, 
		x_ + 60.0f - (namewidth / 2.0f), y_ + 21.0f, namewidth, 20.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		current->getColor(),
		10.0f,
		x_ + 60.0f - (namewidth / 2.0f),
		y_ + 21.0f,
		0.0f,
		current->getName());

	// AutoDefense
	GLWToolTip::instance()->addToolTip(&model->getTips()->autodTip, 
		x_- 21.0f, y_ + 108.0f, 50.0f, 18.0f);
	drawInfoBox(x_- 21.0f, y_ + 126.0f, 50.0f);
	drawJoin(x_ + 28.0f, y_ + 112.0f);
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12.0f,
		x_ - 4.0f,
		y_ + 111.0f,
		0.0f,
		(current->getAccessories().getAutoDefense().haveDefense()?"On":"Off"));

	// Parachutes
	GLWToolTip::instance()->addToolTip(&model->getTips()->paraTip, 
		x_- 41.0f, y_ + 85.0f, 50.0f, 18.0f);
	drawInfoBox(x_- 41.0f, y_ + 103.0f, 50.0f);
	drawJoin(x_ + 8.0f, y_ + 92.0f);
	if (!current->getAccessories().getParachutes().parachutesEnabled())
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ - 24.0f,
			y_ + 88.0f,
			0.0f,
			"Off");
	}
	else
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ - 24.0f,
			y_ + 88.0f,
			0.0f,
			"%i",
			current->getAccessories().getParachutes().getNoParachutes());
	}

	// Health
	GLWToolTip::instance()->addToolTip(&model->getTips()->healthTip, 
		x_- 49.0f, y_ + 62.0f, 50.0f, 18.0f);
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
	GLWToolTip::instance()->addToolTip(&model->getTips()->shieldTip, 
		x_- 49.0f, y_ + 39.0f, 50.0f, 18.0f);
	drawInfoBox(x_- 49.0f, y_ + 57.0f, 50.0f);
	drawJoin(x_ + 1.0f, y_ + 46.0f);
	if (!current->getAccessories().getShields().getCurrentShield())
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ - 32.0f,
			y_ + 42.0f,
			0.0f,
			"Off");
	}
	else
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ - 32.0f,
			y_ + 42.0f,
			0.0f,
			"%.0f",
			current->getAccessories().getShields().getShieldPower());
	}

	// Batteries
	GLWToolTip::instance()->addToolTip(&model->getTips()->batteryTip, 
		x_- 41.0f, y_ + 16.0f, 50.0f, 18.0f);
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
	GLWToolTip::instance()->addToolTip(&model->getTips()->fuelTip, 
		x_- 21.0f, y_ - 7.0f, 50.0f, 18.0f);
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

	// Auto Defense
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

void TankDialog::mouseDown(float x, float y, bool &skipRest)
{
	Tank *current = 
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	GLWTankTips *tankTips = 0;
	if (current)
	{
		TankModelRenderer *model = (TankModelRenderer *) 
			current->getModel().getModelIdRenderer();
		tankTips = model->getTips();
	}

	if (current && tankTips &&
		current->getState().getState() == TankState::sNormal)
	{
		// Autodefense
		if (inBox(x, y, x_- 21.0f, y_ + 108.0f, 50.0f, 18.0f))
		{
			static GLWTip useTip("Auto Defense On", 
				"Enable the auto defense.");
			static GLWTip offTip("Auto Defense Off", 
				"Disable the auto defense.");

			skipRest = true;
			std::list<GLWSelectorEntry> entries;
			if (current->getAccessories().getAutoDefense().haveDefense()) 
				entries.push_back(GLWSelectorEntry("On", &useTip));
			else entries.push_back(GLWSelectorEntry("Off", &offTip));
			GLWSelector::instance()->showSelector(0, x, y, entries,
				ClientState::StatePlaying);
		}
		// Parachutes
		else if (inBox(x, y, x_- 41.0f, y_ + 85.0f, 50.0f, 18.0f))
		{
			static GLWTip useTip("Parachutes On", 
				"Enable parachutes.");
			static GLWTip offTip("Parachutes Off", 
				"Disable parachutes.");

			skipRest = true;
			std::list<GLWSelectorEntry> entries;
			if (current->getAccessories().getParachutes().getNoParachutes() > 0)
			{
				char buffer[128];
				sprintf(buffer, "On (%i)", 
					current->getAccessories().getParachutes().getNoParachutes());
				entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) 1));
			}
			entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, (void *) 0));
			GLWSelector::instance()->showSelector(&tankTips->paraTip, x, y, entries,
				ClientState::StatePlaying);
		}
		// Health
		else if (inBox(x, y, x_- 49.0f, y_ + 62.0f, 50.0f, 18.0f))
		{
			skipRest = true;
		}
		// Shield
		else if (inBox(x, y, x_- 49.0f, y_ + 39.0f, 50.0f, 18.0f))
		{
			static GLWTip offTip("Sheild Off", 
				"Don't select a shield or\n"
				"turn off any current shield");

			skipRest = true;
			std::list<GLWSelectorEntry> entries;
			std::map<Shield*, int> &shields = 
				current->getAccessories().getShields().getAllShields();
			std::map<Shield*, int>::iterator itor;
			for (itor = shields.begin();
				itor != shields.end();
				itor++)
			{
				char buffer[128];
				sprintf(buffer, "%s (%i)", 
					(*itor).first->getName(),
					(*itor).second);
				entries.push_back(GLWSelectorEntry(buffer, &(*itor).first->getToolTip(), 
					0, 0, (*itor).first));
			}
			entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, 0));
			GLWSelector::instance()->showSelector(&tankTips->shieldTip, x, y, entries,
				ClientState::StatePlaying);
		}
		// Batteries
		else if (inBox(x, y, x_- 41.0f, y_ + 16.0f, 50.0f, 18.0f))
		{
			static GLWTip useTip("Battery", 
				"Use some batteries");
			static GLWTip offTip("Battery Cancel", 
				"Don't use any batteries");

			skipRest = true;
			std::list<GLWSelectorEntry> entries;
			for (int i=1; i<=current->getAccessories().getBatteries().getNoBatteries(); i++)
			{
				char buffer[128];
				sprintf(buffer, "Use %i", i);
				entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) i));
			}
			entries.push_back(GLWSelectorEntry("Cancel", &offTip, 0, 0, (void *) 0));
			GLWSelector::instance()->showSelector(&tankTips->batteryTip, x, y, entries,
				ClientState::StatePlaying);		
		}
		// Fuel
		else if (inBox(x, y, x_- 21.0f, y_ - 7.0f, 50.0f, 18.0f))
		{
			static GLWTip useTip("Fuel", 
				"Swicth movement mode on.\n"
				"In this mode left clicking on the light areas\n"
				"of the landscape will move the tank there.");
			static GLWTip offTip("Fuel Cancel", 
				"Switch back to the normal camera mode.");

			skipRest = true;
			std::list<GLWSelectorEntry> entries;
			if (current->getAccessories().getFuel().getNoFuel() > 0)
			{
				entries.push_back(GLWSelectorEntry("Move", &useTip, 0, 0, (void *) 1));
			}
			entries.push_back(GLWSelectorEntry("Cancel", &offTip, 0, 0, (void *) 0));
			GLWSelector::instance()->showSelector(&tankTips->fuelTip, x, y, entries,
				ClientState::StatePlaying);		
		}
		else GLWWindow::mouseDown(x, y, skipRest);
	}
	else GLWWindow::mouseDown(x, y, skipRest);
}
