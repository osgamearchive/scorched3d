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

#if !defined(AFX_TankAIComputerTosser_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_)
#define AFX_TankAIComputerTosser_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_

#include <map>
#include <list>
#include <common/Vector.h>
#include <tankai/TankAIComputerShooter.h>

class TankAIComputerTosser : public TankAIComputerShooter
{
public:
	TankAIComputerTosser();
	virtual ~TankAIComputerTosser();

	virtual void newGame();
	virtual bool parseConfig(XMLNode *node);
	virtual void ourShotLanded(Weapon *weapon, Vector &position);
	virtual void playMove(const unsigned state, float frameTime, 
		char *buffer, unsigned int keyState);

TANKAI_DEFINE(TankAIComputerTosser);

protected:
	struct MadeShot
	{
		float angleXYDegs; 
		float angleYZDegs;
		float power;
		Vector finalpos;
	} *lastShot_;

	std::map<unsigned int, std::list<MadeShot> > madeShots_;
	bool refineLastShot(Tank *tank, float &angleXYDegs, float &angleYZDegs, float &power);
	float sniperDist_;
};

#endif // !defined(AFX_TankAIComputerTosser_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_)
