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


#include <landscape/Sun.h>
#include <common/OptionsGame.h>
#include <common/OptionsDisplay.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLLenseFlare.h>
#include <GLEXT/GLCameraFrustum.h>
#include <stdlib.h>
#include <math.h>

Sun::Sun() : drawSun_(false)
{
	float sunRotXY = float(OptionsDisplay::instance()->getSunXYAng()) 
		/ 180.0f * 3.14f;
	float sunRotYZ = float(OptionsDisplay::instance()->getSunYZAng())
		/ 180.0f * 3.14f;
	position_ = Vector(
		sinf(sunRotXY) * 1800 * cosf(sunRotYZ), 
		cosf(sunRotXY) * 1800 * cosf(sunRotYZ), 
		sinf(sunRotYZ) * 150);
}

Sun::~Sun()
{
}

void Sun::draw()
{
	// Dont draw until the sky shading is implemented
	// Just return for now
	if (!drawSun_) return;

	Vector pos;
	pos[0] = position_[0] / 1.5f + 128.0f;
	pos[1] = position_[1] / 1.5f + 128.0f;
	pos[2] = position_[2] / 1.5f;
	GLLenseFlare::instance()->draw(pos, false, 2);

	glPushMatrix();
		glTranslatef(128.0f, 128.0f, -15.0f);

		GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(position_);
		glEnd();
	glPopMatrix();
}
