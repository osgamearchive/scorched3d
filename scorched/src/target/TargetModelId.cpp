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

#include <target/TargetModelId.h>
#include <tankgraph/TargetModelIdRenderer.h>
#include <tankgraph/TankModelStore.h>
#include <engine/ScorchedContext.h>

TargetModelId::TargetModelId(const char *tankModelName) :
	tankModelName_(tankModelName),
	modelIdRenderer_(0),
	targetType_(eTankModel)
{

}

TargetModelId::TargetModelId(ModelID &targetModel) :
	targetModel_(targetModel),
	modelIdRenderer_(0),
	targetType_(eTargetModel)
{

}

TargetModelId::TargetModelId(const char *treeModel, bool) :
	treeModel_(treeModel),
	modelIdRenderer_(0),
	targetType_(eTreeModel)
{

}

TargetModelId::TargetModelId(const TargetModelId &other) :
	tankModelName_(other.tankModelName_),
	targetModel_(other.targetModel_),
	modelIdRenderer_(0),
	treeModel_(other.treeModel_),
	targetType_(other.targetType_)
{

}

TargetModelId::~TargetModelId()
{
	delete modelIdRenderer_;
	modelIdRenderer_ = 0;
}

TankType *TargetModelId::getTankType(ScorchedContext &context)
{
	TankModel *tankModel = 
		context.tankModelStore->getModelByName(tankModelName_.c_str(), 0);
	if (!tankModel) 
	{
		dialogExit("Scorched3D", 
			"Failed to find tank model %s",
			tankModelName_.c_str());
	}
	return tankModel->getTankType();
}

const TargetModelId & TargetModelId::operator=(const TargetModelId &other)
{
	tankModelName_ = other.tankModelName_;
	targetModel_ = other.targetModel_;
	modelIdRenderer_ = 0;
	return *this;
}

bool TargetModelId::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) targetType_);
	switch (targetType_)
	{
	case eTankModel:
		buffer.addToBuffer(tankModelName_);
		break;
	case eTargetModel:
		targetModel_.writeModelID(buffer);
		break;
	case eTreeModel:
		buffer.addToBuffer(treeModel_);
		break;
	}	
	return true;
}

bool TargetModelId::readMessage(NetBufferReader &reader)
{
	int targetType;
	if (!reader.getFromBuffer(targetType)) return false;
	targetType_ = (TargetModelType) targetType;

	switch (targetType_)
	{
	case eTankModel:
		{
			std::string newName;
			if (!reader.getFromBuffer(newName)) return false;
			if (0 != strcmp(newName.c_str(), tankModelName_.c_str()))
			{
				tankModelName_ = newName;
				delete modelIdRenderer_;
				modelIdRenderer_ = 0;
			}
		}
		break;
	case eTargetModel:
		if (!targetModel_.readModelID(reader)) return false;
		break;
	case eTreeModel:
		if (!reader.getFromBuffer(treeModel_)) return false;
		break;
	}	

	return true;
}
