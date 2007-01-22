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

#include <landscapedef/LandscapeDefinition.h>
#include <coms/ComsLevelMessage.h>
#include <common/Defines.h>

ComsLevelMessage::ComsLevelMessage() :
	ComsMessage("ComsLevelMessage")
{

}

ComsLevelMessage::~ComsLevelMessage()
{
}

void ComsLevelMessage::createMessage(LandscapeDefinition &hdef)
{
	hdef_ = hdef;
}

LandscapeDefinition &ComsLevelMessage::getGroundMapsDefn()
{ 
	return hdef_; 
}

bool ComsLevelMessage::writeMessage(NetBuffer &buffer)
{
	if (!hdef_.writeMessage(buffer)) return false;
	if (!hMap_.writeMessage(buffer)) return false;
	return true;
}

bool ComsLevelMessage::readMessage(NetBufferReader &reader)
{
	if (!hdef_.readMessage(reader)) return false;
	if (!hMap_.readMessage(reader)) return false;
	return true;
}

