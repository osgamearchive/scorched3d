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

#include <weapons/EconomyFreeMarket.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>
#include <tank/Tank.h>
#include <stdlib.h>

REGISTER_CLASS_SOURCE(EconomyFreeMarket);

EconomyFreeMarket::EconomyFreeMarket()
{
}

EconomyFreeMarket::~EconomyFreeMarket()
{
}

bool EconomyFreeMarket::loadPrices()
{
	// Parse the file containing the last used prices
	XMLFile file;
	if (!file.readFile(getSettingsFile("freemarket.xml")))
	{
		dialogMessage("EconomyFreeMarket",
			"Failed to parse freemarket.xml file : %s",
			file.getParserError());
		return false;
	}

	// If there are entries in the file
	XMLNode *rootnode = file.getRootNode();
	if (rootnode)
	{
		std::list<XMLNode *> &nodes = rootnode->getChildren();
		std::list<XMLNode *>::iterator itor;
		for (itor = nodes.begin();
			itor != nodes.end();
			itor++)
		{
			XMLNode *node = *itor;
			XMLNode *nameNode = node->getNamedChild("name");
			XMLNode *sellNode = node->getNamedChild("sellprice");
			XMLNode *buyNode = node->getNamedChild("buyprice");
			if (!nameNode || !sellNode || !buyNode)
			{
				dialogMessage("EconomyFreeMarket",
					"Failed to find name or price nodes");
				return false;
			}

			prices_[nameNode->getContent()] = 
				PriceEntry(atoi(buyNode->getContent()),
					atoi(sellNode->getContent()));
		}
	}

	// Make sure all accessories are catered for
	std::list<Accessory *> otherList = AccessoryStore::instance()->getAllOthers();
	std::list<Accessory *> weaponList = AccessoryStore::instance()->getAllWeapons();
	otherList.insert(otherList.end(), weaponList.begin(), weaponList.end());
	std::list<Accessory *>::iterator itor;
	for (itor = otherList.begin();
		itor != otherList.end();
		itor++)
	{
		Accessory *current = *itor;
		std::map<std::string, PriceEntry>::iterator findItor = 
			prices_.find(current->getName());
		if (findItor == prices_.end())
		{
			prices_[current->getName()] = 
				PriceEntry(current->getPrice(), current->getSellPrice());
		}
	}

	return true;	
}

bool EconomyFreeMarket::savePrices()
{
	FileLines file;
	file.addLine("<prices source=\"Scorched3D\">");
	std::map<std::string, PriceEntry>::iterator itor;
	for (itor = prices_.begin();
		itor != prices_.end();
		itor++)
	{
		std::string name = (*itor).first;
		PriceEntry &price = (*itor).second;

		file.addLine("  <accessory>");
		file.addLine("    <name>%s</name>", name.c_str());
		file.addLine("    <buyprice>%i</buyprice>", price.buyPrice);
		file.addLine("    <sellprice>%i</sellprice>", price.sellPrice);
		file.addLine("  </accessory>");
	}
	file.addLine("</prices>");

	if (!file.writeFile((char *) getSettingsFile("freemarket.xml"))) return false;

	return true;
}

void EconomyFreeMarket::calculatePrices()
{

}

void EconomyFreeMarket::accessoryBought(Tank *tank, 
		const char *accessoryName)
{

}

void EconomyFreeMarket::accessorySold(Tank *tank, 
		const char *accessoryName)
{

}

int EconomyFreeMarket::getAccessoryBuyPrice(
		const char *accessoryName)
{
	Accessory *accessory = AccessoryStore::instance()->
		findByPrimaryAccessoryName(accessoryName);
	DIALOG_ASSERT(accessory);
	return accessory->getPrice();
}

int EconomyFreeMarket::getAccessorySellPrice(
		const char *accessoryName)
{
	Accessory *accessory = AccessoryStore::instance()->
		findByPrimaryAccessoryName(accessoryName);
	DIALOG_ASSERT(accessory);
	return accessory->getSellPrice();
}

