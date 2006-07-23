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
#include <XML/XMLParser.h>

class TankAIComputerBuyer;
class TankAIComputerAim
{
public:
	TankAIComputerAim();
	virtual ~TankAIComputerAim();

	enum AimResult
	{
		AimFailed = 0,
		AimOk,
		AimBurried
	};

	void setTank(Tank *tank);
	bool parseConfig(XMLNode *node);
	void newGame();
	void ourShotLanded(Weapon *weapon, Vector &position);
	AimResult aimAtTank(Tank *tank, TankAIComputerBuyer *buyer,
		float &distance, int &noShots, bool &sniper);

protected:
	struct MadeShot
	{
		float angleXYDegs; 
		float angleYZDegs;
		float power;
		Vector finalDist;
	};
	struct ShotListEntry
	{
		ShotListEntry() : 
			shotCount(0)
		{
		}
		int shotCount;
		Vector lastDist;
		std::list<MadeShot> shotList;
	};

	unsigned int lastShot_;
	Tank *currentTank_;
	float sniperDist_;
	float sniperWobble_;
	std::map<unsigned int, ShotListEntry> madeShots_;
	std::string aimType_;
	bool checkNearCollision_;

	AimResult randomAim(float &distance, int &noShots, bool &sniper);
	AimResult refinedAim(Tank *tank, TankAIComputerBuyer *buyer,
		bool refine, float &distance, int &noShots, bool &sniper);
	bool refineLastShot(Tank *tank, float &angleXYDegs, 
		float &angleYZDegs, float &power);
	bool oldRefineLastShot(Tank *tank, 
		float &angleXYDegs, float &angleYZDegs, float &power);
	bool newRefineLastShot(Tank *tank, 
		float &angleXYDegs, float &angleYZDegs, float &power);
};

#endif // __INCLUDE_TankAIComputerAimh_INCLUDE__
