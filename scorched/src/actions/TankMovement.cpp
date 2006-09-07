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

#include <actions/TankMovement.h>
#include <actions/TankMovementEnd.h>
#include <actions/TankFalling.h>
#include <actions/ShotProjectile.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/Fuel.h>
#include <weapons/AccessoryStore.h>
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeTex.h>
#include <landscape/MovementMap.h>
#include <landscape/Landscape.h>
#include <tank/TankContainer.h>
#include <tankgraph/TankModelStore.h>
#include <3dsparse/ImageStore.h>
#include <GLEXT/GLBitmapModifier.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <sound/Sound.h>

static const int NoMovementTransitions = 4;
std::map<unsigned int, TankMovement*> TankMovement::movingTanks;

REGISTER_ACTION_SOURCE(TankMovement);

TankMovement::TankMovement() : 
	timePassed_(0.0f), vPoint_(0), fuel_(0),
	remove_(false), moving_(true), moveSoundSource_(0),
	smokeCounter_(0.1f, 0.1f), stepCount_(0)
{
}

TankMovement::TankMovement(unsigned int playerId,
	Fuel *fuel,
	int positionX, int positionY) : 
	playerId_(playerId), 
	positionX_(positionX), positionY_(positionY),
	timePassed_(0.0f), vPoint_(0), fuel_(fuel),
	remove_(false), moving_(true), moveSoundSource_(0),
	smokeCounter_(0.1f, 0.1f), stepCount_(0)
{
}

TankMovement::~TankMovement()
{
	if (vPoint_) context_->viewPoints->releaseViewPoint(vPoint_);
	delete moveSoundSource_;
	moveSoundSource_ = 0;
}

void TankMovement::init()
{
	movingTanks[playerId_] = this;

	Tank *tank = context_->tankContainer->getTankById(playerId_);
	if (!tank) return;	

	startPosition_ = tank->getTargetPosition();
	vPoint_ = context_->viewPoints->getNewViewPoint(playerId_);
	
	// Start the tank movement sound
	if (!context_->serverMode) 
	{
		SoundBuffer *moveSound = 
			Sound::instance()->fetchOrCreateBuffer((char *)
				getDataFile("data/wav/movement/tankmove.wav"));
		moveSoundSource_ = new VirtualSoundSource(VirtualSoundPriority::eAction, true, false);
		moveSoundSource_->setPosition(tank->getPosition().getTankPosition());
		moveSoundSource_->play(moveSound);
	}

	// As with everything to do with movement
	// The xy position is stored as an unsigned int
	// to save space, z is calculated from the landscape
	// Lower 32 bits = y position
	// Upper 32 bits = x positions
	std::list<unsigned int> positions;
	MovementMap mmap(
		context_->landscapeMaps->getDefinitions().getDefn()->landscapewidth,
		context_->landscapeMaps->getDefinitions().getDefn()->landscapeheight);
	mmap.calculateForTank(tank, 
		fuel_->getParent()->getAccessoryId(), 
		*context_, true);
	
	MovementMap::MovementMapEntry entry =
		mmap.getEntry(positionX_, positionY_);
	if (entry.type == MovementMap::eMovement)
	{
		// Add the end (destination) point to the list of points for the tank
		// to visit
		unsigned int pt = (positionX_ << 16) | (positionY_ & 0xffff);
		positions.push_front(pt);

		// Work backward to the source point and pre-pend them onto the
		// this of points
		while (entry.srcEntry)
		{
			pt = entry.srcEntry;
			unsigned int x = pt >> 16;
			unsigned int y = pt & 0xffff;
			positions.push_front(pt);
			entry = mmap.getEntry(x, y);
		}
	}
	
	// Expand these positions into a interpolated set of positions with
	// x, y and z
	std::list<unsigned int>::iterator itor;
	for (itor = positions.begin();
		itor != positions.end();)
	{
		unsigned int fistpt = (*itor);
		itor++;

		if (itor != positions.end())
		{
			unsigned int secpt = (*itor);

			int firstx = int(fistpt >> 16);
			int firsty = int(fistpt & 0xffff);
			int secx = int(secpt >> 16);
			int secy = int(secpt & 0xffff);
			int diffX = secx - firstx;
			int diffY = secy - firsty;
			float ang = (atan2f((float) diffY, (float) diffX) / 3.14f * 180.0f) - 90.0f;

			for (int i=0; i<NoMovementTransitions; i++)
			{
				float currentX = firstx + diffX/float(NoMovementTransitions)*float(i+1);
				float currentY = firsty + diffY/float(NoMovementTransitions)*float(i+1);
				expandedPositions_.push_back(
					PositionEntry(
						firstx, firsty, 
						secx, secy,
						currentX, currentY, 
						ang, (i==(NoMovementTransitions-1))));
			}
		}
	}
}

void TankMovement::simulate(float frameTime, bool &remove)
{
	if (!remove_)
	{
		if (moving_)
		{
			simulationMove(frameTime);
		}
	}
	else
	{
		remove = true;
	}

	if (remove && moveSoundSource_)
	{
		moveSoundSource_->stop();
	}
	
	ActionMeta::simulate(frameTime, remove);
}

void TankMovement::remove()
{
	remove_ = true;

	TankMovement::movingTanks.erase(playerId_);
}

void TankMovement::simulationMove(float frameTime)
{
	Tank *tank = 
		context_->tankContainer->getTankById(playerId_);
	if (tank)
	{
		// Stop moving if the tank is dead
		if (tank->getState().getState() == TankState::sNormal)
		{
			// Check to see if this tank is falling
			// If it is then we wait until the fall is over
			std::map<unsigned int, TankFalling *>::iterator findItor =
				TankFalling::fallingTanks.find(playerId_);
			if (findItor == TankFalling::fallingTanks.end())
			{
				// Add a smoke trail
				// Check if we are not on the server
				if (!context_->serverMode)
				{
					// Check if this tank type allows smoke trails
					TankModel *model = context_->tankModelStore->getModelByName(
						tank->getModelContainer().getTankModelName(),
						tank->getTeam(),
						(tank->getDestinationId() == 0));
					if (model && model->getMovementSmoke())
					{
						if (smokeCounter_.nextDraw(frameTime))
						{
							Landscape::instance()->getSmoke().addSmoke(
								tank->getTargetPosition()[0],
								tank->getTargetPosition()[1],
								tank->getTargetPosition()[2]);
						}
					}
				}

				// Move the tank one position every stepTime seconds
				// i.e. 1/stepTime positions a second
				timePassed_ += frameTime;
				float stepTime = fuel_->getStepTime();
				while (timePassed_ >= stepTime)
				{
					timePassed_ -= stepTime;
					if (!expandedPositions_.empty())
					{
						moveTank(tank);
					}
					else break;
				}

				if (expandedPositions_.empty()) moving_ = false;
			}
		}
		else moving_ = false;
	}
	else moving_ = false;

	if (moving_ == false)
	{
		// If this is the very last movement made
		// Ensure all tanks always end in the same place
		context_->actionController->addAction(
			new TankMovementEnd(tank->getPosition().getTankPosition(),
				tank->getPlayerId()));
	}
}

void TankMovement::moveTank(Tank *tank)
{
	float x = expandedPositions_.front().x;
	float y = expandedPositions_.front().y;
	float a = expandedPositions_.front().ang;
	bool useF = expandedPositions_.front().useFuel;

	int firstx = expandedPositions_.front().firstX;
	int firsty = expandedPositions_.front().firstY;
	float firstz = context_->landscapeMaps->getGroundMaps().getHeight(firstx, firsty);

	int secondx = expandedPositions_.front().secondX;
	int secondy = expandedPositions_.front().secondY;
	float secondz = context_->landscapeMaps->getGroundMaps().getHeight(secondx, secondy);
	float z = context_->landscapeMaps->getGroundMaps().getInterpHeight(x, y);
	expandedPositions_.pop_front();

	// Form the new tank position
	Vector newPos(x, y, z);

	// Check we are not trying to climb to high (this may be due
	// to the landscape changing after we started move)
	if (secondz - firstz > context_->optionsGame->getMaxClimbingDistance())
	{
		expandedPositions_.clear();
		return;
	}

	// Check to see we are not moving into water with a movement restriction
	// in place
	if (context_->optionsGame->getMovementRestriction() ==
		OptionsGame::MovementRestrictionLand ||
		context_->optionsGame->getMovementRestriction() ==
		OptionsGame::MovementRestrictionLandOrAbove)
	{
		float waterHeight = -10.0f;
		LandscapeTex &tex = *context_->landscapeMaps->getDefinitions().getTex();
		if (tex.border->getType() == LandscapeTexType::eWater)
		{
			LandscapeTexBorderWater *water = 
				(LandscapeTexBorderWater *) tex.border;
			waterHeight = water->height;
		}

		if (context_->optionsGame->getMovementRestriction() ==
			OptionsGame::MovementRestrictionLandOrAbove)
		{
			if (waterHeight > startPosition_[2] - 0.1f)
			{
				waterHeight = startPosition_[2] - 0.1f;
			}
		}

		if (secondz < waterHeight)
		{
			expandedPositions_.clear();
			return;
		}
	}

	// Move the tank to this new position
	// Use up one unit of fuel
	if (useF)
	{
		tank->getAccessories().rm(fuel_->getParent());
	}

	// Actually move the tank
	tank->getLife().setRotation(a);
	tank->setTargetPosition(newPos);

	// Add tracks
	if (!context_->serverMode)
	{
		stepCount_++;
		if (stepCount_ % 5 == 0)
		{
			TankModel *model = context_->tankModelStore->getModelByName(
				tank->getModelContainer().getTankModelName(),
				tank->getTeam(),
				(tank->getDestinationId() == 0));
			if (model)
			{
				GLImage *image = 0;
				if (firstx == secondx)
				{
					image = ImageStore::instance()->
						loadImage(model->getTracksVId());
				}
				else if (firsty == secondy)
				{
					image = ImageStore::instance()->
						loadImage(model->getTracksHId());
				}
				else if (firsty - secondy == firstx - secondx)
				{
					image = ImageStore::instance()->
						loadImage(model->getTracksVHId());
				}
				else 
				{
					image = ImageStore::instance()->
						loadImage(model->getTracksHVId());
				}

				GLBitmapModifier::addBitmapToLandscape(
					*context_,
					*image,
					newPos[0], 
					newPos[1],
					0.04f, 0.04f,
					true);
			}
		}
	}

	// Set viewpoints
	if (vPoint_) vPoint_->setPosition(newPos);
	if (moveSoundSource_) moveSoundSource_->setPosition(newPos);
}

bool TankMovement::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	context_->accessoryStore->writeAccessoryPart(buffer, fuel_);
	buffer.addToBuffer(positionX_);
	buffer.addToBuffer(positionY_);
	return true;
}

bool TankMovement::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	fuel_ = (Fuel *) context_->accessoryStore->readAccessoryPart(reader); if (!fuel_) return false;
	if (!reader.getFromBuffer(positionX_)) return false;
	if (!reader.getFromBuffer(positionY_)) return false;
	return true;
}
