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

TargetModelId::TargetModelId(const char *tankModelName) :
	tankModelName_(tankModelName),
	modelIdRenderer_(0)
{

}

TargetModelId::TargetModelId(ModelID &targetModel) :
	targetModel_(targetModel),
	modelIdRenderer_(0)
{

}

TargetModelId::TargetModelId(const TargetModelId &other) :
	tankModelName_(other.tankModelName_),
	targetModel_(other.targetModel_),
	modelIdRenderer_(0)
{

}

TargetModelId::~TargetModelId()
{
	delete modelIdRenderer_;
	modelIdRenderer_ = 0;
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
	buffer.addToBuffer(tankModelName_);
	targetModel_.writeModelID(buffer);
	return true;
}

bool TargetModelId::readMessage(NetBufferReader &reader)
{
	std::string newName;
	if (!reader.getFromBuffer(newName)) return false;
	if (0 != strcmp(newName.c_str(), tankModelName_.c_str()))
	{
		tankModelName_ = newName;
		delete modelIdRenderer_;
		modelIdRenderer_ = 0;
	}
	ModelID newId;
	if (!newId.readModelID(reader)) return false;
	if (0 != strcmp(newId.getStringHash(), targetModel_.getStringHash()))
	{
		targetModel_ = newId;
		delete modelIdRenderer_;
		modelIdRenderer_ = 0;
	}	

	return true;
}
