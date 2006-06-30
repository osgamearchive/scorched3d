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

#if !defined(AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_)
#define AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_

#include <XML/XMLFile.h>
#include <GLW/GLWToolTip.h>
#include <3dsparse/ModelID.h>
#include <weapons/AccessoryPart.h>
#include <string>
#include <map>

class Tank;
class MissileMesh;
class GLTexture;
class AccessoryStore;
class OptionsGame;
class Accessory  
{
public:
	Accessory();
	virtual ~Accessory();

	bool parseXML(OptionsGame &context,
		AccessoryStore *store, XMLNode *accessoryNode);

	const char *getActivationSound();
	const char *getName() { return name_.c_str(); }
	const char *getDescription() { return description_.c_str(); }
	const int getPrice() { return price_; }
	const int getSellPrice() { return sellPrice_; }
	const int getOriginalSellPrice() { return originalSellPrice_; }
	const int getOriginalPrice() { return originalPrice_; }
	const int getBundle() { return bundle_; }
	const int getArmsLevel() { return armsLevel_; }
	const int getMaximumNumber() { return maximumNumber_; }
	const int getStartingNumber() { return startingNumber_; }
	const bool getAIOnly() { return aiOnly_; }

	GLWTip &getToolTip() { return toolTip_; }
	const char *getIconName() { return iconName_.c_str(); }
	AccessoryPart *getAction() { return accessoryAction_; }
	float getModelScale() { return modelScale_; }
	ModelID &getModel() { return modelId_; }
	bool getMuzzleFlash() { return muzzleFlash_; }

	AccessoryPart::AccessoryType getType() { return accessoryAction_->getType(); }
	GLTexture *getTexture();

	void setPrice(int p) { if (p>0) price_ = p; }
	void setSellPrice(int p) { if (p>0) sellPrice_ = p; }

	static void resetAccessoryIds() { nextAccessoryId_ = 0; }
	unsigned int getAccessoryId() { return accessoryId_; }

	static MissileMesh *getWeaponMesh(ModelID &id, Tank *currentPlayer);

protected:
	static std::map<std::string, MissileMesh *> loadedMeshes_;
	static unsigned int nextAccessoryId_;
	unsigned int accessoryId_;
	bool aiOnly_;
	AccessoryPart *accessoryAction_;
	GLWTip toolTip_;
	GLTexture *texture_;
	ModelID modelId_;
	std::string iconName_;
	std::string name_;
	std::string description_;
	std::string activationSound_;
	int price_;
	int originalPrice_;
	int bundle_;
	int armsLevel_;
	int sellPrice_;
	int originalSellPrice_;
	int maximumNumber_;
	int startingNumber_;
	float modelScale_;
	bool muzzleFlash_;
};

#endif // !defined(AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_)
