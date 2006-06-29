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

#include <placement/PlacementShadowDefinition.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/DeformLandscape.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <3dsparse/ImageStore.h>
#include <GLEXT/GLBitmapModifier.h>

PlacementShadowDefinition::PlacementShadowDefinition() :
	drawShadow_(true), flattenArea_(0.0f)
{
}

PlacementShadowDefinition::~PlacementShadowDefinition()
{
}

bool PlacementShadowDefinition::readXML(XMLNode *node, const char *base)
{
	node->getNamedChild("drawshadow", drawShadow_, false);
	node->getNamedChild("flattenarea", flattenArea_, false);

	XMLNode *groundMap = 0;
	if (node->getNamedChild("groundmap", groundMap, false))
	{
		if (!groundMap_.initFromNode(base, groundMap)) return false;
	}

	return true;
}

void PlacementShadowDefinition::updateLandscapeHeight(
	ScorchedContext &context,
	Vector &position, Vector &size)
{
	if (flattenArea_ != 0.0f)
	{
		float areaSize = flattenArea_;
		if (areaSize < 0.0f)
		{
			areaSize = MAX(size[0], size[1])/2.0f + 1.0f;
		}

		DeformLandscape::flattenArea(context, position, 0, false, areaSize);
	}
}

void PlacementShadowDefinition::updateLandscapeTexture(
	ScorchedContext &context,
	Vector &position, Vector &size)
{
	float shadowMultWidth = (float) Landscape::instance()->getMainMap().getWidth() / 
		context.landscapeMaps->getGroundMaps().getMapWidth();
	float shadowMultHeight = (float) Landscape::instance()->getMainMap().getHeight() / 
		context.landscapeMaps->getGroundMaps().getMapHeight();

	if (groundMap_.imageValid())
	{
		GLImage *image = ImageStore::instance()->loadImage(groundMap_);
		GLBitmapModifier::addBitmap(
			Landscape::instance()->getMainMap(),
			*image,
			position[0] * shadowMultWidth, 
			position[1] * shadowMultHeight,
			shadowMultWidth / 4.0f,
			shadowMultHeight / 4.0f);
	}

	if (drawShadow_)
	{
		GLBitmapModifier::addCircle(
			Landscape::instance()->getMainMap(),
			position[0] * shadowMultWidth, 
			position[1] * shadowMultHeight, 
			MAX(size[0], size[1]) * shadowMultWidth, 1.0f);
	}
}
