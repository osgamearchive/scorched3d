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
	unsigned int landSeed; // Seed to generate landscape
	float landHillsMin, landHillsMax; // Number of hill in landscape
	float landWidthX, landWidthY; // The size of the landscape
	float landHeightMin, landHeightMax; // The max height of the landscape
	float landPeakWidthXMin, landPeakWidthXMax; // Width of hill
	float landPeakWidthYMin, landPeakWidthYMax; // Width of hill
	float landPeakHeightMin, landPeakHeightMax; // Height of hill
	float landSmoothing; // Amount of landscape smoothing
	float tankStartCloseness; // Min distance between tanks 
	float tankStartHeightMin, tankStartHeightMax; // Min/max height of tanks start
	std::string name;  // Name of this landscape definition type
	std::string resourceFile; // The resource to use for the landscape textures
	std::string heightMapFile; // A possible height map bitmap if not auto gened
	std::string heightMaskFile; // A possible height map mask if needed

	// stuff NOT sent over coms
	std::vector<std::string> resourceFiles; // This list of possible resources for this defn
	float weight; // The posibility this defn will be choosen
	std::string description;  // Description of this landscape definition type
	std::string picture; // Visible view of this landscape definition type

	// Fns
	void generate();
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);
	bool readXML(XMLNode *node);
	bool readHeightMap(XMLNode *node, const char *name);
	bool readTextureMap(XMLNode *node, const char *name);
	bool readXMLMinMax(XMLNode *node, 
		const char *name,
		float &min, float &max);

protected:
	//LandscapeDefinition(const LandscapeDefinition &other);
	//LandscapeDefinition &operator=(LandscapeDefinition &other);
};

class OptionsGame;
class LandscapeDefinitions
{
public:
	static LandscapeDefinitions *instance();

	bool readLandscapeDefinitions();
	void clearLandscapeDefinitions();

	LandscapeDefinition &getRandomLandscapeDefn(OptionsGame &context);

	bool landscapeEnabled(OptionsGame &context, const char *name);
	std::vector<LandscapeDefinition> &getAllLandscapes() 
		{ return definitions_; }

protected:
	static LandscapeDefinitions *instance_;
	std::vector<LandscapeDefinition> definitions_;

private:
	LandscapeDefinitions();
	virtual ~LandscapeDefinitions();
};

#endif
