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

Sun::Sun() : drawSun_(true)
{
}

Sun::~Sun()
{
}

void Sun::setPosition(float sunRotXY, float sunRotYZ)
{
	sunRotXY = sunRotXY / 180.0f * 3.14f;
	sunRotYZ = sunRotYZ / 180.0f * 3.14f;
	position_ = Vector(
		(sinf(sunRotXY) * 300.0f * cosf(sunRotYZ)) + 128.0f, 
		(cosf(sunRotXY) * 300.0f * cosf(sunRotYZ)) + 128.0f, 
		(sinf(sunRotYZ) * 300.0f));
}

void Sun::draw()
{
	// Dont draw until the sky shading is implemented
	// Just return for now
	if (!drawSun_) return;

	if (!texture_.textureValid())
	{
		std::string file = getDataFile("data/textures/glow1.bmp");
		GLBitmap map(file.c_str(), file.c_str(), false);
		texture_.create(map, GL_RGBA, true);
	}

	GLState currentStateOne(GLState::TEXTURE_ON | GLState::DEPTH_OFF | GLState::BLEND_ON);
	texture_.draw();
	Vector color(1.0f, 1.0f, 1.0f);
	GLCameraFrustum::instance()->drawBilboard(
		position_, color, 
		1.0f, // alpha
		10.0f, 10.0f, // width, height
		true, // additive texture
		0); // tex coord

	GLState currentState(GLState::TEXTURE_OFF);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3f(128.0f, 128.0f, 0.0f);
		glVertex3fv(position_);
	glEnd();
	glPointSize(3.0f);
	glBegin(GL_POINTS);
		glVertex3fv(position_);
	glEnd();
	glPointSize(1.0f);
}

