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

#include <landscape/Surround.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/Hemisphere.h>
#include <GLEXT/GLState.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/FileList.h>

Surround::Surround(SurroundDefs &defs) : xy_(0.0f), 
	cloudSpeed_(500.0f), cloudDirection_(0.0f),
	layer1_(0), layer2_(0)
{
}

Surround::~Surround()
{
}

void Surround::clear()
{
	delete layer1_;
	layer1_ = 0;
	delete layer2_;
	layer2_ = 0;
}

void Surround::simulate(float frameTime)
{
	if (OptionsDisplay::instance()->getNoSkyMovement()) return;

	float fastSpeed = 100;
	float slowSpeed = 500;
	float currentSpeed = ScorchedClient::instance()->getOptionsTransient().getWindSpeed();
	float wantedAngle = 180.0f-ScorchedClient::instance()->getOptionsTransient().getWindAngle();
	float wantedSpeed = (((5.0f - currentSpeed) / 5.0f) * (slowSpeed - fastSpeed)) + fastSpeed;

	// Move the cloud layer
	cloudSpeed_ = wantedSpeed;
	cloudDirection_ = wantedAngle;
	xy_ += frameTime / cloudSpeed_;
}

void Surround::draw()
{
	Landscape::instance()->getCloudTexture().draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	float slowXY = xy_ / 1.5f;
	glPushMatrix();
		// Rotate the scene so clouds blow the correct way
		glTranslatef(128.0f, 128.0f, -15.0f);
		glRotatef(cloudDirection_, 0.0f, 0.0f, 1.0f);

		GLState mainState2(GLState::TEXTURE_ON | GLState::BLEND_OFF);

		if (!layer1_)
		{
			Vector sunDir = 
				-Landscape::instance()->getSun().getPosition().Normalize();

			layer1_ = Hemisphere::createColored(1800, 180, 10, 10,
				Landscape::instance()->getSkyColorsMap(),
				sunDir,
				ScorchedClient::instance()->getLandscapeMaps().
				getLandDfn().getTex()->skytimeofday);
			layer1_->setNoVBO();
		}
		calculateWind(layer1_, 1800, 180, slowXY, slowXY + 0.4f);
		layer1_->draw();

		if (!OptionsDisplay::instance()->getNoSkyLayers())
		{
			GLState currentState(GLState::BLEND_ON);
			glColor4f(1.0f, 1.0f, 1.0f, 0.3f);

			if (!layer2_)
			{
				layer2_ = Hemisphere::createXY(2100, 120, 10, 20, 0, 0);
				layer2_->setNoVBO();
			}
			calculateWind(layer2_, 2100, 120, 0.0f, xy_ + 0.3f);
			layer2_->draw();
		}
	glPopMatrix();
}

void Surround::calculateWind(GLVertexArray *array, 
	float radius, float radius2, float xvalue, float yvalue)
{
	for (int i=0; i<array->getNoTris(); i++)
	{
		GLVertexArray::GLVertexArrayTexCoord &tex = 
			array->getTexCoordInfo(i);
		GLVertexArray::GLVertexArrayVertex &ver = 
			array->getVertexInfo(i);

		tex.a = (ver.x + radius) / (2 * radius) + xvalue;
		tex.b = (ver.y + radius) / (2 * radius) + yvalue;
	}
}
