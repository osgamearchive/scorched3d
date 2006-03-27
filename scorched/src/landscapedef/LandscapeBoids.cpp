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

#include <landscapedef/LandscapeBoids.h>
#include <XML/XMLNode.h>
#include <common/Defines.h>

bool LandscapeBoidsType::readXML(XMLNode *node)
{
	XMLNode *modelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!model.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("count", count)) return false;
	if (!node->getNamedChild("minbounds", minbounds)) return false;
	if (!node->getNamedChild("maxbounds", maxbounds)) return false;
	if (!node->getNamedChild("maxvelocity", maxvelocity)) return false;
	if (!node->getNamedChild("cruisedistance", cruisedistance)) return false;
	if (!node->getNamedChild("maxacceleration", maxacceleration)) return false;
	modelsize = 1.0f; node->getNamedChild("modelsize", modelsize, false);

	XMLNode *soundsNode;
	std::string sound;
	if (!node->getNamedChild("sounds", soundsNode)) return false;
	if (!soundsNode->getNamedChild("mintime", soundmintime)) return false;
	if (!soundsNode->getNamedChild("maxtime", soundmaxtime)) return false;
	if (!soundsNode->getNamedChild("maxsimul", soundmaxsimul)) return false;
	if (!soundsNode->getNamedChild("volume", soundvolume)) return false;
	while (soundsNode->getNamedChild("sound", sound, false))
	{
		if (!checkDataFile(sound.c_str())) return false;
		sounds.push_back(sound);
	}
	if (!soundsNode->failChildren()) return false;

	return node->failChildren();
}

LandscapeBoids::LandscapeBoids()
{
}

LandscapeBoids::~LandscapeBoids()
{
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

bool LandscapeBoids::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *boidNode;
		while (node->getNamedChild("boid", boidNode, false))
		{
			LandscapeBoidsType *boid = new LandscapeBoidsType;
			if (!boid->readXML(boidNode)) return false;
			objects.push_back(boid);
		}
	}
	return node->failChildren();
}
