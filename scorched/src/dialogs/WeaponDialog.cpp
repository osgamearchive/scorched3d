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

#include <dialogs/WeaponDialog.h>
#include <client/ScorchedClient.h>
#include <weapons/Weapon.h>
#include <tankgraph/MissileMesh.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <GLW/GLWFont.h>

WeaponDialog *WeaponDialog::instance_ = 0;

WeaponDialog *WeaponDialog::instance()
{
	if (!instance_)
	{
		instance_ = new WeaponDialog();
	}
	return instance_;
}

WeaponDialog::WeaponDialog() : 
	GLWWindow("Weapons", 10, 30, 120, 120, 
		eCircle | eSmallTitle),
	totalTime_(0.0f)
{

}

WeaponDialog::~WeaponDialog()
{

}

void WeaponDialog::simulate(float frameTime)
{
	GLWWindow::simulate(frameTime);
	totalTime_+=frameTime;
}

void WeaponDialog::draw()
{
	GLWWindow::draw();

	disabled_ = true;
	Tank *current = 
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (current &&
		current->getState().getState() == TankState::sNormal)
	{
		disabled_ = false;
		GLWWindow::draw();

		drawWeapon(current);
	}
}

void WeaponDialog::drawWeapon(Tank *current)
{
	Weapon *weapon = current->getAccessories().getWeapons().getCurrent();
	if (!weapon) return;

	// Cache some stuff we should only fetch when tank
	// or tank's weapon changes
	static Weapon *storedWeapon = 0;
	static Tank *storedTank = 0;
	static MissileMesh *storedMesh = 0;
	static GLTexture *storedTexture = 0;
	if (weapon != storedWeapon || current != storedTank)
	{
		storedWeapon = weapon;
		storedTank = current;
		storedMesh = weapon->getWeaponMesh(current);
		storedTexture = weapon->getTexture();
	}

	// Draw the current weapon
	glPushMatrix();
		glTranslatef(x_ + 64.0f, y_ + 64.0f, 0.0f);
		glRotatef(totalTime_ * 45.0f, 0.0f, 0.0f, 1.0f);
		Vector position;
		Vector direction(0.3f, 1.0f, 1.0f);
		storedMesh->setScale(40.0f);
		storedMesh->draw(position, direction, 0, totalTime_ * 45.0f);
	glPopMatrix();

	if (!xyTexture_.textureValid())
	{
		GLBitmap xyMap(PKGDIR "data/windows/xy.bmp", true);
		xyTexture_.create(xyMap, GL_RGBA, false);

		GLBitmap yzMap(PKGDIR "data/windows/yz.bmp", true);
		yzTexture_.create(yzMap, GL_RGBA, false);

		GLBitmap powerMap(PKGDIR "data/windows/power.bmp", true);
		powerTexture_.create(powerMap, GL_RGBA, false);
	}

	
	{
		GLState newState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		Vector yellow(0.9f, 0.9f, 1.0f); // Not Yellow!! ;)

		// Draw the weapon name
		float weaponWidth = (float) GLWFont::instance()->getSmallPtFont()->
			getWidth(10.0f, 
			current->getAccessories().getWeapons().getCurrent()->getName());
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10.0f,
			x_ + 60.0f - (weaponWidth / 2.0f),
			y_ + 21.0f,
			0.0f,
			current->getAccessories().getWeapons().getCurrent()->getName());

		// Weapon info
		int count = current->getAccessories().getWeapons().getWeaponCount(
			current->getAccessories().getWeapons().getCurrent());
		const char *format = "%i";
		if (count < 0) format = "Inf";
		drawInfoBox(x_ + 113, y_ + 103.0f, 47.0f);
		drawJoin(x_ + 113.0f, y_ + 90.0f);
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ + 133.0f,
			y_ + 88.0f,
			0.0f,
			format,
			count);

		// Rotation XY
		drawInfoBox(x_ + 111.0f, y_ + 34.0f, 72.0f);
		drawJoin(x_ + 111.0f, y_ + 29.0f);
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ + 131.0f,
			y_ + 19.0f,
			0.0f,
			"%.1f",
			current->getPhysics().getRotationGunXY());

		// Rotation YZ
		drawInfoBox(x_ + 118.0f, y_ + 57.0f, 61.0f);
		drawJoin(x_ + 118.0f, y_ + 49.0f);
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ + 137.0f,
			y_ + 42.0f,
			0.0f,
			"%.1f",
			current->getPhysics().getRotationGunYZ());

		// Power
		drawInfoBox(x_ + 118.0f, y_ + 80.0f, 80.0f);
		drawJoin(x_ + 118.0f, y_ + 72.0f);
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12.0f,
			x_ + 137.0f,
			y_ + 65.0f,
			0.0f,
			"%.1f",
			current->getState().getPower());

		GLState currentStateBlend(GLState::BLEND_ON | GLState::TEXTURE_ON);

		// Draw the weapon icon
		if (storedTexture)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			storedTexture->draw();
			drawIconBox(x_ + 116.0f, y_ + 84.0f);
		}

		// XY Icon
		glColor4f(0.9f, 0.9f, 0.3f, 0.8f);
		xyTexture_.draw();
		drawIconBox(x_ + 114.0f, y_ + 16.0f);

		// YZ Icon
		glColor4f(0.9f, 0.9f, 0.3f, 0.8f);
		yzTexture_.draw();
		drawIconBox(x_ + 121.0f, y_ + 40.0f);

		// Power
		glColor4f(0.9f, 0.0f, 0.0f, 0.8f);
		powerTexture_.draw();
		drawIconBox(x_ + 122.0f, y_ + 63.0f);
	}
}
