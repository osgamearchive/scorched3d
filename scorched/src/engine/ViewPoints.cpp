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

#include <engine/ViewPoints.h>
#include <engine/ScorchedContext.h>

ViewPoints::ViewPoints() : context_(0)
{
}

ViewPoints::~ViewPoints()
{
}

Vector &ViewPoints::getLookAt()
{
	static Vector position;
	position.zero();
	float count = 0.0f;

	std::list<ViewPoint *>::iterator itor =
		points_.begin();
	std::list<ViewPoint *>::iterator enditor =
		points_.end();
	for (; itor != enditor; itor++)
	{
		position += (*itor)->getPosition();
		count += 1.0f;
	}

	position /= count;
	return position;
}

int ViewPoints::getLookAtCount()
{
	return (int) points_.size();
}

ViewPoints::ViewPoint *ViewPoints::getNewViewPoint(unsigned int playerId)
{
	if (context_->serverMode) return 0;

	if (context_->tankContainer.getCurrentPlayerId() != playerId &&
		context_->optionsGame.getTurnType() == OptionsGame::TurnSimultaneous)
	{
		return 0;
	}

	ViewPoint *viewpoint = new ViewPoint();
	points_.push_back(viewpoint);
	return viewpoint;
}

void ViewPoints::releaseViewPoint(ViewPoint *point)
{
	points_.remove(point);
}
