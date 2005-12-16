#include <weapons/PowerUp.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <actions/AddPowerUp.h>
#include <engine/ActionController.h>
#include <engine/ScorchedContext.h>
#include <tankai/TankAIAdder.h>
#include <landscape/LandscapeMaps.h>

REGISTER_ACCESSORY_SOURCE(PowerUp);

PowerUp::PowerUp() : 
	shield_(0), parachute_(0)
{
}

PowerUp::~PowerUp()
{
}

bool PowerUp::parseXML(OptionsGame &context,
	AccessoryStore *store, XMLNode *accessoryNode)
{
	XMLNode *modelNode = 0;
	if (accessoryNode->getNamedChild("powerupmodel", modelNode))
	{
		if (!powerUpModelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	std::string shield;
	if (!accessoryNode->getNamedChild("shield", shield)) return false;
	if (0 != strcmp(shield.c_str(), "none"))
	{
		shield_ = (Shield *) store->
			findByPrimaryAccessoryName(shield.c_str());
	}

	std::string parachute;
	if (!accessoryNode->getNamedChild("parachute", parachute)) return false;
	if (0 != strcmp(parachute.c_str(), "none"))
	{
		parachute_ = (Parachute *) store->
			findByPrimaryAccessoryName(parachute.c_str());
	}

	return accessoryNode->failChildren();
}

void PowerUp::fireWeapon(ScorchedContext &context, 
	unsigned int playerId, Vector &oldPosition, Vector &velocity,
	unsigned int data)
{
	Vector position = oldPosition;
	position[2] = context.landscapeMaps->
		getGroundMaps().getHeight((int) position[0], (int) position[1]);

	Action *action = new AddPowerUp(TankAIAdder::getNextTankId(), position, this);
	context.actionController->addAction(action);
}
