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

	int getNumberExplosionParts() { return explosionParts_.getValue(); }
	void setNumberExplosionParts(int value) { explosionParts_.setValue(value); }

	int getExplosionParticlesMult() { return explosionParticleMult_.getValue(); }
	void setExplosionParticlesMult(int value) { explosionParticleMult_.setValue(value); }

	int getNumberExplosionSubParts() { return explosionSubParts_.getValue(); }
	void setNumberExplosionSubParts(int value) { explosionSubParts_.setValue(value); }

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

	bool getDetailTexture() { return detailTexture_.getValue(); }
	void setDetailTexture(bool value) { detailTexture_.setValue(value); }	

	bool getUseLandscapeTexture() { return useLandscapeTexture_.getValue(); }
	void setUseLandscapeTexture(bool value) { useLandscapeTexture_.setValue(value); }	

	bool getFullScreen() { return fullScreen_.getValue(); }
	void setFullScreen(bool value) { fullScreen_.setValue(value); }	

	bool getFullClear() { return fullClear_.getValue(); }
	void setFullClear(bool value) { fullClear_.setValue(value); }

	bool getInvertUpDownKey() { return invertUpDownKey_.getValue(); }
	void setInvertUpDownKey(bool value) { invertUpDownKey_.setValue(value); }

	bool getNoGLExt() { return noGLExt_.getValue(); }
	void setNoGLExt(bool value) { noGLExt_.setValue(value); }

	bool getNoGLTexSubImage() { return noGLTexSubImage_.getValue(); }
	void setNoGLTexSubImage(bool value) { noGLTexSubImage_.setValue(value); }

	bool getNoGLMultiTex() { return noGLMultiTex_.getValue(); }
	void setNoGLMultiTex(bool value) { noGLMultiTex_.setValue(value); }

	bool getNoGLCompiledArrays() { return noGLCompiledArrays_.getValue(); }
	void setNoGLCompiledArrays(bool value) { noGLCompiledArrays_.setValue(value); }
	
	bool getNoGLEnvCombine() { return noGLEnvCombine_.getValue(); }
	void setNoGLEnvCombine(bool value) { noGLEnvCombine_.setValue(value); }

	bool getNoGLCubeMap() { return noGLCubeMap_.getValue(); }
	void setNoGLCubeMap(bool value) { noGLCubeMap_.setValue(value); }

	bool getNoGLHardwareMipmaps() { return noGLHardwareMipmaps_.getValue(); }
	void setNoGLHardwareMipmaps(bool value) { noGLHardwareMipmaps_.setValue(value); }

	bool getNoSound() { return noSound_.getValue(); }
	void setNoSound(bool value) { noSound_.setValue(value); }

	bool getNoSkins() { return noSkins_.getValue(); }
	void setNoSkins(bool value) { noSkins_.setValue(value); }

	int getTankDetail() { return tankDetail_.getValue(); }
	void setTankDetail(int value) { tankDetail_.setValue(value); }

	int getEffectsDetail() { return effectsDetail_.getValue(); }
	void setEffectsDetail(int value) { effectsDetail_.setValue(value); }

	bool getUseHexidecimal() { return useHex_.getValue(); }
	void setUseHexidecimal(bool value) { useHex_.setValue(value); }

	bool getDrawLines() { return drawLines_.getValue(); }
	void setDrawLines(bool value) { drawLines_.setValue(value); }

	bool getDrawNormals() { return drawNormals_.getValue(); }
	void setDrawNormals(bool value) { drawNormals_.setValue(value); }

	bool getDrawPlayerNames() { return drawPlayerNames_.getValue(); }
	void setDrawPlayerNames(bool value) { drawPlayerNames_.setValue(value); }

	bool getDrawPlayerSight() { return drawPlayerSight_.getValue(); }
	void setDrawPlayerSight(bool value) { drawPlayerSight_.setValue(value); }

	bool getDrawPlayerHealth() { return drawPlayerHealth_.getValue(); }
	void setDrawPlayerHealth(bool value) { drawPlayerHealth_.setValue(value); }

	bool getDrawPlayerColor() { return drawPlayerColor_.getValue(); }
	void setDrawPlayerColor(bool value) { drawPlayerColor_.setValue(value); }

	bool getDrawWater() { return drawWater_.getValue(); }
	void setDrawWater(bool value) { drawWater_.setValue(value); }

	bool getNoWaterMovement() { return noWaterMovement_.getValue(); }
	void setNoWaterMovement(bool value) { noWaterMovement_.setValue(value); }

	bool getNoSkyMovement() { return noSkyMovement_.getValue(); }
	void setNoSkyMovement(bool value) { noSkyMovement_.setValue(value); }

	bool getNoSkyLayers() { return noSkyLayers_.getValue(); }
	void setNoSkyLayers(bool value) { noSkyLayers_.setValue(value); }

	bool getShowContextHelp() { return showContextHelp_.getValue(); }
	void setShowContextHelp(bool value) { showContextHelp_.setValue(value); }

	int getBannerRows() { return bannerRows_.getValue(); }
	void setBannerRows(int value) { bannerRows_.setValue(value); }

	int getTexSize() { return texSize_.getValue(); }
	void setTexSize(int value) { texSize_.setValue(value); }

	int  getScreenWidth() { return screenWidth_.getValue(); }
	void setScreenWidth(int value) { screenWidth_.setValue(value); }

	int  getScreenHeight() { return screenHeight_.getValue(); }
	void setScreenHeight(int value) { screenHeight_.setValue(value); }

	int getDepthBufferBits() { return depthBufferBits_.getValue(); }
	void setDepthBufferBits(int value) { depthBufferBits_.setValue(value); }

	int getColorComponentSize() { return colorComponentSize_.getValue(); }
	void setColorComponentSize(int value) { colorComponentSize_.setValue(value); }

	int getBitsPerPixel() { return bitsPerPixel_.getValue(); }
	void setBitsPerPixel(int value) { bitsPerPixel_.setValue(value); }

	bool getDoubleBuffer() { return doubleBuffer_.getValue(); }
	void setDoubleBuffer(bool value) { doubleBuffer_.setValue(value); }

	const char *getUniqueUserId() { return uniqueUserId_.getValue(); }
	void setUniqueUserId(const char *value) { uniqueUserId_.setValue(value); }

	const char *getOnlineUserName() { return onlineUserName_.getValue(); }
	void setOnlineUserName(const char *value) { onlineUserName_.setValue(value); }

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
	OptionEntryInt colorComponentSize_;
	OptionEntryInt bitsPerPixel_;
	OptionEntryInt depthBufferBits_;
	OptionEntryInt bannerRows_;
	OptionEntryBool detailTexture_;
	OptionEntryBool useHex_;
	OptionEntryBool doubleBuffer_;
	OptionEntryBool fullScreen_;
	OptionEntryBool fullClear_;
	OptionEntryBool noGLTexSubImage_;
	OptionEntryBool noGLExt_;
	OptionEntryBool noGLMultiTex_;
	OptionEntryBool noGLCompiledArrays_;
	OptionEntryBool noGLEnvCombine_;
	OptionEntryBool noGLCubeMap_;
	OptionEntryBool noGLHardwareMipmaps_;
	OptionEntryBool invertUpDownKey_;
	OptionEntryBool noSound_;
	OptionEntryBool drawNormals_;
	OptionEntryBool drawLines_;
	OptionEntryBool noLenseFlare_;
	OptionEntryBool noSkins_;
	OptionEntryBool drawWater_;
	OptionEntryBool drawPlayerNames_;
	OptionEntryBool drawPlayerSight_;
	OptionEntryBool drawPlayerColor_;
	OptionEntryBool drawPlayerHealth_;
	OptionEntryBool noROAM_;
	OptionEntryBool useLandscapeTexture_;
	OptionEntryBool noSkyLayers_;
	OptionEntryBool noSkyMovement_;
	OptionEntryBool frameTimer_;
	OptionEntryBool noWaterMovement_;
	OptionEntryBool showContextHelp_;
	OptionEntryBoundedInt texSize_;
	OptionEntryBoundedInt tankDetail_;
	OptionEntryBoundedInt effectsDetail_;
	OptionEntryInt screenWidth_;
	OptionEntryInt screenHeight_;
	OptionEntryString uniqueUserId_;
	OptionEntryString onlineUserName_;

private:
	OptionsDisplay();
	virtual ~OptionsDisplay();

};


#endif
