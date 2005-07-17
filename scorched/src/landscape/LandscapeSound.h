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

#if !defined(__INCLUDE_LandscapeSoundh_INCLUDE__)
#define __INCLUDE_LandscapeSoundh_INCLUDE__

#include <XML/XMLFile.h>
#include <string>
#include <vector>

class LandscapeSoundTiming
{
public:
	virtual bool readXML(XMLNode *node) = 0;
	virtual float getNextEventTime() = 0;
};

class LandscapeSoundTimingLooped : public LandscapeSoundTiming
{
public:
	virtual bool readXML(XMLNode *node);
	virtual float getNextEventTime();
};

class LandscapeSoundTimingRepeat : public LandscapeSoundTiming
{
public:
	virtual bool readXML(XMLNode *node);
	virtual float getNextEventTime();
	float min, max;
};

class VirtualSoundSource;
class LandscapeSoundPosition
{
public:
	virtual bool readXML(XMLNode *node) = 0;
	virtual bool setPosition(VirtualSoundSource *source) = 0;
};

class LandscapeSoundPositionAmbient : public LandscapeSoundPosition
{
public:
	virtual bool readXML(XMLNode *node);
	virtual bool setPosition(VirtualSoundSource *source);
};

class LandscapeSoundPositionAbsoulte : public LandscapeSoundPosition
{
public:
	virtual bool readXML(XMLNode *node);
	virtual bool setPosition(VirtualSoundSource *source);
	Vector position;
};

class LandscapeSoundPositionWater : public LandscapeSoundPosition
{
public:
	virtual bool readXML(XMLNode *node);
	virtual bool setPosition(VirtualSoundSource *source);
	float falloff;
};

class LandscapeSoundPositionGroup : public LandscapeSoundPosition
{
public:
	virtual bool readXML(XMLNode *node);
	virtual bool setPosition(VirtualSoundSource *source);

	std::string name;
	float falloff;
};

class LandscapeSoundType
{
public:
	LandscapeSoundType();
	virtual ~LandscapeSoundType();

	virtual bool readXML(XMLNode *node);

	std::string sound;
	std::string positiontype;
	std::string timingtype;
	LandscapeSoundPosition *position;
	LandscapeSoundTiming *timing;
};

class LandscapeSound
{
public:
	LandscapeSound();
	virtual ~LandscapeSound();

	std::string name;
	std::vector<LandscapeSoundType *> objects;

	bool readXML(XMLNode *node);

private:
	LandscapeSound(const LandscapeSound &other);
	LandscapeSound &operator=(LandscapeSound &other);
};
#endif // __INCLUDE_LandscapeSoundh_INCLUDE__
