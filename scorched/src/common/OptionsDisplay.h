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


#if !defined(__INCLUDE_OptionsDisplayh_INCLUDE__)
#define __INCLUDE_OptionsDisplayh_INCLUDE__

#include <common/OptionEntry.h>

class OptionsDisplay
{
public:
	static OptionsDisplay *instance();

	enum DataType
	{
		NoAccess = 0,
		RWAccess = 1,
		RAccess = 2
	};

	int getMaxModelTriPercentage() { return maxModelTriPercentage_.getValue(); }
	void setMaxModelTriPercentage(int value) { maxModelTriPercentage_.setValue(value); }

	int getNoExplosionParts() { return explosionParts_.getValue(); }
	void setNoExplosionParts(int value) { explosionParts_.setValue(value); }

	int getExplosionParticlesMult() { return explosionParticleMult_.getValue(); }
	void setExplosionParticlesMult(int value) { explosionParticleMult_.setValue(value); }

	int getNoExplosionSubParts() { return explosionSubParts_.getValue(); }
	void setNoExplosionSubParts(int value) { explosionSubParts_.setValue(value); }

	int getWaterHeight() { return waterHeight_.getValue(); }
	void setWaterHeight(int value) { waterHeight_.setValue(value); }

	int getSunXYAng() { return sunXYAng_.getValue(); }
	void setSunXYAng(int value) { sunXYAng_.setValue(value); }

	int getSunYZAng() { return sunYZAng_.getValue(); }
	void setSunYZAng(int value) { sunYZAng_.setValue(value); }

	int getBrightness() { return brightness_.getValue(); }
	void setBrightness(int value) { brightness_.setValue(value); }

	bool getFrameTimer() { return frameTimer_.getValue(); }
	void setFrameTimer(bool value) { frameTimer_.setValue(value); }	

	bool getNoLenseFlare() { return noLenseFlare_.getValue(); }
	void setNoLenseFlare(bool value) { noLenseFlare_.setValue(value); }	

	bool getNoROAM() { return noROAM_.getValue(); }
	void setNoROAM(bool value) { noROAM_.setValue(value); }	

	bool getUseLandscapeTexture() { return useLandscapeTexture_.getValue(); }
	void setUseLandscapeTexture(bool value) { useLandscapeTexture_.setValue(value); }	

	bool getFullScreen() { return fullScreen_.getValue(); }
	void setFullScreen(bool value) { fullScreen_.setValue(value); }	

	bool getFullClear() { return fullClear_.getValue(); }
	void setFullClear(bool value) { fullClear_.setValue(value); }

	bool getInvertUpDownKey() { return invertUpDownKey_.getValue(); }
	void setInvertUpDownKey(bool value) { invertUpDownKey_.setValue(value); }

	bool getNoExt() { return noExt_.getValue(); }
	void setNoExt(bool value) { noExt_.setValue(value); }

	bool getNoSound() { return noSound_.getValue(); }
	void setNoSound(bool value) { noSound_.setValue(value); }

	bool getNoMultiTex() { return noMultiTex_.getValue(); }
	void setNoMultiTex(bool value) { noMultiTex_.setValue(value); }

	bool getNoSkins() { return noSkins_.getValue(); }
	void setNoSkins(bool value) { noSkins_.setValue(value); }

	bool getDrawLines() { return drawLines_.getValue(); }
	void setDrawLines(bool value) { drawLines_.setValue(value); }

	bool getDrawNormals() { return drawNormals_.getValue(); }
	void setDrawNormals(bool value) { drawNormals_.setValue(value); }

	bool getDrawPlayerNames() { return drawPlayerNames_.getValue(); }
	void setDrawPlayerNames(bool value) { drawPlayerNames_.setValue(value); }

	bool getDrawWater() { return drawWater_.getValue(); }
	void setDrawWater(bool value) { drawWater_.setValue(value); }

	bool getNoWaterMovement() { return noWaterMovement_.getValue(); }
	void setNoWaterMovement(bool value) { noWaterMovement_.setValue(value); }

	bool getNoSkyMovement() { return noSkyMovement_.getValue(); }
	void setNoSkyMovement(bool value) { noSkyMovement_.setValue(value); }

	bool getNoSkyLayers() { return noSkyLayers_.getValue(); }
	void setNoSkyLayers(bool value) { noSkyLayers_.setValue(value); }

	int getTexSize() { return texSize_.getValue(); }
	void setTexSize(int value) { texSize_.setValue(value); }

	int  getScreenWidth() { return screenWidth_.getValue(); }
	void setScreenWidth(int value) { screenWidth_.setValue(value); }

	int  getScreenHeight() { return screenHeight_.getValue(); }
	void setScreenHeight(int value) { screenHeight_.setValue(value); }

	const char *getUniqueUserId() { return uniqueUserId_.getValue(); }
	void setUniqueUserId(const char *value) { uniqueUserId_.setValue(value); }

	bool writeOptionsToFile(char *filePath);
	bool readOptionsFromFile(char *filePath);
	void addToConsole();

protected:
	static OptionsDisplay *instance_;
	std::list<OptionEntry *> options_;

	OptionEntryBoundedInt brightness_;
	OptionEntryBoundedInt explosionParts_;
	OptionEntryBoundedInt explosionSubParts_;
	OptionEntryBoundedInt maxModelTriPercentage_;
	OptionEntryBoundedInt explosionParticleMult_;
	OptionEntryInt waterHeight_;
	OptionEntryInt sunYZAng_;
	OptionEntryInt sunXYAng_;
	OptionEntryBool fullScreen_;
	OptionEntryBool fullClear_;
	OptionEntryBool noExt_;
	OptionEntryBool invertUpDownKey_;
	OptionEntryBool noSound_;
	OptionEntryBool drawNormals_;
	OptionEntryBool drawLines_;
	OptionEntryBool noLenseFlare_;
	OptionEntryBool noMultiTex_;
	OptionEntryBool noSkins_;
	OptionEntryBool drawWater_;
	OptionEntryBool drawPlayerNames_;
	OptionEntryBool noROAM_;
	OptionEntryBool useLandscapeTexture_;
	OptionEntryBool noSkyLayers_;
	OptionEntryBool noSkyMovement_;
	OptionEntryBool frameTimer_;
	OptionEntryBool noWaterMovement_;
	OptionEntryBoundedInt texSize_;
	OptionEntryInt screenWidth_;
	OptionEntryInt screenHeight_;
	OptionEntryString uniqueUserId_;

private:
	OptionsDisplay();
	virtual ~OptionsDisplay();

};


#endif
