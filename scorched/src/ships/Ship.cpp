////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <ships/Ship.h>
#include <3dsparse/ModelStore.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <landscape/LandscapeTex.h>
#include <landscape/Landscape.h>
#include <landscape/Water.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <math.h>

Ship::Ship(LandscapeTexShip *texShip)
{
	Model *model = ModelStore::instance()->loadModel(texShip->model);
	scale_ = texShip->scale;
	ship_ = new ModelRenderer(model);
	size_ = MAX(model->getMax()[0], model->getMax()[1]) * scale_;
	length_ = model->getMax()[1];
}

Ship::~Ship()
{
	delete ship_;
}

void Ship::draw(Vector &position, Vector &direction, Vector &directionPerp)
{
	Vector shipPosition = position;

	shipPosition += directionPerp * -offset_[0];
	shipPosition += direction * offset_[1];
	float angle = atan2f(direction[1], direction[0]);
	float angleDegs = (angle / 3.14f) * 180.0f - 90.0f;

	if (GLCameraFrustum::instance()->sphereInFrustum(shipPosition, size_))
	{
		glPushMatrix();
			glTranslatef(shipPosition[0], shipPosition[1], shipPosition[2] - 1.0f);
			glRotatef(angleDegs, 0.0f, 0.0f, 1.0f);

			{
				glDepthMask(GL_FALSE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glDisable(GL_FOG);
				Landscape::instance()->getWater().getWaves().
					getWavesTexture1().draw();
				glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
				glPushMatrix();
					glTranslatef(0.0f, -(length_ * scale_) - 10.0f, 0.0f);
					float swellSize = length_ * scale_ / 2.0f;
					glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 0.0f);
						glVertex3f(-swellSize, -swellSize, 1.5f);
						glTexCoord2f(1.0f, 0.0f);
						glVertex3f(swellSize, -swellSize, 1.5f);
						glTexCoord2f(1.0f, 1.0f);
						glVertex3f(swellSize / 3.0f, swellSize, 1.5f);
						glTexCoord2f(0.0f, 1.0f);
						glVertex3f(-swellSize / 3.0f, swellSize, 1.5f);
					glEnd();
				glPopMatrix();
				if (!OptionsDisplay::instance()->getNoFog()) glEnable(GL_FOG); // NOTE: Fog on
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthMask(GL_TRUE);
			}

			glScalef(scale_, scale_, scale_);
			ship_->drawBottomAligned();
		glPopMatrix();
	}
}

void Ship::simulate(float frameTime)
{
	ship_->simulate(frameTime);
}
