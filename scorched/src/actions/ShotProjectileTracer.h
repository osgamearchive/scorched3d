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


// ShotProjectileTracer.h: interface for the ShotProjectileTracer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOTPROJECTILETRACER_H__52B7BD0D_0C99_4E50_B9F6_DF3F555946EF__INCLUDED_)
#define AFX_SHOTPROJECTILETRACER_H__52B7BD0D_0C99_4E50_B9F6_DF3F555946EF__INCLUDED_

#include <list>
#include <actions/ShotProjectile.h>

class ShotProjectileTracer : public ShotProjectile
{
public:
	ShotProjectileTracer();
	ShotProjectileTracer(Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId, bool smokeTracer);
	virtual ~ShotProjectileTracer();

	std::list<Vector> &getPositions() { return positions_; }
	bool getSmokeTracer() { return smokeTracer_; }

	virtual void init();
	virtual void simulate(float frametime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);
	virtual void collision(Vector &position);

	REGISTER_ACTION_HEADER(ShotProjectileTracer);

protected:
	bool smokeTracer_;
	float snapTime_;
	std::list<Vector> positions_;

};

#endif // !defined(AFX_SHOTPROJECTILETRACER_H__52B7BD0D_0C99_4E50_B9F6_DF3F555946EF__INCLUDED_)
