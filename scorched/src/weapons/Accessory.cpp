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

#include <common/ToolTip.h>
#include <common/ModelID.h>
#include <weapons/Accessory.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <stdlib.h>

unsigned int Accessory::nextAccessoryId_ = 0;

#ifndef S3D_SERVER
#include <3dsparse/Model.h>
#include <3dsparse/ModelStore.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <tankgraph/MissileMesh.h>
#include <GLEXT/GLBitmap.h>
#endif

Accessory::Accessory() :
	accessoryId_(++nextAccessoryId_),
	name_("NONAME"), description_("NODESC"), toolTip_("", ""),
	price_(0), bundle_(1), armsLevel_(9),
	modelScale_(1.0f),
	positionSelect_(ePositionSelectNone), positionSelectLimit_(10),
	maximumNumber_(0),
	startingNumber_(0),
	muzzleFlash_(true),
	aiOnly_(false),
	botOnly_(false)
{
#ifndef S3D_SERVER
	texture_ = 0;
#endif
}

Accessory::~Accessory()
{
}

bool Accessory::parseXML(OptionsGame &context,
	AccessoryStore *store, XMLNode *accessoryNode)
{
	// Get the accessory name
	if (!accessoryNode->getNamedChild("name", name_)) return false;

	// Get the accessory armslevel
	accessoryNode->getNamedChild("armslevel", armsLevel_, false);

	// Get the optional muzzleflash
	XMLNode *muzzleFlashNode = 0;
	accessoryNode->getNamedChild("nomuzzleflash", muzzleFlashNode, false);
	if (muzzleFlashNode) muzzleFlash_ = false;

	// Get the accessory description
	accessoryNode->getNamedChild("description", description_, false);
	toolTip_.setText(getName(), getDescription());

	// Get the accessory icon
	if (accessoryNode->getNamedChild("icon", iconName_, false))
	{
		if (!checkDataFile(formatString("data/textures/wicons/%s", getIconName()))) return false;
	}

	// Get the accessory sound 
	if (accessoryNode->getNamedChild("activationsound", activationSound_, false))
	{
		if (!checkDataFile(formatString("data/wav/%s", getActivationSound()))) return false;
	}

	// Get the accessory bundle
	accessoryNode->getNamedChild("bundlesize", bundle_, false);

	// Get ai only
	accessoryNode->getNamedChild("aionly", aiOnly_, false);

	// Get bot only (ie: only for bots, not for tank objects)
	accessoryNode->getNamedChild("botonly", botOnly_, false);

	// Get the maximum number
	maximumNumber_ = context.getMaxNumberWeapons();
	accessoryNode->getNamedChild("maximumnumber", maximumNumber_, false);

	// Get the starting number
	accessoryNode->getNamedChild("startingnumber", startingNumber_, false);

	// Get the accessory cost
	accessoryNode->getNamedChild("cost", price_, false);

	// Get the weapon model scale
	accessoryNode->getNamedChild("modelscale", modelScale_, false);

	// Get the weapon model
	XMLNode *modelNode = 0;
	if (accessoryNode->getNamedChild("model", modelNode, false))
	{
		if (!modelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	// Get action
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("accessoryaction", subNode)) return false;
	accessoryAction_ = store->createAccessoryPart(context, this, subNode);
	if (!accessoryAction_)
	{
		dialogMessage("Accessory", formatString(
			"Failed to create action \"%s\"", name_.c_str()));
		return false;
	}

	// Setup price
	sellPrice_ = 0;
	if (price_ > 0 && bundle_ > 0) sellPrice_ = int((price_ / bundle_) * 0.8f);
	originalPrice_ = price_;
	originalSellPrice_ = sellPrice_;

	// Position Selection Type
	std::string positionSelection;
	if (accessoryNode->getNamedChild("positionselection", positionSelection, false))
	{
		if (0 == strcmp(positionSelection.c_str(), "none"))
		{
			positionSelect_ = ePositionSelectNone;
		}
		else if (0 == strcmp(positionSelection.c_str(), "generic"))
		{
			positionSelect_ = ePositionSelectGeneric;
		}
		else if (0 == strcmp(positionSelection.c_str(), "fuel"))
		{
			positionSelect_ = ePositionSelectFuel;

			// Make sure there is a "WeaponMoveTank" under here somewhere
			if (!store->findAccessoryPartByAccessoryId(
				getAccessoryId(), "WeaponMoveTank"))
			{
				return accessoryNode->returnError(
					"Fuel selection can only be used with WeaponMoveTank weapons");
			}
		}
		else if (0 == strcmp(positionSelection.c_str(), "limit"))
		{
			positionSelect_ = ePositionSelectLimit;
			if (!accessoryNode->getNamedChild("positionselectionlimit", positionSelectLimit_)) return false;
		}
		else
		{
			return accessoryNode->returnError(formatString(
				"Unknown accessory position selection type \"%s\"", 
				positionSelection.c_str()));
		}
	}

	// Get the accessory groupname
	if (!accessoryNode->getNamedChild("group", groupName_, false))
	{
		if (accessoryAction_->getType() == AccessoryPart::AccessoryWeapon)
		{
			groupName_ = "weapon";
		}
		else
		{
			groupName_ = "defense";
		}
	}

	return true;
}

const char *Accessory::getActivationSound()
{
	if (!activationSound_.c_str()[0]) return 0;
	return activationSound_.c_str();
}

#ifndef S3D_SERVER

GLTexture *Accessory::getTexture()
{
	if (texture_) return texture_;

	GLTexture *texture = 0;
	if (getIconName()[0])
	{
		GLBitmap bmap(getDataFile(formatString("data/textures/wicons/%s", getIconName())), true);
		texture = new GLTexture();
		texture->create(bmap, GL_RGBA, false);
	}
	else
	{
		GLBitmap bmap(getDataFile(formatString("data/textures/wicons/%s", "tracer.bmp")), true);
		texture = new GLTexture();
		texture->create(bmap, GL_RGBA, false);
	}
	texture_ = texture;
	return texture;
}


std::map<std::string, MissileMesh *> Accessory::loadedMeshes_;

MissileMesh *Accessory::getWeaponMesh(ModelID &id, Tank *currentPlayer)
{
	// Set the default model to use if neither the tank
	// or weapon have one
	static ModelID defaultModelId;
	if (!defaultModelId.modelValid())
	{
		defaultModelId.initFromString(
			"MilkShape",
			"data/accessories/v2missile/v2missile.txt",
			"");
	}

	// Set the model to use as the default model id
	ModelID *usedModelId = &defaultModelId;

	// Get the model to use from the weapon (if there is one)
	if (id.modelValid())
	{
		usedModelId = &id;
	}
	else
	{
		// The weapon does not have a model defined for it
		// check the player to see if they have a default model
		if (currentPlayer)
		{
			TargetRendererImplTank *renderer = (TargetRendererImplTank *) 
				currentPlayer->getRenderer();
			if (renderer && 
				renderer->getModel()->getProjectileModelID().modelValid())
			{
				usedModelId = &renderer->getModel()->getProjectileModelID();
			}
		}
	}

	// Load or find the correct missile mesh
	MissileMesh *mesh = 0;
	const char *name = usedModelId->getStringHash();
	std::map<std::string, MissileMesh *>::iterator itor =
		loadedMeshes_.find(name);
	if (itor == loadedMeshes_.end())
	{
		mesh = new MissileMesh(*usedModelId);
		loadedMeshes_[name] = mesh;
	}
	else
	{
		// Find
		mesh = (*itor).second;
	}
	return mesh;
}
#endif // S3D_SERVER
