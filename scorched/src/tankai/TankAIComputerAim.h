////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_TankAIComputerAimh_INCLUDE__)
#define __INCLUDE_TankAIComputerAimh_INCLUDE__

#include <tank/Tank.h>

class TankAIComputerAim
{
public:
	TankAIComputerAim();
	virtual ~TankAIComputerAim();

	void setTank(Tank *tank);
	bool parseConfig(XMLNode *node);
	void newGame();
	void ourShotLanded(Weapon *weapon, Vector &position);
	bool aimAtTank(Tank *tank);

protected:
	struct MadeShot
	{
		float angleXYDegs; 
		float angleYZDegs;
		float power;
		Vector finalpos;
	} *lastShot_;

	Tank *currentTank_;
	float sniperDist_;
	std::map<unsigned int, std::list<MadeShot> > madeShots_;
	std::string aimType_;

	void randomAim();
	void refinedAim(Tank *tank, bool refine);
	bool refineLastShot(Tank *tank, float &angleXYDegs, 
		float &angleYZDegs, float &power);
};

#endif // __INCLUDE_TankAIComputerAimh_INCLUDE__
