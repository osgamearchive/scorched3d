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
#include <tank/TankContainer.h>
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
			TankContainer::instance()->getTankById(shot->getPlayerId());
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
				glVertex3fv(*itor);
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
