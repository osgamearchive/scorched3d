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

ViewPoints::ViewPoints() : context_(0), totalTime_(0)
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

	float dist = 15.0f;
	float maxMin = (max - min).Magnitude();
	if (maxMin > 0.0f)
	{
		dist = 15.0f + maxMin;
	}
	lookFrom.StoreNormalize();

	lookAt /= count;
	lookFrom *= dist;

	// Make some constant changes, regardless of framerate
	if (count == 1)
	{
		lookAt_ = lookAt;
		lookFrom_ = lookFrom;
	}
	else
	{
		const float SecondsToReachTarget = 0.05f;
		totalTime_ += frameTime;
		while (totalTime_ > 0.05f)
		{
			totalTime_ -= 0.05f;

			// Calculate the new look at value
			Vector directionLookAt = lookAt - lookAt_;
			directionLookAt	*= SecondsToReachTarget;
			lookAt_ += directionLookAt;

			// Calculate the new look from value
			Vector directionPosition = lookFrom - lookFrom_;
			directionPosition *= SecondsToReachTarget;
			lookFrom_ += directionPosition;
		}
	}
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
