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


#include <actions/TankFired.h>
#include <actions/Explosion.h>
#include <weapons/AccessoryStore.h>
#include <engine/ScorchedContext.h>
#include <common/SoundStore.h>
#include <common/OptionsParam.h>
#include <tankgraph/TankModelRenderer.h>

REGISTER_ACTION_SOURCE(TankFired);

TankFired::TankFired() : firstTime_(true)
{
}

TankFired::TankFired(unsigned int playerId,
					 Weapon *weapon,
					 float rotXY, float rotXZ) :
	playerId_(playerId), weapon_(weapon),
	rotXY_(rotXY), rotXZ_(rotXZ), firstTime_(true)
{

}

TankFired::~TankFired()
{
}

void TankFired::init()
{

}

void TankFired::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		Tank *tank = 
			context_->tankContainer.getTankById(playerId_);
		if (tank)
		{
			tank->getPhysics().rotateGunXY(rotXY_, false);
			tank->getPhysics().rotateGunYZ(rotXZ_, false);

			if (!context_->serverMode) 
			{
				TankModelRenderer *model = (TankModelRenderer *) 
					tank->getModel().getModelIdRenderer();
				if (model)
				{
					model->fired();
				}

				// play fired sound
				if (weapon_->getFiredSound())
				{
					SoundBuffer *firedSound = 
						SoundStore::instance()->fetchOrCreateBuffer( (char*)
							getDataFile("data/wav/%s", weapon_->getFiredSound()));
					firedSound->play();
				}
			}

			Explosion *explosion = new Explosion(tank->getPhysics().getTankGunPosition(), 3.0f, 
				weapon_, playerId_, false, Explosion::DeformNone);
			context_->actionController.addAction(explosion);
		}
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

bool TankFired::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(rotXY_);
	buffer.addToBuffer(rotXZ_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool TankFired::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(rotXY_)) return false;
	if (!reader.getFromBuffer(rotXZ_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}
