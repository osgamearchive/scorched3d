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

#include <sprites/TracerActionRenderer.h>
#include <GLEXT/GLLenseFlare.h>
#include <actions/ShotProjectileTracer.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <GLEXT/GLState.h>

TracerActionRenderer::TracerActionRenderer()
{
}

TracerActionRenderer::~TracerActionRenderer()
{
}

void TracerActionRenderer::draw(Action *action)
{
	ShotProjectileTracer *shot = (ShotProjectileTracer *) action;
	if (shot->getSmokeTracer())
	{
		Tank *current = 
			action->getScorchedContext()->tankContainer.
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
			}
			glEnd();
		}
	}

	Vector &actualPos = shot->getCurrentPosition();
	GLLenseFlare::instance()->draw(
		actualPos, 
		!OptionsDisplay::instance()->getNoLenseFlare(),
		0);
}
