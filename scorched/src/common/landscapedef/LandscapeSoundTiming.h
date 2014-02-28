////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_LandscapeSoundTimingh_INCLUDE__)
#define __INCLUDE_LandscapeSoundTimingh_INCLUDE__

#include <XML/XMLEntrySimpleTypes.h>

class LandscapeSoundTiming : public XMLEntryContainer
{
public:
	LandscapeSoundTiming(const char *typeName, const char *description);
	virtual ~LandscapeSoundTiming();

	virtual float getNextEventTime() = 0;
};

class LandscapeSoundTimingChoice : public XMLEntryTypeChoice<LandscapeSoundTiming>
{
public:
	LandscapeSoundTimingChoice();
	virtual ~LandscapeSoundTimingChoice();

	virtual LandscapeSoundTiming *createXMLEntry(const std::string &type);
	virtual void getAllTypes(std::set<std::string> &allTypes);
};

class LandscapeSoundTimingLooped : public LandscapeSoundTiming
{
public:
	LandscapeSoundTimingLooped();
	virtual ~LandscapeSoundTimingLooped();

	virtual float getNextEventTime();
};

class LandscapeSoundTimingRepeat : public LandscapeSoundTiming
{
public:
	LandscapeSoundTimingRepeat();
	virtual ~LandscapeSoundTimingRepeat();

	virtual float getNextEventTime();
protected:
	XMLEntryFixed min, max;
};

#endif // __INCLUDE_LandscapeSoundTimingh_INCLUDE__
