#pragma once

#include <weapons/Weapon.h>
#include <3dsparse/ModelID.h>

class Shield;
class Parachute;
class PowerUp : public Weapon
{
public:
	PowerUp();
	virtual ~PowerUp();

	ModelID &getPowerUpModel() { return powerUpModelId_; }
	Shield *getShield() { return shield_; }
	Parachute *getParachute() { return parachute_; }

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);
	void fireWeapon(ScorchedContext &context, 
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(PowerUp, AccessoryPart::AccessoryWeapon);

protected:
	ModelID powerUpModelId_;
	Shield *shield_;
	Parachute *parachute_;
};
