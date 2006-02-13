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
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <XML/XMLFile.h>
#include <XML/XMLParser.h>
#include <tank/Tank.h>
#include <stdlib.h>

REGISTER_CLASS_SOURCE(EconomyFreeMarket);

static const char *getEconomyFileName()
{
	if (OptionsParam::instance()->getDedicatedServer())
	{
		return getSettingsFile(formatString("freemarket-%s-%i.xml",
			ScorchedServer::instance()->getOptionsGame().getMod(),
			ScorchedServer::instance()->getOptionsGame().getPortNo()));
	}
	return getSettingsFile(formatString("freemarket-%s-single.xml",
		ScorchedServer::instance()->getOptionsGame().getMod()));
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
		dialogMessage("EconomyFreeMarket", formatString(
			"Failed to parse freemarket.xml file : %s",
			file.getParserError()));
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
			XMLNode *nameNode, *buyNode;
			if (!node->getNamedChild("name", nameNode)) return false;
			if (!node->getNamedChild("buyprice", buyNode)) return false;

			Accessory *accessory = ScorchedServer::instance()->getAccessoryStore().
				findByPrimaryAccessoryName(nameNode->getContent());
			if (!accessory)
			{
				Logger::log(formatString(
					"Warning: Economy free market failed to find accessory named \"%s\"",
					nameNode->getContent()));
			}
			else
			{	
				// Check that this accessory is still valid
				// (just in case the file has been changed)
				if (accessory->getMaximumNumber() != 0 &&
					accessory->getStartingNumber() != -1 && 
					accessory->getType() == AccessoryPart::AccessoryWeapon)
				{
					// Set the actual accessory price (based on the last used market prices)
					int price = atoi(buyNode->getContent());
					setPrice(accessory, price);
				}
			}
		}
	}

	return true;	
}

bool EconomyFreeMarket::savePrices()
{
	FileLines file;
	file.addLine("<prices source=\"Scorched3D\">");

	std::list<Accessory *> weapons = 
		ScorchedServer::instance()->getAccessoryStore().getAllAccessories();
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{
		Accessory *accessory = *itor;

		if (accessory->getMaximumNumber() != 0 &&
			accessory->getStartingNumber() != -1 && 
			accessory->getType() == AccessoryPart::AccessoryWeapon)
		{
			std::string cleanName;
			std::string dirtyName(accessory->getName());
			XMLParser::removeSpecialChars(dirtyName, cleanName);
			file.addLine("  <accessory>");
			file.addLine(formatString("    <name>%s</name>", cleanName.c_str()));
			file.addLine(formatString("    <buyprice>%i</buyprice>", accessory->getPrice()));
			file.addLine("  </accessory>");
		}
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
			Accessory *accessory = 
				ScorchedServer::instance()->getAccessoryStore().
				findByAccessoryId((*itor).first);
			
			int price = accessory->getPrice() + diff;
			setPrice(accessory, price);
		}
	}
	newPrices_.clear();
}

void EconomyFreeMarket::accessoryBought(Tank *tank, 
		const char *accessoryName)
{
	// Find the bought accessory
	Accessory *boughtAccessory = 
		ScorchedServer::instance()->getAccessoryStore().
		findByPrimaryAccessoryName(accessoryName);
	DIALOG_ASSERT(boughtAccessory);

	// Find the list of accessories that this player could have bought
	// This list comprises of weapons that are similar in price to the one
	// that has been bought
	std::list<Accessory *> possibleAccessories;
	{
		std::list<Accessory *> weapons = 
			ScorchedServer::instance()->getAccessoryStore().
			getAllAccessories();
		std::list<Accessory *>::iterator itor;
		for (itor = weapons.begin();
			itor != weapons.end();
			itor++)
		{
			Accessory *accessory = *itor;
	
			if (accessory->getPrice() <= tank->getScore().getMoney() &&
				accessory->getPrice() >= int(float(boughtAccessory->getPrice()) * 0.5f) &&
				accessory->getPrice() <= int(float(boughtAccessory->getPrice()) * 1.5f) &&
				accessory->getStartingNumber() != -1 &&
				accessory->getMaximumNumber() != 0 &&
				accessory->getType() == AccessoryPart::AccessoryWeapon)
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

			// Figure out how much money was spent on this weapon
			int moneyDidAquire = 0;
			if (accessory == boughtAccessory) moneyDidAquire = 
				boughtAccessory->getPrice();

			// Figure out if this is more or less money than on average
			// should be spent on this weapon
			int adjustment = 
				ScorchedServer::instance()->getOptionsGame().
				getFreeMarketAdjustment();
			int priceDiff = 0;
			if (moneyDidAquire < moneyShouldAquire)
			{
				// This weapon was not bought, decrease its price
				priceDiff = -adjustment / int(possibleAccessories.size());
				priceDiff /= 2;
			}
			else if (moneyDidAquire >= moneyShouldAquire)
			{
				// This weapon was bought, increase its price
				priceDiff = int((float(adjustment) * (float(possibleAccessories.size()) - 1.0f))
					/ float(possibleAccessories.size()));
			}

			// Update the price difference for this weapon
			std::map<unsigned int, int>::iterator findItor = newPrices_.
				find(accessory->getAccessoryId());
			if (findItor == newPrices_.end()) 
				newPrices_[accessory->getAccessoryId()] = priceDiff;
			else newPrices_[accessory->getAccessoryId()] += priceDiff;
		}
	}
}

void EconomyFreeMarket::setPrice(Accessory *accessory, int price)
{
	price = (price / 10) * 10; // Round to 10

	// Make suse price does not get greater than 1.5X the original price
	if (price > int(float(accessory->getOriginalPrice()) * 1.5f))
		price = int(float(accessory->getOriginalPrice()) * 1.5f);
	// Make sure price does not get lower than 0.75X the original price
	else if (price < int(float(accessory->getOriginalPrice()) * 0.75f))
		price = int(float(accessory->getOriginalPrice()) * 0.75f);
	accessory->setPrice(price);

	// Sell price is 0.8X the buy price
	int selPrice = int(float(accessory->getPrice()) /
		float(accessory->getBundle()) * 0.8f);
	selPrice = (selPrice / 10) * 10; // Round to 10
	accessory->setSellPrice(selPrice);
}

void EconomyFreeMarket::accessorySold(Tank *tank, 
		const char *accessoryName)
{
	// Do nothing (yet) on a sold item
}

