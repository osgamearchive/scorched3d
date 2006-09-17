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

#include <engine/ScorchedPhysicsEngine.h>
#include <engine/ScorchedCollisionHandler.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/SkyRoofCollision.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>

ScorchedPhysicsEngine::ScorchedPhysicsEngine() :
	hmcol_(0), srcol_(0), sccol_(0), context_(0)
{
}

ScorchedPhysicsEngine::~ScorchedPhysicsEngine()
{
}

void ScorchedPhysicsEngine::setScorchedContext(ScorchedContext *context)
{
	context_ = context;

	hmcol_ = new HeightMapCollision(context);
	srcol_ = new SkyRoofCollision(context);
	sccol_ = new ScorchedCollisionHandler(context);
	setCollisionHandler(sccol_);

	hmcol_->setContext(context_);
	srcol_->setContext(context_);
}

void ScorchedPhysicsEngine::resetContext()
{
	HeightMapCollision::setContext(context_);
 	SkyRoofCollision::setContext(context_);
}

void ScorchedPhysicsEngine::generate()
{
	// Set the wind for the next shot
	Vector wind;
	if (context_->optionsTransient->getWindOn())
	{
		wind = context_->optionsTransient->getWindDirection();
		wind *= context_->optionsTransient->getWindSpeed() / 2.0f;
	}

	float gravity = (float) context_->optionsGame->getGravity();
	Vector gravityVec(0.0f, 0.0f, gravity);
	setGravity(gravityVec);
	setWind(wind);

	// Set the walls
	hmcol_->generate();
}
