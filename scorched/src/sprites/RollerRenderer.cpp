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

#include <actions/Roller.h>
#include <sprites/RollerRenderer.h>
#include <client/ScorchedClient.h>
#include <actions/ShotProjectile.h>
#include <GLEXT/GLState.h>

RollerRenderer::RollerRenderer()
{

}

RollerRenderer::~RollerRenderer()
{
}

void RollerRenderer::draw(Action *action)
{
	Roller *roller = (Roller *) action;

	float z = ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(
		roller->getX(), roller->getY());

	GLState state(GLState::TEXTURE_OFF);

	glPointSize(4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex3f((float) roller->getX(), (float) roller->getY(), z);
	glEnd();
	glPointSize(1.0f);

	if (action->getScorchedContext()->tankContainer.getCurrentPlayerId() == roller->getPlayerId())
	{
		Vector position(
			(float) roller->getX(), 
			(float) roller->getY(),
			z);
		ShotProjectile::getLookAtPosition() += position;
		ShotProjectile::getLookAtCount()++;
	}
}
