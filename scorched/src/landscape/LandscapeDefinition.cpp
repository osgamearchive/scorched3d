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

#include <landscape/LandscapeDefinition.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefn.h>
#include <common/Defines.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

LandscapeDefinition::LandscapeDefinition(
	LandscapeTex *tex, LandscapeDefn *defn,
	unsigned int seed) :
	tex_(tex), defn_(defn), seed_(seed)
{
}

bool LandscapeDefinition::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(seed_);
	if (!tex_->writeMessage(buffer)) return false;
	if (!defn_->writeMessage(buffer)) return false;
	return true;
}

bool LandscapeDefinition::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(seed_)) return false;
	tex_ = new LandscapeTex;
	if (!tex_->readMessage(reader)) return false;
	defn_ = new LandscapeDefn;
	if (!defn_->readMessage(reader)) return false;
	return true;
}

