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

#ifndef _TankModelContainer_h
#define _TankModelContainer_h

#include <coms/NetBuffer.h>

// The model used for this tank
class ScorchedContext;
class TankType;
class Tank;
class TankModelContainer
{
public:
	TankModelContainer(const char *name);
	virtual ~TankModelContainer();

	void setTank(Tank *tank) { tank_ = tank; }

	// The name of the model that should be used for this tank
	const char *getTankModelName() { return tankModelName_.c_str(); }
	void setTankModelName(const char *name);

	// The type of this tank
	TankType *getTankType(ScorchedContext &context);

	// Serialize the modelid
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	Tank *tank_;
	std::string tankModelName_; // Model for tank
	TankType *tankType_;

};

#endif // _TankModelContainer_h
