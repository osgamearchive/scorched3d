////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <placement/PlacementObjectTarget.h>
#include <landscape/LandscapeObjectsEntryModel.h>
#include <landscape/LandscapeMaps.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
#include <XML/XMLParser.h>

PlacementObjectTarget::PlacementObjectTarget()
{
}

PlacementObjectTarget::~PlacementObjectTarget()
{
}

bool PlacementObjectTarget::readXML(XMLNode *node)
{
	if (!targetDef_.readXML(node, ".")) return false;
	if (!groups_.readXML(node)) return false;
	return PlacementObject::readXML(node);
}

void PlacementObjectTarget::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	PlacementType::Position &position)
{
	++playerId;
	Target *target = targetDef_.createTarget(
		playerId, position.position, context, generator);
	context.targetContainer->addTarget(target);

	context.landscapeMaps->getGroundMaps().getObjects().getShadows().push_back(
		PlacementShadowDefinition::Entry(
		&targetDef_.getShadow(),
		position.position,
		targetDef_.getSize()));

	if (target->getRenderer())
	{
		groups_.addToGroups(context, 
			(LandscapeObjectsEntry *) target->getRenderer(), false);
	}
}
