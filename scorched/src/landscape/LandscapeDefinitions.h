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

#if !defined(__INCLUDE_LandscapeDefinitionsh_INCLUDE__)
#define __INCLUDE_LandscapeDefinitionsh_INCLUDE__

#include <coms/ComsMessage.h>
#include <XML/XMLFile.h>
#include <string>
#include <list>
#include <vector>

class LandscapeDefinition
{
public:
	LandscapeDefinition() {}

	// Stuff sent over COMS
	unsigned int landSeed;
	float landHillsMin, landHillsMax;
	float landWidthX, landWidthY;
	float landHeightMin, landHeightMax;
	float landPeakWidthXMin, landPeakWidthXMax;
	float landPeakWidthYMin, landPeakWidthYMax;
	float landPeakHeightMin, landPeakHeightMax;
	float landSmoothing;
	float tankStartCloseness;
	float tankStartHeightMin, tankStartHeightMax;
	std::string name;
	std::string resourceFile;
	std::string heightMapFile;
	std::string textureMapFile;

	// stuff NOT sent over coms
	std::vector<std::string> resourceFiles;
	float weight;

	// Fns
	void generate();
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);
	bool readXML(XMLNode *node);
	bool readXMLMinMax(XMLNode *node, 
		const char *name,
		float &min, float &max);

protected:
	//LandscapeDefinition(const LandscapeDefinition &other);
	//LandscapeDefinition &operator=(LandscapeDefinition &other);
};

class LandscapeDefinitions
{
public:
	static LandscapeDefinitions *instance();

	bool readLandscapeDefinitions();

	LandscapeDefinition &getLandscapeDefn();

protected:
	static LandscapeDefinitions *instance_;

	float totalWeight_;
	std::vector<LandscapeDefinition> definitions_;

private:
	LandscapeDefinitions();
	virtual ~LandscapeDefinitions();
};

#endif
