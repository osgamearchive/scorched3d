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

#include <landscape/LandscapeTex.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static LandscapeTexType *fetchBorderTexType(const char *type)
{
	if (0 == strcmp(type, "water")) return new LandscapeTexBorderWater;
	if (0 == strcmp(type, "none")) return new LandscapeTexTypeNone;
	dialogMessage("LandscapeTexType", "Unknown border type %s", type);
	return 0;
}

static LandscapeTexType *fetchTextureTexType(const char *type)
{
	if (0 == strcmp(type, "generate")) return new LandscapeTexTextureGenerate;
	dialogMessage("LandscapeTexType", "Unknown texture type %s", type);
	return 0;
}

static LandscapeTexType *fetchPlacementTexType(const char *type)
{
	if (0 == strcmp(type, "trees")) return new LandscapeTexObjectsPlacementTree;
	if (0 == strcmp(type, "mask")) return new LandscapeTexObjectsPlacementMask;
	dialogMessage("LandscapeTexType", "Unknown placement type %s", type);
	return 0;
}

static LandscapeTexType *fetchObjectTexType(const char *type)
{
	if (0 == strcmp(type, "tree")) return new LandscapeTexObjectsTree;
	if (0 == strcmp(type, "model")) return new LandscapeTexObjectsModel;
	dialogMessage("LandscapeTexType", "Unknown object type %s", type);
	return 0;
}

static LandscapeTexType *fetchPrecipitationTexType(const char *type)
{
	if (0 == strcmp(type, "none")) return new LandscapeTexTypeNone;
	if (0 == strcmp(type, "rain")) return new LandscapeTexPrecipitation;
	if (0 == strcmp(type, "snow")) return new LandscapeTexPrecipitation;
	dialogMessage("LandscapeTexType", "Unknown precipitation type %s", type);
	return 0;
}

static LandscapeTexCondition *fetchConditionTexType(const char *type)
{
	if (0 == strcmp(type, "time")) return new LandscapeTexConditionTime;
	dialogMessage("LandscapeTexType", "Unknown condition type %s", type);
	return 0;
}

static LandscapeTexAction *fetchActionTexType(const char *type)
{
	if (0 == strcmp(type, "fireweapon")) return new LandscapeTexActionFireWeapon;
	dialogMessage("LandscapeTexType", "Unknown action type %s", type);
	return 0;
}

bool LandscapeTexTypeNone::writeMessage(NetBuffer &buffer)
{
	return true;
}

bool LandscapeTexTypeNone::readMessage(NetBufferReader &reader)
{
	return true;
}

bool LandscapeTexTypeNone::readXML(XMLNode *node)
{
	return node->failChildren();
}

// LandscapeTexEvent
LandscapeTexEvent::~LandscapeTexEvent()
{
	delete condition;
	delete action;
}

bool LandscapeTexEvent::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(conditiontype);
	if (!condition->writeMessage(buffer)) return false;
	buffer.addToBuffer(actiontype);
	if (!action->writeMessage(buffer)) return false;
	return true;
}

bool LandscapeTexEvent::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(conditiontype)) return false;
	if (!(condition = fetchConditionTexType(conditiontype.c_str()))) return false;
	if (!condition->readMessage(reader)) return false;
	if (!reader.getFromBuffer(actiontype)) return false;
	if (!(action = fetchActionTexType(actiontype.c_str()))) return false;
	if (!action->readMessage(reader)) return false;	
	return true;
}

bool LandscapeTexEvent::readXML(XMLNode *node)
{
	{
		XMLNode *conditionNode;
		if (!node->getNamedChild("condition", conditionNode)) return false;
		if (!conditionNode->getNamedParameter("type", conditiontype)) return false;
		if (!(condition = fetchConditionTexType(conditiontype.c_str()))) return false;
		if (!condition->readXML(conditionNode)) return false;
	}
	{
		XMLNode *actionNode;
		if (!node->getNamedChild("action", actionNode)) return false;
		if (!actionNode->getNamedParameter("type", actiontype)) return false;
		if (!(action = fetchActionTexType(actiontype.c_str()))) return false;
		if (!action->readXML(actionNode)) return false;
	}	
	
	return node->failChildren();
}

// LandscapeTexConditionTime
float LandscapeTexConditionTime::getNextEventTime()
{
	return RAND * (maxtime - mintime) + mintime;
}

bool LandscapeTexConditionTime::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(mintime);
	buffer.addToBuffer(maxtime);
	return true;
}

bool LandscapeTexConditionTime::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(mintime)) return false;
	if (!reader.getFromBuffer(maxtime)) return false;
	return true;
}

bool LandscapeTexConditionTime::readXML(XMLNode *node)
{
	if (!node->getNamedChild("mintime", mintime)) return false;
	if (!node->getNamedChild("maxtime", maxtime)) return false;
	return node->failChildren();
}

// LandscapeTexActionFireWeapon
void LandscapeTexActionFireWeapon::fireAction(ScorchedContext &context)
{
	Accessory *accessory = 
		context.accessoryStore->findByPrimaryAccessoryName(
			weapon.c_str());
	if (!accessory) dialogExit("LandscapeTexActionFireWeapon",
		"Failed to find weapon named \"%s\"", weapon.c_str());
	if (accessory->getType() != Accessory::AccessoryWeapon) 
		dialogExit("LandscapeTexActionFireWeapon",
			"Accessory named \"%s\" is not a weapon", weapon.c_str());
	Weapon *weapon = (Weapon *) accessory;

	Vector pos, vel;
	pos[0] += position[0] - positionoffset[0] + 
		positionoffset[0] * 2.0f * RAND;
	pos[1] += position[1] - positionoffset[1] + 
		positionoffset[1] * 2.0f * RAND;
	pos[2] += position[2] - positionoffset[2] + 
		positionoffset[2] * 2.0f * RAND;
	vel[0] += direction[0] - directionoffset[0] + 
		directionoffset[0] * 2.0f * RAND;
	vel[1] += direction[1] - directionoffset[1] + 
		directionoffset[1] * 2.0f * RAND;
	vel[2] += direction[2] - directionoffset[2] + 
		directionoffset[2] * 2.0f * RAND;

	weapon->fireWeapon(context, 0, pos, vel, 
		Weapon::eDataDeathAnimation);
}

bool LandscapeTexActionFireWeapon::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(position);
	buffer.addToBuffer(positionoffset);
	buffer.addToBuffer(direction);
	buffer.addToBuffer(directionoffset);
	buffer.addToBuffer(weapon);
	return true;
}

bool LandscapeTexActionFireWeapon::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position)) return false;
	if (!reader.getFromBuffer(positionoffset)) return false;
	if (!reader.getFromBuffer(direction)) return false;
	if (!reader.getFromBuffer(directionoffset)) return false;
	if (!reader.getFromBuffer(weapon)) return false;
	return true;
}

bool LandscapeTexActionFireWeapon::readXML(XMLNode *node)
{
	if (!node->getNamedChild("position", position)) return false;
	if (!node->getNamedChild("positionoffset", positionoffset)) return false;
	if (!node->getNamedChild("direction", direction)) return false;
	if (!node->getNamedChild("directionoffset", directionoffset)) return false;
	if (!node->getNamedChild("weapon", weapon)) return false;
	return node->failChildren();
}

// LandscapeTexObjectsModel
bool LandscapeTexObjectsModel::writeMessage(NetBuffer &buffer)
{
	if (!model.writeModelID(buffer)) return false;
	if (!modelburnt.writeModelID(buffer)) return false;
	return true;
}

bool LandscapeTexObjectsModel::readMessage(NetBufferReader &reader)
{
	if (!model.readModelID(reader)) return false;
	if (!modelburnt.readModelID(reader)) return false;
	return true;
}

bool LandscapeTexObjectsModel::readXML(XMLNode *node)
{
	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!model.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("modelburnt", burntmodelnode)) return false;
	if (!modelburnt.initFromNode(".", burntmodelnode)) return false;
	return node->failChildren();
}

// LandscapeTexObjectsTree
bool LandscapeTexObjectsTree::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(tree);
	buffer.addToBuffer(snow);
	return true;
}

bool LandscapeTexObjectsTree::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(tree)) return false;
	if (!reader.getFromBuffer(snow)) return false;
	return true;
}

bool LandscapeTexObjectsTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("tree", tree)) return false;
	if (!node->getNamedChild("snow", snow)) return false;
	return node->failChildren();
}

// LandscapeTexPrecipitation
bool LandscapeTexPrecipitation::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(particles);
	return true;
}

bool LandscapeTexPrecipitation::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(particles)) return false;
	return true;
}

bool LandscapeTexPrecipitation::readXML(XMLNode *node)
{
	if (!node->getNamedChild("particles", particles)) return false;
	return node->failChildren();
}

// LandscapeTexObjectsPlacementTree
LandscapeTexObjectsPlacement::~LandscapeTexObjectsPlacement()
{
	delete object;
}

bool LandscapeTexObjectsPlacement::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(objecttype);
	if (!object->writeMessage(buffer)) return false;
	return true;
}

bool LandscapeTexObjectsPlacement::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(objecttype)) return false;
	if (!(object = fetchObjectTexType(objecttype.c_str()))) return false;
	if (!object->readMessage(reader)) return false;
	return true;
}

bool LandscapeTexObjectsPlacement::readXML(XMLNode *node)
{
	XMLNode *objectNode;
	if (!node->getNamedChild("object", objectNode)) return false;
	if (!objectNode->getNamedParameter("type", objecttype)) return false;
	if (!(object = fetchObjectTexType(objecttype.c_str()))) return false;
	if (!object->readXML(objectNode)) return false;
	return node->failChildren();
}

// LandscapeTexObjectsPlacementMask
bool LandscapeTexObjectsPlacementMask::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(numobjects);
	buffer.addToBuffer(mask);
	buffer.addToBuffer(minheight);
	buffer.addToBuffer(maxheight);
	buffer.addToBuffer(mincloseness);
	buffer.addToBuffer(minslope);
	buffer.addToBuffer(xsnap);
	buffer.addToBuffer(ysnap);
	buffer.addToBuffer(angsnap);
	return LandscapeTexObjectsPlacement::writeMessage(buffer);
}

bool LandscapeTexObjectsPlacementMask::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(numobjects)) return false;
	if (!reader.getFromBuffer(mask)) return false;
	if (!reader.getFromBuffer(minheight)) return false;
	if (!reader.getFromBuffer(maxheight)) return false;
	if (!reader.getFromBuffer(mincloseness)) return false;
	if (!reader.getFromBuffer(minslope)) return false;
	if (!reader.getFromBuffer(xsnap)) return false;
	if (!reader.getFromBuffer(ysnap)) return false;
	if (!reader.getFromBuffer(angsnap)) return false;
	return LandscapeTexObjectsPlacement::readMessage(reader);
}

bool LandscapeTexObjectsPlacementMask::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("mask", mask)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	if (!node->getNamedChild("mincloseness", mincloseness)) return false;
	if (!node->getNamedChild("minslope", minslope)) return false;
	if (!node->getNamedChild("xsnap", xsnap)) return false;
	if (!node->getNamedChild("ysnap", ysnap)) return false;
	if (!node->getNamedChild("angsnap", angsnap)) return false;
	return LandscapeTexObjectsPlacement::readXML(node);
}

// LandscapeTexObjectsPlacementTree
bool LandscapeTexObjectsPlacementTree::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(numobjects);
	buffer.addToBuffer(numclusters);
	buffer.addToBuffer(minheight);
	buffer.addToBuffer(maxheight);
	return LandscapeTexObjectsPlacement::writeMessage(buffer);
}

bool LandscapeTexObjectsPlacementTree::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(numobjects)) return false;
	if (!reader.getFromBuffer(numclusters)) return false;
	if (!reader.getFromBuffer(minheight)) return false;
	if (!reader.getFromBuffer(maxheight)) return false;
	return LandscapeTexObjectsPlacement::readMessage(reader);
}

bool LandscapeTexObjectsPlacementTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("numclusters", numclusters)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	return LandscapeTexObjectsPlacement::readXML(node);
}

// LandscapeTexBorderWater 
bool LandscapeTexBorderWater::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(reflection);
	buffer.addToBuffer(texture);
	buffer.addToBuffer(wavecolor);
	buffer.addToBuffer(wavetexture1);
	buffer.addToBuffer(wavetexture2);
	buffer.addToBuffer(height);
	return true;
}

bool LandscapeTexBorderWater::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(reflection)) return false;
	if (!reader.getFromBuffer(texture)) return false;
	if (!reader.getFromBuffer(wavecolor)) return false;
	if (!reader.getFromBuffer(wavetexture1)) return false;
	if (!reader.getFromBuffer(wavetexture2)) return false;
	if (!reader.getFromBuffer(height)) return false;
	return true;
}

bool LandscapeTexBorderWater::readXML(XMLNode *node)
{
	if (!node->getNamedChild("reflection", reflection)) return false;
	if (!node->getNamedChild("texture", texture)) return false;
	if (!node->getNamedChild("wavecolor", wavecolor)) return false;
	if (!node->getNamedChild("wavetexture1", wavetexture1)) return false;
	if (!node->getNamedChild("wavetexture2", wavetexture2)) return false;
	if (!node->getNamedChild("height", height)) return false;
	if (!checkDataFile(reflection.c_str())) return false;
	if (!checkDataFile(texture.c_str())) return false;
	if (!checkDataFile(wavetexture1.c_str())) return false;
	if (!checkDataFile(wavetexture2.c_str())) return false;
	return node->failChildren();
}

// LandscapeTexTextureGenerate
bool LandscapeTexTextureGenerate::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(roof);
	buffer.addToBuffer(surround);
	buffer.addToBuffer(rockside);
	buffer.addToBuffer(shore);
	buffer.addToBuffer(texture0);
	buffer.addToBuffer(texture1);
	buffer.addToBuffer(texture2);
	buffer.addToBuffer(texture3);
	buffer.addToBuffer(texture4);
	return true;
}

bool LandscapeTexTextureGenerate::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(roof)) return false;
	if (!reader.getFromBuffer(surround)) return false;
	if (!reader.getFromBuffer(rockside)) return false;
	if (!reader.getFromBuffer(shore)) return false;
	if (!reader.getFromBuffer(texture0)) return false;
	if (!reader.getFromBuffer(texture1)) return false;
	if (!reader.getFromBuffer(texture2)) return false;
	if (!reader.getFromBuffer(texture3)) return false;
	if (!reader.getFromBuffer(texture4)) return false;
	return true;
}

bool LandscapeTexTextureGenerate::readXML(XMLNode *node)
{
	if (!node->getNamedChild("roof", roof)) return false;
	if (!node->getNamedChild("surround", surround)) return false;
	if (!node->getNamedChild("rockside", rockside)) return false;
	if (!node->getNamedChild("shore", shore)) return false;
	if (!node->getNamedChild("texture0", texture0)) return false;
	if (!node->getNamedChild("texture1", texture1)) return false;
	if (!node->getNamedChild("texture2", texture2)) return false;
	if (!node->getNamedChild("texture3", texture3)) return false;
	if (!node->getNamedChild("texture4", texture4)) return false;
	if (!checkDataFile(surround.c_str())) return false;
	if (!checkDataFile(roof.c_str())) return false;
	if (!checkDataFile(rockside.c_str())) return false;
	if (!checkDataFile(shore.c_str())) return false;
	if (!checkDataFile(texture0.c_str())) return false;
	if (!checkDataFile(texture1.c_str())) return false;
	if (!checkDataFile(texture2.c_str())) return false;
	if (!checkDataFile(texture3.c_str())) return false;
	if (!checkDataFile(texture4.c_str())) return false;
	return node->failChildren();
}

// LandscapeTex
LandscapeTex::LandscapeTex() :
	border(0), texture(0)
{
}

LandscapeTex::~LandscapeTex()
{
	delete border;
	delete texture;
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	for (unsigned int i=0; i<events.size(); i++)
	{
		delete events[i];
	}
	objects.clear();
	objectstype.clear();
	events.clear();
}

bool LandscapeTex::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(name);
	buffer.addToBuffer(detail);
	buffer.addToBuffer(magmasmall);
	buffer.addToBuffer(scorch);
	buffer.addToBuffer(fog);
	buffer.addToBuffer(lowestlandheight);
	buffer.addToBuffer(skytexture);
	buffer.addToBuffer(skycolormap);
	buffer.addToBuffer(skytimeofday);
	buffer.addToBuffer(skysunxy);
	buffer.addToBuffer(skysunyz);
	buffer.addToBuffer(skyambience);
	buffer.addToBuffer(skydiffuse);

	buffer.addToBuffer(bordertype);
	if (!border->writeMessage(buffer)) return false;
	
	buffer.addToBuffer(texturetype);
	if (!texture->writeMessage(buffer)) return false;
	
	buffer.addToBuffer(precipitationtype);
	if (!precipitation->writeMessage(buffer)) return false;

	int size = 0;
	size = (int) objects.size();
	buffer.addToBuffer(size);
	for (int i=0; i<size; i++)
	{
		buffer.addToBuffer(objectstype[i]);
		if (!objects[i]->writeMessage(buffer)) return false;
	}
	size = (int) events.size();
	buffer.addToBuffer(size);
	for (int i=0; i<size; i++)
	{
		if (!events[i]->writeMessage(buffer)) return false;
	}
	return true;
}

bool LandscapeTex::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name)) return false;
	if (!reader.getFromBuffer(detail)) return false;
	if (!reader.getFromBuffer(magmasmall)) return false;
	if (!reader.getFromBuffer(scorch)) return false;
	if (!reader.getFromBuffer(fog)) return false;
	if (!reader.getFromBuffer(lowestlandheight)) return false;
	if (!reader.getFromBuffer(skytexture)) return false;
	if (!reader.getFromBuffer(skycolormap)) return false;
	if (!reader.getFromBuffer(skytimeofday)) return false;
	if (!reader.getFromBuffer(skysunxy)) return false;
	if (!reader.getFromBuffer(skysunyz)) return false;
	if (!reader.getFromBuffer(skyambience)) return false;
	if (!reader.getFromBuffer(skydiffuse)) return false;

	if (!reader.getFromBuffer(bordertype)) return false;
	if (!(border = fetchBorderTexType(bordertype.c_str()))) return false;
	if (!border->readMessage(reader)) return false;

	if (!reader.getFromBuffer(texturetype)) return false;
	if (!(texture = fetchTextureTexType(texturetype.c_str()))) return false;
	if (!texture->readMessage(reader)) return false;
	
	if (!reader.getFromBuffer(precipitationtype)) return false;
	if (!(precipitation = fetchPrecipitationTexType(precipitationtype.c_str()))) return false;
	if (!precipitation->readMessage(reader)) return false;	

	int size = 0;
	if (!reader.getFromBuffer(size)) return false;
	for (int i=0; i<size; i++)
	{
		std::string placementtype;
		LandscapeTexType *placement = 0;
		if (!reader.getFromBuffer(placementtype)) return false;
		if (!(placement = fetchPlacementTexType(placementtype.c_str()))) return false;
		if (!placement->readMessage(reader)) return false;
		objects.push_back(placement);
		objectstype.push_back(placementtype);
	}
	if (!reader.getFromBuffer(size)) return false;
	for (int i=0; i<size; i++)
	{
		LandscapeTexEvent *event = new LandscapeTexEvent;
		if (!event->readMessage(reader)) return false;
		events.push_back(event);
	}
	return true;
}

bool LandscapeTex::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	if (!node->getNamedChild("detail", detail)) return false;
	if (!node->getNamedChild("magmasmall", magmasmall)) return false;
	if (!node->getNamedChild("scorch", scorch)) return false;
	if (!node->getNamedChild("fog", fog)) return false;
	if (!node->getNamedChild("lowestlandheight", lowestlandheight)) return false;
	if (!node->getNamedChild("skytexture", skytexture)) return false;
	if (!node->getNamedChild("skycolormap", skycolormap)) return false;
	if (!node->getNamedChild("skytimeofday", skytimeofday)) return false;
	if (!node->getNamedChild("skysunxy", skysunxy)) return false;
	if (!node->getNamedChild("skysunyz", skysunyz)) return false;
	if (!node->getNamedChild("skydiffuse", skydiffuse)) return false;
	if (!node->getNamedChild("skyambience", skyambience)) return false;

	if (!checkDataFile(detail.c_str())) return false;
	if (!checkDataFile(magmasmall.c_str())) return false;
	if (!checkDataFile(scorch.c_str())) return false;
	if (!checkDataFile(skytexture.c_str())) return false;
	if (!checkDataFile(skycolormap.c_str())) return false;

	{
		XMLNode *borderNode;
		if (!node->getNamedChild("border", borderNode)) return false;
		if (!borderNode->getNamedParameter("type", bordertype)) return false;
		if (!(border = fetchBorderTexType(bordertype.c_str()))) return false;
		if (!border->readXML(borderNode)) return false;
	}
	{
		XMLNode *textureNode;
		if (!node->getNamedChild("texture", textureNode)) return false;
		if (!textureNode->getNamedParameter("type", texturetype)) return false;
		if (!(texture = fetchTextureTexType(texturetype.c_str()))) return false;
		if (!texture->readXML(textureNode)) return false;
	}
	{
		XMLNode *precipitationNode;
		if (!node->getNamedChild("precipitation", precipitationNode)) return false;
		if (!precipitationNode->getNamedParameter("type", precipitationtype)) return false;
		if (!(precipitation = fetchPrecipitationTexType(precipitationtype.c_str()))) return false;
		if (!precipitation->readXML(precipitationNode)) return false;
	}
	{
		XMLNode *placementsNode, *placementNode;
		if (!node->getNamedChild("placements", placementsNode)) return false;
		while (placementsNode->getNamedChild("placement", placementNode, false))
		{
			std::string placementtype;
			LandscapeTexType *placement = 0;
			if (!placementNode->getNamedParameter("type", placementtype)) return false;
			if (!(placement = fetchPlacementTexType(placementtype.c_str()))) return false;
			if (!placement->readXML(placementNode)) return false;
			objects.push_back(placement);
			objectstype.push_back(placementtype);
		}
		if (!placementsNode->failChildren()) return false;
	}
	{
		XMLNode *eventsNode, *eventNode;
		if (!node->getNamedChild("events", eventsNode)) return false;
		while (eventsNode->getNamedChild("event", eventNode, false))
		{
			LandscapeTexEvent *event = new LandscapeTexEvent;
			if (!event->readXML(eventNode)) return false;
			events.push_back(event);
		}
		if (!eventsNode->failChildren()) return false;
	}
	return node->failChildren();
}

