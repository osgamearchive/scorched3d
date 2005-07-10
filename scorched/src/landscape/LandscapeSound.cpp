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

#include <landscape/LandscapeSound.h>
#include <landscape/LandscapeObjects.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Landscape.h>
#include <common/Defines.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <sound/Sound.h>
#include <math.h>

bool LandscapeSoundPositionGroup::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	return node->failChildren();
}

void LandscapeSoundPositionGroup::setPosition(VirtualSoundSource *source)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	int a = int(cameraPos[0]);
	int b = int(cameraPos[1]);
	int x = MAX(0, MIN(a, 255));
	int y = MAX(0, MIN(b, 255));

	float distance = 255.0f;
	LandscapeObjects::LandscapeObjectsGroupEntry *groupEntry =
		ScorchedClient::instance()->getLandscapeMaps().getObjects().getGroup(
			name.c_str());
	if (groupEntry)
	{
		distance = groupEntry->getDistance(x, y);
	}
	distance += fabsf(float(a - x)) + fabsf(float(b - y));
	distance *= 4.0f;

	Vector position(0.0f, 0.0f, distance + cameraPos[2]);

	source->setRelative();
	source->setPosition(position);
}

bool LandscapeSoundPositionWater::readXML(XMLNode *node)
{
	return node->failChildren();
}

void LandscapeSoundPositionWater::setPosition(VirtualSoundSource *source)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	int a = int(cameraPos[0]) / 4;
	int b = int(cameraPos[1]) / 4;
	int x = MAX(0, MIN(a, 63));
	int y = MAX(0, MIN(b, 63));

	float distance = Landscape::instance()->getWater().
		getWaves().getWaveDistance()[x + y * 64];
	distance += fabsf(float(a - x)) + fabsf(float(b - y));
	distance *= 4.0f;

	Vector position(0.0f, 0.0f, distance + cameraPos[2]);

	source->setRelative();
	source->setPosition(position);
}

bool LandscapeSoundPositionAmbient::readXML(XMLNode *node)
{
	return node->failChildren();
}

void LandscapeSoundPositionAmbient::setPosition(VirtualSoundSource *source)
{
	source->setRelative();
	source->setPosition(Vector::nullVector);
}

bool LandscapeSoundPositionAbsoulte::readXML(XMLNode *node)
{
	if (!node->getNamedChild("position", position)) return false;
	return node->failChildren();
}

void LandscapeSoundPositionAbsoulte::setPosition(VirtualSoundSource *source)
{
	source->setPosition(position);
}

bool LandscapeSoundTimingLooped::readXML(XMLNode *node)
{
	return node->failChildren();
}

float LandscapeSoundTimingLooped::getNextEventTime()
{
	return -1.0f;
}

bool LandscapeSoundTimingRepeat::readXML(XMLNode *node)
{
	if (!node->getNamedChild("min", min)) return false;
	if (!node->getNamedChild("max", max)) return false;
	return node->failChildren();
}

float LandscapeSoundTimingRepeat::getNextEventTime()
{
	return min + max * RAND;
}

LandscapeSoundType::LandscapeSoundType() : 
	position(0), timing(0)
{
}

LandscapeSoundType::~LandscapeSoundType()
{
	delete position;
	delete timing;
}

bool LandscapeSoundType::readXML(XMLNode *node)
{
	if (!node->getNamedChild("sound", sound)) return false;
	{
		XMLNode *positionNode;
		if (!node->getNamedChild("position", positionNode)) return false;
		if (!positionNode->getNamedParameter("type", positiontype)) return false;
		if (0 == strcmp(positiontype.c_str(), "ambient"))
			position = new LandscapeSoundPositionAmbient;
		else if (0 == strcmp(positiontype.c_str(), "absolute"))
			position = new LandscapeSoundPositionAbsoulte;
		else if (0 == strcmp(positiontype.c_str(), "water"))
			position = new LandscapeSoundPositionWater;
		else if (0 == strcmp(positiontype.c_str(), "group"))
			position = new LandscapeSoundPositionGroup;
		else return false;
		if (!position->readXML(positionNode)) return false;
	}
	{
		XMLNode *timingNode;
		if (!node->getNamedChild("timing", timingNode)) return false;
		if (!timingNode->getNamedParameter("type", timingtype)) return false;
		if (0 == strcmp(timingtype.c_str(), "looped"))
			timing = new LandscapeSoundTimingLooped;
		else if (0 == strcmp(timingtype.c_str(), "repeat"))
			timing = new LandscapeSoundTimingRepeat;
		else return false;
		if (!timing->readXML(timingNode)) return false;
	}
	return node->failChildren();
}

LandscapeSound::LandscapeSound()
{
}

LandscapeSound::~LandscapeSound()
{
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

bool LandscapeSound::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	{
		XMLNode *soundsNode, *soundNode;
		if (!node->getNamedChild("sounds", soundsNode)) return false;
		while (soundsNode->getNamedChild("sound", soundNode, false))
		{
			LandscapeSoundType *sound = new LandscapeSoundType;
			if (!sound->readXML(soundNode)) return false;
			objects.push_back(sound);
		}
		if (!soundsNode->failChildren()) return false;
	}
	return node->failChildren();
}
