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

#if !defined(AFX_TankAICurrentMove_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_)
#define AFX_TankAICurrentMove_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_

#include <tankai/TankAICurrentTarget.h>
#include <tankai/TankAICurrentMoveWeapons.h>

class Vector;
class TankAICurrentMove
{
public:
	TankAICurrentMove();
	virtual ~TankAICurrentMove();

	virtual bool parseConfig(XMLNode *node);

	void playMove(Tank *tank, 
		TankAIWeaponSets::WeaponSet *weapons,
		bool useBatteries);

	TankAICurrentTarget &getTargets() { return targets_; }

protected:
	TankAICurrentTarget targets_;
	bool useResign_;

	bool shootAtTank(Tank *tank, Tank *targetTank, 
		TankAICurrentMoveWeapons &weapons);
	bool makeProjectileShot(Tank *tank, Tank *targetTank,
		TankAICurrentMoveWeapons &weapons);
	bool makeSniperShot(Tank *tank, Tank *targetTank,
		TankAICurrentMoveWeapons &weapons);
	bool makeLaserSniperShot(Tank *tank, Tank *targetTank, 
		TankAICurrentMoveWeapons &weapons);
	bool makeBurriedShot(Tank *tank, Tank *targetTank, 
		TankAICurrentMoveWeapons &weapons);
	bool inHole(Vector &position);
	Vector lowestHighest(TankAICurrentMoveWeapons &weapons,
		Vector &position, bool highest);

	void setWeapon(Tank *tank, Accessory *accessory);
	void useBattery(Tank *tank, unsigned int batteryId);
	void resign(Tank *tank);
	void skipMove(Tank *tank);
	void fireShot(Tank *tank);

};

#endif // !defined(AFX_TankAICurrentMove_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_)
