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
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <common/OptionsGame.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLLenseFlare.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <stdlib.h>
#include <math.h>

Sun::Sun()
{
}

Sun::~Sun()
{
}

void Sun::setPosition(float sunRotXY, float sunRotYZ)
{
	LandscapeDefn &defn = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getDefn();

	sunRotXY = sunRotXY / 180.0f * 3.14f;
	sunRotYZ = sunRotYZ / 180.0f * 3.14f;
	position_ = Vector(
		(sinf(sunRotXY) * 900.0f * cosf(sunRotYZ)) + defn.landscapewidth / 2.0f, 
		(cosf(sunRotXY) * 900.0f * cosf(sunRotYZ)) + defn.landscapeheight / 2.0f, 
		(sinf(sunRotYZ) * 900.0f));
}

void Sun::generate()
{
	LandscapeTex &tex = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();

	std::string file = getDataFile(tex.suntexture.c_str());
	GLBitmap map(file.c_str(), file.c_str(), false);
	DIALOG_ASSERT(texture_.replace(map, GL_RGBA, true));
}

void Sun::draw()
{
	GLState currentStateOne(GLState::TEXTURE_ON | GLState::DEPTH_OFF | GLState::BLEND_ON);
	texture_.draw();

	float aspect = float(GLViewPort::getHeight()) / float(GLViewPort::getWidth());
	LandscapeTex &tex = *ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();
	GLCameraFrustum::instance()->drawBilboard(
		position_, tex.suncolor, 
		1.0f, // alpha
		60.0f, 60.0f * aspect, // width, height
		true, // additive texture
		0); // tex coord
}

