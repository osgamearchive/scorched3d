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
#include <coms/NetBuffer.h>
#include <GLW/GLWToolTip.h>
#include <string>

#define REGISTER_ACCESSORY_HEADER(x, y) \
	virtual const char *getAccessoryTypeName() { return #x ; } \
	virtual AccessoryType getType() { return y ; } \
	virtual Accessory *getAccessoryCopy() { return new x ; }

#define REGISTER_ACCESSORY_SOURCE(x) \
	struct META_##x { META_##x() { AccessoryMetaRegistration::addMap(#x , new x ); } }; \
	static META_##x META_IMPL_##x ;

class Accessory  
{
public:
	enum AccessoryType
	{
		AccessoryWeapon,
		AccessoryParachute,
		AccessoryShield,
		AccessoryAutoDefense,
		AccessoryBattery,
		AccessoryFuel
	};

	Accessory();
	virtual ~Accessory();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	virtual const char *getActivationSound();
	virtual const char *getName();
	virtual const char *getDescription();
	virtual const int getPrice();
	virtual const int getSellPrice();
	virtual const int getBundle();
	virtual const int getArmsLevel();
	GLWTip &getToolTip();
	const char *getIconName() { return iconName_.c_str(); }

	unsigned int getAccessoryId() { return accessoryId_; }
	bool &getPrimary() { return primary_; }
	virtual bool singular();

	virtual AccessoryType getType() = 0;
	virtual const char *getAccessoryTypeName() = 0;
	virtual Accessory *getAccessoryCopy() = 0;

protected:
	static unsigned int nextAccessoryId_;
	GLWTip toolTip_;
	std::string iconName_;
	std::string name_;
	std::string description_;
	std::string activationSound_;
	int price_;
	int bundle_;
	int armsLevel_;
	int sellPrice_;
	bool primary_;
	unsigned int accessoryId_;
};

class AccessoryMetaRegistration
{
public:
	static void addMap(const char *name, Accessory *action);
	static std::map<std::string, Accessory *> *accessoryMap;
	static Accessory *getNewAccessory(const char *name);
};

#endif // !defined(AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_)
