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

#if !defined(__INCLUDE_LandscapeBoidsh_INCLUDE__)
#define __INCLUDE_LandscapeBoidsh_INCLUDE__

#include <list>
#include <vector>
#include <common/Vector.h>
#include <common/ModelID.h>

class LandscapeDefinitions;
class LandscapeBoidsType
{
public:
	ModelID model;
	int count;
	Vector minbounds, maxbounds;
	float soundmintime, soundmaxtime;
	float soundvolume;
	float modelsize;
	float maxvelocity;
	float cruisedistance;
	float maxacceleration;
	int soundmaxsimul;
	std::list<std::string> sounds;

	bool readXML(XMLNode *node);
};

class LandscapeBoids
{
public:
	LandscapeBoids();
	virtual ~LandscapeBoids();

	std::vector<LandscapeBoidsType *> objects;

	bool readXML(LandscapeDefinitions *definitions, XMLNode *node);

private:
	LandscapeBoids(const LandscapeBoids &other);
	LandscapeBoids &operator=(LandscapeBoids &other);
};


#endif // __INCLUDE_LandscapeBoidsh_INCLUDE__
