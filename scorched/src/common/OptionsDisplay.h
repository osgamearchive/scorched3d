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
		RAccess = 2,
		NoRestore = 4
	};

	int getMaxModelTriPercentage() { return maxModelTriPercentage_.getValue(); }
	void setMaxModelTriPercentage(int value) { maxModelTriPercentage_.setValue(value); }
	const char *getMaxModelTriPercentageToolTip() { return maxModelTriPercentage_.getDescription(); }

	int getTankModelSize() { return tankModelSize_.getValue(); }
	void setTankModelSize(int value) { tankModelSize_.setValue(value); }
	const char *getTankModelSizeToolTip() { return tankModelSize_.getDescription(); }

	int getNumberExplosionParts() { return explosionParts_.getValue(); }
	void setNumberExplosionParts(int value) { explosionParts_.setValue(value); }
	const char *getNumberExplosionPartsToolTip() { return explosionParts_.getDescription(); }

	int getExplosionParticlesMult() { return explosionParticleMult_.getValue(); }
	void setExplosionParticlesMult(int value) { explosionParticleMult_.setValue(value); }
	const char *getExplosionParticlesMultToolTip() { return explosionParticleMult_.getDescription(); }

	int getNumberExplosionSubParts() { return explosionSubParts_.getValue(); }
	void setNumberExplosionSubParts(int value) { explosionSubParts_.setValue(value); }
	const char *getNumberExplosionSubPartsToolTip() { return explosionSubParts_.getDescription(); }

	int getBrightness() { return brightness_.getValue(); }
	void setBrightness(int value) { brightness_.setValue(value); }
	const char *getBrightnessToolTip() { return brightness_.getDescription(); }

	bool getFrameTimer() { return frameTimer_.getValue(); }
	void setFrameTimer(bool value) { frameTimer_.setValue(value); }	
	const char *getFrameTimerToolTip() { return frameTimer_.getDescription(); }

	bool getNoLenseFlare() { return noLenseFlare_.getValue(); }
	void setNoLenseFlare(bool value) { noLenseFlare_.setValue(value); }	
	const char *getNoLenseFlareToolTip() { return noLenseFlare_.getDescription(); }

	bool getNoROAM() { return noROAM_.getValue(); }
	void setNoROAM(bool value) { noROAM_.setValue(value); }	
	const char *getNoROAMToolTip() { return noROAM_.getDescription(); }

	bool getNoTessalation() { return noTessalation_.getValue(); }
	void setNoTessalation(bool value) { noTessalation_.setValue(value); }	
	const char *getNoTessalationToolTip() { return noTessalation_.getDescription(); }

	bool getNoVBO() { return noVBO_.getValue(); }
	void setNoVBO(bool value) { noVBO_.setValue(value); }	
	const char *getNoVBOToolTip() { return noVBO_.getDescription(); }

	bool getDetailTexture() { return detailTexture_.getValue(); }
	void setDetailTexture(bool value) { detailTexture_.setValue(value); }	
	const char *getDetailTextureToolTip() { return detailTexture_.getDescription(); }

	bool getUseLandscapeTexture() { return useLandscapeTexture_.getValue(); }
	void setUseLandscapeTexture(bool value) { useLandscapeTexture_.setValue(value); }	
	const char *getUseLandscapeTextureToolTip() { return useLandscapeTexture_.getDescription(); }

	bool getFullScreen() { return fullScreen_.getValue(); }
	void setFullScreen(bool value) { fullScreen_.setValue(value); }	
	const char *getFullScreenToolTip() { return fullScreen_.getDescription(); }

	bool getMoreRes() { return moreRes_.getValue(); }
	void setMoreRes(bool value) { moreRes_.setValue(value); }	
	const char *getMoreResToolTip() { return moreRes_.getDescription(); }

	bool getFullClear() { return fullClear_.getValue(); }
	void setFullClear(bool value) { fullClear_.setValue(value); }
	const char *getFullClearToolTip() { return fullClear_.getDescription(); }

	bool getInvertElevation() { return invertElevation_.getValue(); }
	void setInvertElevation(bool value) { invertElevation_.setValue(value); }
	const char *getInvertElevationToolTip() { return invertElevation_.getDescription(); }

	bool getInvertMouse() { return invertMouse_.getValue(); }
	void setInvertMouse(bool value) { invertMouse_.setValue(value); }
	const char *getInvertMouseToolTip() { return invertMouse_.getDescription(); }

	bool getSwapYAxis() { return swapYAxis_.getValue(); }
	void setSwapYAxis(bool value) { swapYAxis_.setValue(value); }
	const char *getSwapYAxisToolTip() { return swapYAxis_.getDescription(); }

	bool getSideScroll() { return sideScroll_.getValue(); }
	void setSideScroll(bool value) { sideScroll_.setValue(value); }
	const char *getSideScrollToolTip() { return sideScroll_.getDescription(); }

	bool getNoGLExt() { return noGLExt_.getValue(); }
	void setNoGLExt(bool value) { noGLExt_.setValue(value); }
	const char *getNoGLExtToolTip() { return noGLExt_.getDescription(); }

	bool getNoGLTexSubImage() { return noGLTexSubImage_.getValue(); }
	void setNoGLTexSubImage(bool value) { noGLTexSubImage_.setValue(value); }
	const char *getNoGLTexSubImageToolTip() { return noGLTexSubImage_.getDescription(); }

	bool getNoGLMultiTex() { return noGLMultiTex_.getValue(); }
	void setNoGLMultiTex(bool value) { noGLMultiTex_.setValue(value); }
	const char *getNoGLMultiTexToolTip() { return noGLMultiTex_.getDescription(); }

	bool getNoGLCompiledArrays() { return noGLCompiledArrays_.getValue(); }
	void setNoGLCompiledArrays(bool value) { noGLCompiledArrays_.setValue(value); }
	const char *getNoGLCompiledArraysToolTip() { return noGLCompiledArrays_.getDescription(); }
	
	bool getNoGLEnvCombine() { return noGLEnvCombine_.getValue(); }
	void setNoGLEnvCombine(bool value) { noGLEnvCombine_.setValue(value); }
	const char *getNoGLEnvCombineToolTip() { return noGLEnvCombine_.getDescription(); }

	bool getNoGLCubeMap() { return noGLCubeMap_.getValue(); }
	void setNoGLCubeMap(bool value) { noGLCubeMap_.setValue(value); }
	const char *getNoGLCubeMapToolTip() { return noGLCubeMap_.getDescription(); }

	bool getNoGLSphereMap() { return noGLSphereMap_.getValue(); }
	void setNoGLSphereMap(bool value) { noGLSphereMap_.setValue(value); }
	const char *getNoGLSphereMapToolTip() { return noGLSphereMap_.getDescription(); }

	bool getNoGLHardwareMipmaps() { return noGLHardwareMipmaps_.getValue(); }
	void setNoGLHardwareMipmaps(bool value) { noGLHardwareMipmaps_.setValue(value); }
	const char *getNoGLHardwareMipmapsToolTip() { return noGLHardwareMipmaps_.getDescription(); }

	int getSoundChannels() { return soundChannels_.getValue(); }
	void setSoundChannels(int value) { soundChannels_.setValue(value); }
	const char *getSoundChannelsToolTip() { return soundChannels_.getDescription(); }

	bool getNoSound() { return noSound_.getValue(); }
	void setNoSound(bool value) { noSound_.setValue(value); }
	const char *getNoSoundToolTip() { return noSound_.getDescription(); }

	bool getNoSkins() { return noSkins_.getValue(); }
	void setNoSkins(bool value) { noSkins_.setValue(value); }
	const char *getNoSkinsToolTip() { return noSkins_.getDescription(); }

	int getTankDetail() { return tankDetail_.getValue(); }
	void setTankDetail(int value) { tankDetail_.setValue(value); }
	const char *getTankDetailToolTip() { return tankDetail_.getDescription(); }

	int getEffectsDetail() { return effectsDetail_.getValue(); }
	void setEffectsDetail(int value) { effectsDetail_.setValue(value); }
	const char *getEffectsDetailToolTip() { return effectsDetail_.getDescription(); }

	bool getUseHexidecimal() { return useHex_.getValue(); }
	void setUseHexidecimal(bool value) { useHex_.setValue(value); }
	const char *getUseHexidecimalToolTip() { return useHex_.getDescription(); }

	bool getDrawLines() { return drawLines_.getValue(); }
	void setDrawLines(bool value) { drawLines_.setValue(value); }
	const char *getDrawLinesToolTip() { return drawLines_.getDescription(); }

	bool getDrawNormals() { return drawNormals_.getValue(); }
	void setDrawNormals(bool value) { drawNormals_.setValue(value); }
	const char *getDrawNormalsToolTip() { return drawNormals_.getDescription(); }

	bool getDrawBoundingSpheres() { return drawBoundingSpheres_.getValue(); }
	void setDrawBoundingSpheres(bool value) { drawBoundingSpheres_.setValue(value); }
	const char *getDrawBoundingSpheresToolTip() { return drawBoundingSpheres_.getDescription(); }

	bool getDrawPlayerNames() { return drawPlayerNames_.getValue(); }
	void setDrawPlayerNames(bool value) { drawPlayerNames_.setValue(value); }
	const char *getDrawPlayerNamesToolTip() { return drawPlayerNames_.getDescription(); }

	bool getDrawPlayerIcons() { return drawPlayerIcons_.getValue(); }
	void setDrawPlayerIcons(bool value) { drawPlayerIcons_.setValue(value); }
	const char *getDrawPlayerIconsToolTip() { return drawPlayerIcons_.getDescription(); }

	bool getDrawPlayerSight() { return drawPlayerSight_.getValue(); }
	void setDrawPlayerSight(bool value) { drawPlayerSight_.setValue(value); }
	const char *getDrawPlayerSightToolTip() { return drawPlayerSight_.getDescription(); }

	bool getOldSightPosition() { return oldSightPosition_.getValue(); }
	void setOldSightPosition(bool value) { oldSightPosition_.setValue(value); }
	const char *getOldSightPositionToolTip() { return oldSightPosition_.getDescription(); }

	bool getDrawPlayerHealth() { return drawPlayerHealth_.getValue(); }
	void setDrawPlayerHealth(bool value) { drawPlayerHealth_.setValue(value); }
	const char *getDrawPlayerHealthToolTip() { return drawPlayerHealth_.getDescription(); }

	bool getDrawPlayerColor() { return drawPlayerColor_.getValue(); }
	void setDrawPlayerColor(bool value) { drawPlayerColor_.setValue(value); }
	const char *getDrawPlayerColorToolTip() { return drawPlayerColor_.getDescription(); }

	bool getDrawWater() { return drawWater_.getValue(); }
	void setDrawWater(bool value) { drawWater_.setValue(value); }
	const char *getDrawWaterToolTip() { return drawWater_.getDescription(); }

	bool getDrawLandscape() { return drawLandscape_.getValue(); }
	void setDrawLandscape(bool value) { drawLandscape_.setValue(value); }
	const char *getDrawLandscapeToolTip() { return drawLandscape_.getDescription(); }

	bool getDrawSurround() { return drawSurround_.getValue(); }
	void setDrawSurround(bool value) { drawSurround_.setValue(value); }
	const char *getDrawSurroundToolTip() { return drawSurround_.getDescription(); }

	bool getNoWaterMovement() { return noWaterMovement_.getValue(); }
	void setNoWaterMovement(bool value) { noWaterMovement_.setValue(value); }
	const char *getNoWaterMovementToolTip() { return noWaterMovement_.getDescription(); }

	bool getNoSkyMovement() { return noSkyMovement_.getValue(); }
	void setNoSkyMovement(bool value) { noSkyMovement_.setValue(value); }
	const char *getNoSkyMovementToolTip() { return noSkyMovement_.getDescription(); }

	bool getNoSkyLayers() { return noSkyLayers_.getValue(); }
	void setNoSkyLayers(bool value) { noSkyLayers_.setValue(value); }
	const char *getNoSkyLayersToolTip() { return noSkyLayers_.getDescription(); }

	bool getNoTrees() { return noTrees_.getValue(); }
	void setNoTrees(bool value) { noTrees_.setValue(value); }
	const char *getNoTreesToolTip() { return noTrees_.getDescription(); }

	bool getNoPrecipitation() { return noPrecipitation_.getValue(); }
	void setNoPrecipitation(bool value) { noPrecipitation_.setValue(value); }
	const char *getNoPrecipitationToolTip() { return noPrecipitation_.getDescription(); }

	bool getLowTreeDetail() { return lowTreeDetail_.getValue(); }
	void setLowTreeDetail(bool value) { lowTreeDetail_.setValue(value); }
	const char *getLowTreeDetailToolTip() { return lowTreeDetail_.getDescription(); }

	bool getSortAccessories() { return sortAccessories_.getValue(); }
	void setSortAccessories(bool value) { sortAccessories_.setValue(value); }
	const char *getSortAccessoriesToolTip() { return sortAccessories_.getDescription(); }

	bool getNoDepthSorting() { return noDepthSorting_.getValue(); }
	void setNoDepthSorting(bool value) { noDepthSorting_.setValue(value); }
	const char *getNoDepthSortingToolTip() { return noDepthSorting_.getDescription(); }

	bool getNoWaves() { return noWaves_.getValue(); }
	void setNoWaves(bool value) { noWaves_.setValue(value); }
	const char *getNoWavesToolTip() { return noWaves_.getDescription(); }

	bool getSoftwareMouse() { return softwareMouse_.getValue(); }
	void setSoftwareMouse(bool value) { softwareMouse_.setValue(value); }
	const char *getSoftwareMouseToolTip() { return softwareMouse_.getDescription(); }

	bool getShowContextHelp() { return showContextHelp_.getValue(); }
	void setShowContextHelp(bool value) { showContextHelp_.setValue(value); }
	const char *getShowContextHelpToolTip() { return showContextHelp_.getDescription(); }

	bool getClientLogToFile() { return clientLogToFile_.getValue(); }
	void setClientLogToFile(bool value) { clientLogToFile_.setValue(value); }
	const char *getClientLogToFileToolTip() { return clientLogToFile_.getDescription(); }

	bool getValidateServerIp() { return validateServerIp_.getValue(); }
	void setValidateServerIp(bool value) { validateServerIp_.setValue(value); }
	const char *getValidateServerIpToolTip() { return validateServerIp_.getDescription(); }

	int getSoundVolume() { return soundVolume_.getValue(); }
	void setSoundVolume(int value) { soundVolume_.setValue(value); }
	const char *getSoundVolumeToolTip() { return soundVolume_.getDescription(); }

	int getBannerRows() { return bannerRows_.getValue(); }
	void setBannerRows(int value) { bannerRows_.setValue(value); }
	const char *getBannerRowsToolTip() { return bannerRows_.getDescription(); }

	int getDialogSize() { return dialogSize_.getValue(); }
	void setDialogSize(int value) { dialogSize_.setValue(value); }
	const char *getDialogSizeToolTip() { return dialogSize_.getDescription(); }

	int getTexSize() { return texSize_.getValue(); }
	void setTexSize(int value) { texSize_.setValue(value); }
	const char *getTexSizeToolTip() { return texSize_.getDescription(); }

	int  getScreenWidth() { return screenWidth_.getValue(); }
	void setScreenWidth(int value) { screenWidth_.setValue(value); }
	const char *getScreenWidthToolTip() { return screenWidth_.getDescription(); }

	int  getScreenHeight() { return screenHeight_.getValue(); }
	void setScreenHeight(int value) { screenHeight_.setValue(value); }
	const char *getScreenHeightToolTip() { return screenHeight_.getDescription(); }

	int getDepthBufferBits() { return depthBufferBits_.getValue(); }
	void setDepthBufferBits(int value) { depthBufferBits_.setValue(value); }
	const char *getDepthBufferBitsToolTip() { return depthBufferBits_.getDescription(); }

	int getColorComponentSize() { return colorComponentSize_.getValue(); }
	void setColorComponentSize(int value) { colorComponentSize_.setValue(value); }
	const char *getColorComponentSizeToolTip() { return colorComponentSize_.getDescription(); }

	int getBitsPerPixel() { return bitsPerPixel_.getValue(); }
	void setBitsPerPixel(int value) { bitsPerPixel_.setValue(value); }
	const char *getBitsPerPixelToolTip() { return bitsPerPixel_.getDescription(); }

	bool getDoubleBuffer() { return doubleBuffer_.getValue(); }
	void setDoubleBuffer(bool value) { doubleBuffer_.setValue(value); }
	const char *getDoubleBufferToolTip() { return doubleBuffer_.getDescription(); }

	const char *getOnlineUserName() { return onlineUserName_.getValue(); }
	void setOnlineUserName(const char *value) { onlineUserName_.setValue(value); }
	const char *getOnlineUserNameToolTip() { return onlineUserName_.getDescription(); }

	const char *getOnlineTankModel() { return onlineTankModel_.getValue(); }
	void setOnlineTankModel(const char *value) { onlineTankModel_.setValue(value); }
	const char *getOnlineTankModelToolTip() { return onlineTankModel_.getDescription(); }

	const char *getOnlineUserIcon() { return onlineUserIcon_.getValue(); }
	void setOnlineUserIcon(const char *value) { onlineUserIcon_.setValue(value); }
	const char *getOnlineUserIconToolTip() { return onlineUserIcon_.getDescription(); }

	const char *getHostDescription() { return hostDescription_.getValue(); }
	void setHostDescription(const char *value) { hostDescription_.setValue(value); }
	const char *getHostDescriptionToolTip() { return hostDescription_.getDescription(); }

	void loadSafeValues();
	void loadDefaultValues();
	bool writeOptionsToFile();
	bool readOptionsFromFile();
	void addToConsole();

protected:
	static OptionsDisplay *instance_;
	std::list<OptionEntry *> options_;

	OptionEntryBoundedInt brightness_;
	OptionEntryBoundedInt explosionParts_;
	OptionEntryBoundedInt explosionSubParts_;
	OptionEntryBoundedInt maxModelTriPercentage_;
	OptionEntryBoundedInt explosionParticleMult_;
	OptionEntryInt depricatedDayTime_;
	OptionEntryInt depricatedSunYZAng_;
	OptionEntryInt depricatedSunXYAng_;
	OptionEntryInt colorComponentSize_;
	OptionEntryInt soundChannels_;
	OptionEntryInt bitsPerPixel_;
	OptionEntryInt depthBufferBits_;
	OptionEntryInt bannerRows_;
	OptionEntryInt tankModelSize_;
	OptionEntryBool detailTexture_;
	OptionEntryBool useHex_;
	OptionEntryBool doubleBuffer_;
	OptionEntryBool moreRes_;
	OptionEntryBool fullScreen_;
	OptionEntryBool fullClear_;
	OptionEntryBool noGLTexSubImage_;
	OptionEntryBool noGLExt_;
	OptionEntryBool noGLMultiTex_;
	OptionEntryBool noGLCompiledArrays_;
	OptionEntryBool noGLEnvCombine_;
	OptionEntryBool noGLCubeMap_;
	OptionEntryBool noGLSphereMap_;
	OptionEntryBool noGLHardwareMipmaps_;
	OptionEntryBool invertElevation_;
	OptionEntryBool invertMouse_;
	OptionEntryBool noSound_;
	OptionEntryBool drawNormals_;
	OptionEntryBool drawLines_;
	OptionEntryBool drawBoundingSpheres_;
	OptionEntryBool noLenseFlare_;
	OptionEntryBool noSkins_;
	OptionEntryBool sortAccessories_;
	OptionEntryBool drawWater_;
	OptionEntryBool drawLandscape_;
	OptionEntryBool drawSurround_;
	OptionEntryBool drawPlayerNames_;
	OptionEntryBool drawPlayerIcons_;
	OptionEntryBool drawPlayerSight_;
	OptionEntryBool oldSightPosition_;
	OptionEntryBool drawPlayerColor_;
	OptionEntryBool drawPlayerHealth_;
	OptionEntryBool noTrees_;
	OptionEntryBool noDepthSorting_;
	OptionEntryBool noROAM_;
	OptionEntryBool noTessalation_;
	OptionEntryBool noVBO_;
	OptionEntryBool useLandscapeTexture_;
	OptionEntryBool noSkyLayers_;
	OptionEntryBool noSkyMovement_;
	OptionEntryBool noPrecipitation_;
	OptionEntryBool frameTimer_;
	OptionEntryBool noWaterMovement_;
	OptionEntryBool noWaves_;
	OptionEntryBool showContextHelp_;
	OptionEntryBool lowTreeDetail_;
	OptionEntryBool softwareMouse_;
	OptionEntryBool sideScroll_;
	OptionEntryBool swapYAxis_;
	OptionEntryBool clientLogToFile_;
	OptionEntryBool validateServerIp_;
	OptionEntryBoundedInt dialogSize_;
	OptionEntryBoundedInt texSize_;
	OptionEntryBoundedInt tankDetail_;
	OptionEntryBoundedInt effectsDetail_;
	OptionEntryBoundedInt soundVolume_;
	OptionEntryInt screenWidth_;
	OptionEntryInt screenHeight_;
	OptionEntryString depricatedUniqueUserId_;
	OptionEntryString hostDescription_;
	OptionEntryString onlineUserName_;
	OptionEntryString onlineTankModel_;
	OptionEntryString onlineUserIcon_;

private:
	OptionsDisplay();
	virtual ~OptionsDisplay();

};


#endif
