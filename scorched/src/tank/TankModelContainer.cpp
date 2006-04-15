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

#include <tank/TankModelContainer.h>
#include <tankgraph/TankModelStore.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <engine/ScorchedContext.h>
#include <common/DefinesString.h>

TankModelContainer::TankModelContainer(const char *name) :
	tankModelName_(name),
	tankType_(0)
{

}

TankModelContainer::~TankModelContainer()
{
	tankType_ = 0;
}

TankType *TankModelContainer::getTankType(ScorchedContext &context)
{
	if (!tankType_) 
	{
		TankModel *tankModel = 
			context.tankModelStore->getModelByName(tankModelName_.c_str(), 
			tank_->getTeam(),
			(tank_->getDestinationId() == 0));
		if (!tankModel) 
		{
			dialogExit("Scorched3D", 
				formatString("Failed to find tank model %s",
				tankModelName_.c_str()));
		}
		tankType_ = tankModel->getTankType();
	}

	return tankType_;
}

void TankModelContainer::setTankModelName(const char *name)
{
	if (0 != strcmp(name, tankModelName_.c_str()))
	{
		if (tank_->getRenderer())
		{
			TargetRendererImplTank *renderer = (TargetRendererImplTank *)
				tank_->getRenderer();
			renderer->resetModel();
		}
		tankModelName_ = name;
		tankType_ = 0;
	}
}

bool TankModelContainer::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(tankModelName_);
	return true;
}

bool TankModelContainer::readMessage(NetBufferReader &reader)
{
	std::string newName;
	if (!reader.getFromBuffer(newName)) return false;
	setTankModelName(newName.c_str());
	return true;
}
