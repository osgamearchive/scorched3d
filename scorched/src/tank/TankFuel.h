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

#if !defined(__INCLUDE_TankFuelh_INCLUDE__)
#define __INCLUDE_TankFuelh_INCLUDE__

#include <coms/NetBuffer.h>

class ScorchedContext;
class TankFuel
{
public:
	TankFuel(ScorchedContext &context);
	virtual ~TankFuel();

	void reset();
	void newGame();
	void rmFuel(int no);
	void addFuel(int no);

	int getNoFuel() { return fuelCount_; }

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	int fuelCount_;
	bool infinite_;
	ScorchedContext &context_;
};

#endif
