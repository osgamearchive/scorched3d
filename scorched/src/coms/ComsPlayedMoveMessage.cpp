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


#include <coms/ComsPlayedMoveMessage.h>

ComsPlayedMoveMessage::ComsPlayedMoveMessage(unsigned int playerId, MoveType type) :
	ComsMessage("ComsPlayedMoveMessage"),
	moveType_(type),
	weaponName_(""),
	rotationXY_(0.0f), rotationYZ_(0.0f), power_(0.0f), playerId_(playerId)
{
}

ComsPlayedMoveMessage::~ComsPlayedMoveMessage()
{
}

void ComsPlayedMoveMessage::setPosition(int x, int y)
{
	rotationXY_ = (float)x;
	rotationYZ_ = (float)y;
}

void ComsPlayedMoveMessage::setShot(const char *weaponName,
		float rotationXY,
		float rotationYZ,
		float power)
{
	weaponName_ = weaponName;
	rotationXY_ = rotationXY;
	rotationYZ_ = rotationYZ;
	power_ = power;
}

bool ComsPlayedMoveMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer((int) moveType_);
	if (moveType_ == eMove)
	{
		buffer.addToBuffer(rotationXY_);
		buffer.addToBuffer(rotationYZ_);
	}
	else if (moveType_ == eShot)
	{
		buffer.addToBuffer(weaponName_);
		buffer.addToBuffer(rotationXY_);
		buffer.addToBuffer(rotationYZ_);
		buffer.addToBuffer(power_);
	}
	return true;
}

bool ComsPlayedMoveMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	int mt;
	if (!reader.getFromBuffer(mt)) return false;
	moveType_ = (MoveType) mt;
	if (moveType_ == eMove)
	{
		if (!reader.getFromBuffer(rotationXY_)) return false;
		if (!reader.getFromBuffer(rotationYZ_)) return false;
	}
	else if (moveType_ == eShot)
	{
		if (!reader.getFromBuffer(weaponName_)) return false;
		if (!reader.getFromBuffer(rotationXY_)) return false;
		if (!reader.getFromBuffer(rotationYZ_)) return false;
		if (!reader.getFromBuffer(power_)) return false;
	}
	return true;
}