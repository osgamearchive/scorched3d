///////////////////////////////////////////////////////////////////////////////
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
#include <server/ScorchedServer.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>
#include <tank/Tank.h>
#include <stdlib.h>

REGISTER_CLASS_SOURCE(EconomyFreeMarket);

static const char *getEconomyFileName()
{
	if (OptionsParam::instance()->getDedicatedServer())
	{
		return getSettingsFile("freemarket-server-%i.xml",
			ScorchedServer::instance()->getOptionsGame().getPortNo());
	}
	return getSettingsFile("freemarket-single.xml");
}

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
	if (!file.readFile(getEconomyFileName()))
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

			Accessory *accessory = AccessoryStore::instance()->
				findByPrimaryAccessoryName(nameNode->getContent());
			if (!accessory)
			{
				dialogMessage("EconomyFreeMarket",
					"Failed to find accessory named \"%s\"",
					nameNode->getContent());
				return false;
			}
	
			// Set the actual accessory price (based on the last used market prices)
			int price = atoi(buyNode->getContent());
			int sellPrice = atoi(sellNode->getContent());
			accessory->setPrice(price);
			accessory->setSellPrice(sellPrice);
		}
	}

	return true;	
}

bool EconomyFreeMarket::savePrices()
{
	FileLines file;
	file.addLine("<prices source=\"Scorched3D\">");

	std::list<Accessory *> weapons = AccessoryStore::instance()->
		getAllAccessories();
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{
		Accessory *accessory = *itor;

		file.addLine("  <accessory>");
		file.addLine("    <name>%s</name>", accessory->getName());
		file.addLine("    <buyprice>%i</buyprice>", accessory->getPrice());
		file.addLine("    <sellprice>%i</sellprice>", accessory->getSellPrice());
		file.addLine("  </accessory>");
	}
	file.addLine("</prices>");

	if (!file.writeFile((char *) getEconomyFileName())) return false;

	return true;
}

void EconomyFreeMarket::calculatePrices()
{
	std::map<unsigned int, int>::iterator itor;
	for (itor = newPrices_.begin();
		itor != newPrices_.end();
		itor++)
	{
		int diff = (*itor).second;
		if (diff != 0)
		{
			Accessory *accessory = AccessoryStore::instance()->
				findByAccessoryId((*itor).first);
			diff = (diff / 25) * 25; // Round to 25
			accessory->setPrice(accessory->getPrice() + diff);
			accessory->setSellPrice(int((accessory->getPrice() /
				accessory->getBundle()) * 0.8f));
		}
	}
	newPrices_.clear();
}

void EconomyFreeMarket::accessoryBought(Tank *tank, 
		const char *accessoryName)
{
	// Find the bought accessory
	Accessory *boughtAccessory = AccessoryStore::instance()->
		findByPrimaryAccessoryName(accessoryName);
	DIALOG_ASSERT(boughtAccessory);

	// Find the list of accessories that this player could have bought
	std::list<Accessory *> possibleAccessories;
	{
		std::list<Accessory *> weapons = AccessoryStore::instance()->
			getAllAccessories();
		std::list<Accessory *>::iterator itor;
		for (itor = weapons.begin();
			itor != weapons.end();
			itor++)
		{
			Accessory *accessory = *itor;
	
			if (accessory->getPrice() < tank->getScore().getMoney() &&
				accessory->getPrice() > boughtAccessory->getPrice() / 2 &&
				accessory->getPrice() < boughtAccessory->getPrice() * 1.5 &&
				accessory->getPrice() != 0 &&
				accessory->getType() == Accessory::AccessoryWeapon)
			{
				possibleAccessories.push_back(accessory);
			}
		}
	}

	// Nothing to do if we can only buy 1 item
	if (possibleAccessories.size() <= 1) return;

	// How much should each accessory get (on average)
	int moneyShouldAquire = boughtAccessory->getPrice() / possibleAccessories.size();
	
	// Alter prices
	{
		std::list<Accessory *>::iterator itor;
		for (itor = possibleAccessories.begin();
			itor != possibleAccessories.end();
			itor++)
		{
			Accessory *accessory = (*itor);
			int moneyDidAquire = 0;
			if (accessory == boughtAccessory) moneyDidAquire = 
				boughtAccessory->getPrice();

			int priceDiff = 0;
			if (moneyDidAquire < moneyShouldAquire)
			{
				priceDiff = -500 / int(possibleAccessories.size());
			}
			else if (moneyDidAquire >= moneyShouldAquire)
			{
				priceDiff = 500 / int(possibleAccessories.size());
			}
			std::map<unsigned int, int>::iterator findItor = newPrices_.
				find(accessory->getAccessoryId());
			if (findItor == newPrices_.end()) 
				newPrices_[accessory->getAccessoryId()] = priceDiff;
			else newPrices_[accessory->getAccessoryId()] += priceDiff;
		}
	}
}

void EconomyFreeMarket::accessorySold(Tank *tank, 
		const char *accessoryName)
{
	// Do nothing (yet) on a sold item
}

