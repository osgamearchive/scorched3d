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


// ShotProjectileMirv.h: interface for the ShotProjectileMirv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOTPROJECTILEMIRV_H__50E48B70_E646_4265_8D88_8BFBBDEDD848__INCLUDED_)
#define AFX_SHOTPROJECTILEMIRV_H__50E48B70_E646_4265_8D88_8BFBBDEDD848__INCLUDED_

#include <actions/ShotProjectile.h>
#include <weapons/Weapon.h>

class ShotProjectileMirv : public ShotProjectile 
{
public:
	ShotProjectileMirv();
	ShotProjectileMirv(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId, float width, 
		int noWarheads, bool spread = false);
	virtual ~ShotProjectileMirv();

	virtual void simulate(float timepassed, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(ShotProjectileMirv);

protected:
	float width_;
	int noWarheads_;
	bool spread_;
	bool up_;

	void apex(Vector &position, Vector &currentVelocity);
};

#endif // !defined(AFX_SHOTPROJECTILEMIRV_H__50E48B70_E646_4265_8D88_8BFBBDEDD848__INCLUDED_)
