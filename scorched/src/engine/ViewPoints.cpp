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
#include <common/Defines.h>
#include <common/OptionsScorched.h>
#include <tank/TankContainer.h>

ViewPoints::ViewPoints() : context_(0), totalTime_(0), finished_(false)
{
}

ViewPoints::~ViewPoints()
{
}

void ViewPoints::getValues(Vector &lookAt, 
						   Vector &lookFrom)
{
	lookAt = lookAt_;
	lookFrom = lookFrom_;
}

void ViewPoints::setValues(Vector &lookAt, 
						   Vector &lookFrom)
{
	lookAt_ = lookAt;
	lookFrom_ = lookFrom;
}

void ViewPoints::simulate(float frameTime)
{
	if (getLookAtCount() == 0) return;

	Vector max, min;
	bool firstItor = true;
	float count = 0.0f;

	static Vector lookAt;
	static Vector lookFrom;
	lookAt.zero();
	lookFrom.zero();

	std::list<ViewPoint *>::iterator itor =
		points_.begin();
	std::list<ViewPoint *>::iterator enditor =
		points_.end();
	for (; itor != enditor; itor++)
	{
		Vector &itorPosition = (*itor)->getPosition();
		Vector &itorLookAt = (*itor)->getLookFrom();
		float itorRadius = (*itor)->getRadius();

		if (firstItor)
		{
			firstItor = false;
			min = itorPosition;
			max = itorPosition;
		}

		min[0] = MIN(min[0], itorPosition[0] - itorRadius);
		min[1] = MIN(min[1], itorPosition[1] - itorRadius);
		min[2] = MIN(min[2], itorPosition[2] - itorRadius);
		max[0] = MAX(max[0], itorPosition[0] + itorRadius);
		max[1] = MAX(max[1], itorPosition[1] + itorRadius);
		max[2] = MAX(max[2], itorPosition[2] + itorRadius);		

		lookAt += itorPosition;
		lookFrom += itorLookAt;
		count += 1.0f;
	}

	float dist = 25.0f;
	float maxMin = (max - min).Magnitude();
	if (maxMin > 0.0f)
	{
		dist = 25.0f + maxMin;
	}
	lookFrom.StoreNormalize();

	lookAt /= count;
	lookFrom *= dist;

	lookAt_ = lookAt;
	lookFrom_ = lookFrom;
}

int ViewPoints::getLookAtCount()
{
	if (finished_) return 0;
	return (int) points_.size();
}

ViewPoints::ViewPoint *ViewPoints::getNewViewPoint(unsigned int playerId)
{
	if (context_->serverMode) return 0;
	if (playerId == 0) return 0;

	if (context_->tankContainer->getCurrentPlayerId() != playerId &&
		context_->optionsGame->getTurnType() == OptionsGame::TurnSimultaneous)
	{
		return 0;
	}

	ViewPoint *viewpoint = new ViewPoint();
	points_.push_back(viewpoint);
	return viewpoint;
}

void ViewPoints::explosion(unsigned int playerId)
{
	if (context_->serverMode) return;

	if (context_->tankContainer->getCurrentPlayerId() != playerId &&
		context_->optionsGame->getTurnType() == OptionsGame::TurnSimultaneous)
	{
		return;
	}

	finished_ = true;
}

void ViewPoints::releaseViewPoint(ViewPoint *point)
{
	points_.remove(point);
}
