////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_)
#define AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_

#include <weapons/AccessoryPart.h>
#include <net/NetBuffer.h>
#include <common/FixedVector.h>
#include <engine/ScorchedContext.h>
#include <engine/ObjectGroups.h>
#include <XML/XMLEntrySimpleTypes.h>

class Weapon;
class WeaponFireContextInternal
{
public:
	WeaponFireContextInternal(unsigned int selectPositionX, unsigned int selectPositionY,
		const FixedVector &velocityVectory,
		bool referenced, bool updateStats);
	virtual ~WeaponFireContextInternal();

	int getKillCount() { return killCount_; }
	void setKillCount(int killCount) { killCount_ = killCount; }

	bool getUpdateStats() { return updateStats_; }
	bool getReferenced() { return referenced_; }

	unsigned int getSelectPositionX() { return selectPositionX_; }
	unsigned int getSelectPositionY() { return selectPositionY_; }
	FixedVector &getVelocityVector() { return velocityVector_; }

	ObjectGroups &getLocalGroups() { return localGroups_; }
	std::set<Weapon *> &getWeaponStack() { return weaponStack_; }

	int getIncLabelCount(unsigned int label);

	void incrementReference();
	void decrementReference();

protected:
	int killCount_;
	bool referenced_, updateStats_;
	unsigned int referenceCount_;
	ObjectGroups localGroups_;
	unsigned int selectPositionX_;
	unsigned int selectPositionY_;
	FixedVector velocityVector_;
	std::map<unsigned int, int> *labelCount_;
	std::set<Weapon *> weaponStack_;

private:
	WeaponFireContextInternal(WeaponFireContextInternal &other);
	WeaponFireContextInternal &operator=(WeaponFireContextInternal &other);
};

class CameraPositionAction;
class TankViewPointProvider;
class WeaponCameraTrack : public XMLEntryContainer
{
public:
	WeaponCameraTrack();
	virtual ~WeaponCameraTrack();

	CameraPositionAction *createPositionAction(
		unsigned int playerId, TankViewPointProvider *provider,
		fixed defaultShowTime, unsigned int defaultPriority,
		bool explosion);
	bool getCameraTrack() { return cameraTrack_.getValue(); }

private:
	XMLEntryBool cameraTrack_;
	XMLEntryInt showPriority_;
	XMLEntryFixed showTime_;
};

class WeaponFireContext
{
public:
	WeaponFireContext(unsigned int playerId, 
		unsigned int selectPositionX, unsigned int selectPositionY,
		const FixedVector &velocityVector,
		bool referenced, bool updateStats);
	WeaponFireContext(WeaponFireContext &other);
	virtual ~WeaponFireContext();

	unsigned int getPlayerId() { return playerId_; }
	void setPlayerId(unsigned int playerId) { playerId_ = playerId; }
	
	WeaponFireContextInternal &getInternalContext() { return *internalContext_; }

protected:
	unsigned int playerId_;
	WeaponFireContextInternal *internalContext_;

private:
	WeaponFireContext &operator=(WeaponFireContext &other);
	
};

class Action;
class Weapon : public AccessoryPart
{
public:
	Weapon(const char *typeName, const char *description);
	virtual ~Weapon();
	
	virtual void fire(ScorchedContext &context,
		WeaponFireContext &weaponContext,
		FixedVector &position, FixedVector &velocity);

protected:

	virtual void addWeaponSyncCheck(ScorchedContext &context,
		WeaponFireContext &weaponContext,
		FixedVector &position, FixedVector &velocity);
	virtual void fireWeapon(ScorchedContext &context,
		WeaponFireContext &weaponContext,
		FixedVector &position, FixedVector &velocity) = 0;
};

class XMLEntryWeaponChoice : public XMLEntryTypeChoice<Weapon>
{
public:
	XMLEntryWeaponChoice();
	virtual ~XMLEntryWeaponChoice();

	virtual Weapon *createXMLEntry(const std::string &type, void *xmlData);
	virtual void getAllTypes(std::set<std::string> &allTypes);
};

#endif // !defined(AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_)
