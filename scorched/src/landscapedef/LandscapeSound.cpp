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

#include <landscapedef/LandscapeSound.h>
#include <landscape/LandscapeObjects.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Landscape.h>
#include <landscape/Water.h>
#include <common/Defines.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <sound/Sound.h>
#include <math.h>

bool LandscapeSoundPositionSet::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	if (!node->getNamedChild("maxsounds", maxsounds)) return false;
	
	return node->failChildren();
}

bool LandscapeSoundPositionSet::setPosition(VirtualSoundSource *source, void *data)
{
	LandscapeObjectsGroupEntry *groupEntry =
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().getGroup(
			name.c_str());
	if (!groupEntry) return false;
	if (groupEntry->getObjectCount() <= 0) return false;

	LandscapeObjectEntryBase *obj = (LandscapeObjectEntryBase*) data;
	if (!groupEntry->hasObject(obj)) return false;

	Vector position = obj->getPosition();
	source->setPosition(position);

	return true;
}

int LandscapeSoundPositionSet::getInitCount()
{
	LandscapeObjectsGroupEntry *groupEntry =
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().getGroup(
			name.c_str());
	if (!groupEntry) return 0;	

	return (MIN(maxsounds, groupEntry->getObjectCount()));
}

void *LandscapeSoundPositionSet::getInitData(int count)
{
	LandscapeObjectsGroupEntry *groupEntry =
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().getGroup(
			name.c_str());
	if (!groupEntry) return 0;	

	return groupEntry->getObject(count);
}

bool LandscapeSoundPositionGroup::readXML(XMLNode *node)
{
	if (!node->getNamedChild("falloff", falloff)) return false;
	if (!node->getNamedChild("name", name)) return false;
	return node->failChildren();
}

bool LandscapeSoundPositionGroup::setPosition(VirtualSoundSource *source, void *data)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();
	int groundMapWidth = ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapWidth();
	int groundMapHeight = ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapHeight();

	int a = int(cameraPos[0]);
	int b = int(cameraPos[1]);
	int x = MAX(0, MIN(a, groundMapWidth));
	int y = MAX(0, MIN(b, groundMapHeight));

	float distance = 255.0f;
	LandscapeObjectsGroupEntry *groupEntry =
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().getGroup(
			name.c_str());
	if (groupEntry)
	{
		if (groupEntry->getObjectCount() <= 0) return false;

		distance = groupEntry->getDistance(x, y);
	}
	distance += fabsf(float(a - x)) + fabsf(float(b - y));
	distance *= 4.0f * falloff;

	Vector position(0.0f, 0.0f, distance + cameraPos[2]);

	source->setRelative();
	source->setPosition(position);

	return true;
}

bool LandscapeSoundPositionWater::readXML(XMLNode *node)
{
	if (!node->getNamedChild("falloff", falloff)) return false;
	return node->failChildren();
}

bool LandscapeSoundPositionWater::setPosition(VirtualSoundSource *source, void *data)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	float distance = Landscape::instance()->getWater().
		getWaves().getWaveDistance(int(cameraPos[0]), int(cameraPos[1]));
	distance *= 4.0f * falloff;

	Vector position(0.0f, 0.0f, distance + cameraPos[2]);

	source->setRelative();
	source->setPosition(position);

	return true;
}

bool LandscapeSoundPositionAmbient::readXML(XMLNode *node)
{
	return node->failChildren();
}

bool LandscapeSoundPositionAmbient::setPosition(VirtualSoundSource *source, void *data)
{
	source->setRelative();
	source->setPosition(Vector::nullVector);

	return true;
}

bool LandscapeSoundPositionAbsoulte::readXML(XMLNode *node)
{
	if (!node->getNamedChild("position", position)) return false;
	return node->failChildren();
}

bool LandscapeSoundPositionAbsoulte::setPosition(VirtualSoundSource *source, void *data)
{
	source->setPosition(position);
	return true;
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

bool LandscapeSoundSoundFile::readXML(XMLNode *node)
{
	if (!node->getNamedChild("file", file)) return false;
	if (!checkDataFile(file.c_str())) return false;
	if (!node->getNamedChild("gain", gain)) return false;
	return node->failChildren();
}

bool LandscapeSoundSoundFile::play(VirtualSoundSource *source)
{
	SoundBuffer *buffer = 
		Sound::instance()->fetchOrCreateBuffer((char *)
			getDataFile(file.c_str()));
	if (!buffer) return false;

	source->setGain(gain);
	source->play(buffer);

	return true;
}

LandscapeSoundType::LandscapeSoundType() : 
	position(0), timing(0), sound(0)
{
}

LandscapeSoundType::~LandscapeSoundType()
{
	delete position;
	delete timing;
	delete sound;
}

bool LandscapeSoundType::readXML(XMLNode *node)
{
	{
		std::string soundtype;
		XMLNode *soundNode;
		if (!node->getNamedChild("sound", soundNode)) return false;
		if (!soundNode->getNamedParameter("type", soundtype)) return false;
		if (0 == strcmp(soundtype.c_str(), "file"))
			sound = new LandscapeSoundSoundFile;
		else return false;

		if (!sound->readXML(soundNode)) return false;
	}

	{
		std::string positiontype;
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
		else if (0 == strcmp(positiontype.c_str(), "set"))
			position = new LandscapeSoundPositionSet;
		else return false;
		if (!position->readXML(positionNode)) return false;
	}
	{
		std::string timingtype;
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

bool LandscapeSound::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
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
