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

#include <sprites/MissileActionRenderer.h>
#include <GLEXT/GLCameraFrustum.h>
#include <3dsparse/ModelsFile.h>
#include <actions/ShotProjectile.h>
#include <landscape/Landscape.h>
#include <common/OptionsDisplay.h>
#include <tankgraph/TankModelRenderer.h>
#include <tank/TankContainer.h>
#include <engine/ScorchedContext.h>

MissileActionRenderer::MissileActionRenderer(int flareType, float scale) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0), scale_(scale), rotation_(0.0f)
{

}

MissileActionRenderer::MissileActionRenderer(int flareType) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0), scale_(1.0f), rotation_(0.0f)
{

}

MissileActionRenderer::~MissileActionRenderer()
{
}

void MissileActionRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	ShotProjectile *shot = (ShotProjectile *) action;
	if (shot->getWeapon()->getCreateSmoke())
	{
		if (counter_.nextDraw(timepassed))
		{
			Vector &actualPos = shot->getCurrentPosition();
			Landscape::instance()->getSmoke().
				addSmoke(actualPos[0], actualPos[1], actualPos[2], 
					0.0f, 0.0f, 0.0f, 0.7f);
		}
	}
	rotation_ += shot->getCurrentVelocity().Magnitude();
}

void MissileActionRenderer::draw(Action *action)
{
	ShotProjectile *shot = (ShotProjectile *) action;
	Vector &actualPos = shot->getCurrentPosition();
	Vector &actualdir = shot->getCurrentVelocity();

	if (shot->getWeapon()->getShowShotPath())
	{
		Tank *current = 
			action->getScorchedContext()->tankContainer->
			getTankById(shot->getPlayerId());
		if (current)
		{
			GLState state(GLState::TEXTURE_OFF | GLState::BLEND_OFF);

			glColor3fv(current->getColor());
			glBegin(GL_LINES);
			std::list<Vector>::iterator itor;
			for (itor = shot->getPositions().begin();
				 itor != shot->getPositions().end();
				 itor++)
			{
				Vector &startPos = *itor;
				itor++;
				if (itor != shot->getPositions().end())
				{
					Vector &endPos = *itor;

					if (fabs(startPos[0] - endPos[0]) < 100.0f &&
						fabs(startPos[1] - endPos[1]) < 100.0f)
					{
						glVertex3fv(startPos);
						glVertex3fv(endPos);
					}
				}
				else break;
			}
			glEnd();
		}
	}

	// Check we can see the missile
	if (!GLCameraFrustum::instance()->pointInFrustum(actualPos))
	{
		return;
	}

	// Do we have a loaded mesh
	if (!mesh_)
	{
		Tank *currentPlayer = action->
			getScorchedContext()->tankContainer->getTankById(
			shot->getPlayerId());
		mesh_ = shot->getWeapon()->getWeaponMesh(currentPlayer);
	}

	mesh_->setScale(scale_);
	mesh_->draw(actualPos, actualdir, flareType_, rotation_);
}
