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

#if !defined(AFX_TANKSHIELDS_H__88E5EA32_F84D_41E3_AF97_01CBD2874CE3__INCLUDED_)
#define AFX_TANKSHIELDS_H__88E5EA32_F84D_41E3_AF97_01CBD2874CE3__INCLUDED_

#include <coms/NetBuffer.h>
#include <map>
#include <list>

class Accessory;
class ScorchedContext;
class Tank;
class TankShields  
{
public:
	TankShields(ScorchedContext &context);
	virtual ~TankShields();

	void setTank(Tank *tank) { tank_ = tank; }

	void newMatch();
	void changed();

	// Serialize
    bool writeMessage(NetBuffer &buffer, bool writeAccessories);
    bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	Tank *tank_;

};

#endif // !defined(AFX_TANKSHIELDS_H__88E5EA32_F84D_41E3_AF97_01CBD2874CE3__INCLUDED_)
