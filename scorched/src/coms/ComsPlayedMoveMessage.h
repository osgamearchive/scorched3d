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


#if !defined(__INCLUDE_ComsPlayedMoveMessageh_INCLUDE__)
#define __INCLUDE_ComsPlayedMoveMessageh_INCLUDE__

#include <coms/ComsMessage.h>

class ComsPlayedMoveMessage : public ComsMessage
{
public:
	enum MoveType
	{
		eNone,
		eShot,
		eResign,
		eMove,
		eSkip
	};

	ComsPlayedMoveMessage(MoveType type = eNone);
	virtual ~ComsPlayedMoveMessage();

	void setPosition(int x, int y);
	void setShot(const char *weaponName,
		float rotationXY,
		float rotationYZ,
		float power);

	const char *getWeaponName() { return weaponName_.c_str(); }
	float getRotationXY() { return rotationXY_; }
	float getRotationYZ() { return rotationYZ_; }
	int getPositionX() { return (int) rotationXY_; }
	int getPositionY() { return (int) rotationYZ_; }
	float getPower() { return power_; }
	MoveType getType() { return moveType_; }

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer);
    virtual bool readMessage(NetBufferReader &reader);

protected:
	MoveType moveType_;
	std::string weaponName_;
	float rotationXY_;
	float rotationYZ_;
	float power_;

private:
	ComsPlayedMoveMessage(const ComsPlayedMoveMessage &);
	const ComsPlayedMoveMessage & operator=(const ComsPlayedMoveMessage &);
};


#endif
