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
		NoRestore = 4,
		DebugOnly = 8
	};

	bool getHideFinalScore() { return hideFinalScore_; }
	OptionEntryBool &getHideFinalScoreEntry() { return hideFinalScore_; }

	bool getNoPlanDraw() { return noPlanDraw_; }
	OptionEntryBool &getNoPlanDrawEntry() { return noPlanDraw_; }

	int getTankModelSize() { return tankModelSize_; }
	OptionEntryInt &getTankModelSizeEntry() { return tankModelSize_; }

	int getRoamVarianceRamp() { return roamVarianceRamp_; }
	OptionEntryInt &getRoamVarianceRampEntry() { return roamVarianceRamp_; }

	int getRoamVarianceStart() { return roamVarianceStart_; }
	OptionEntryInt &getRoamVarianceStartEntry() { return roamVarianceStart_; }

	int getRoamVarianceTank() { return roamVarianceTank_; }
	OptionEntryInt &getRoamVarianceTankEntry() { return roamVarianceTank_; }

	int getNumberExplosionParts() { return explosionParts_; }
	OptionEntryInt &getNumberExplosionPartsEntry() { return explosionParts_; }

	int getExplosionParticlesMult() { return explosionParticleMult_; }
	OptionEntryInt &getExplosionParticlesMultEntry() { return explosionParticleMult_; }

	int getNumberExplosionSubParts() { return explosionSubParts_; }
	OptionEntryInt &getNumberExplosionSubPartsEntry() { return explosionSubParts_; }

	int getBrightness() { return brightness_; }
	OptionEntryInt &getBrightnessEntry() { return brightness_; }

	float getDrawDistance() { return drawDistance_; }
	OptionEntryFloat &getDrawDistanceEntry() { return drawDistance_; } ;

	float getDrawDistanceFade() { return drawDistanceFade_; }
	OptionEntryFloat &getDrawDistanceFadeEntry() { return drawDistanceFade_; } ;

	bool getSmoothLines() { return smoothLines_; }
	OptionEntryBool &getSmoothLinesEntry() { return smoothLines_; }

	bool getFrameTimer() { return frameTimer_; }
	OptionEntryBool &getFrameTimerEntry() { return frameTimer_; }

	bool getNoLenseFlare() { return noLenseFlare_; }
	OptionEntryBool &getNoLenseFlareEntry() { return noLenseFlare_; }

	bool getNoROAM() { return noROAM_; }
	OptionEntryBool &getNoROAMEntry() { return noROAM_; }

	bool getNoShadows() { return noShadows_; }
	OptionEntryBool &getNoShadowsEntry() { return noShadows_; }

	bool getNoGLShadows() { return noGLShadows_; }
	OptionEntryBool &getNoGLShadowsEntry() { return noGLShadows_; }

	bool getNoDrawParticles() { return noDrawParticles_; }
	OptionEntryBool &getNoDrawParticlesEntry() { return noDrawParticles_; }

	bool getNoSimulateParticles() { return noSimulateParticles_; }
	OptionEntryBool &getNoSimulateParticlesEntry() { return noSimulateParticles_; }

	bool getNoTessalation() { return noTessalation_; }
	OptionEntryBool &getNoTessalationEntry() { return noTessalation_; }

	bool getNoModelLOD() { return noModelLOD_; }
	OptionEntryBool &getNoModelLODEntry() { return noModelLOD_; }

	bool getNoModelLighting() { return noModelLighting_; }
	OptionEntryBool &getNoModelLightingEntry() { return noModelLighting_; }

	bool getNoVBO() { return noVBO_; }
	OptionEntryBool &getNoVBOEntry() { return noVBO_; }

	bool getNoFog() { return noFog_; }
	OptionEntryBool &getNoFogEntry() { return noFog_; }

	bool getHideMenus() { return hideMenus_; }
	OptionEntryBool &getHideMenusEntry() { return hideMenus_; }

	const char *getLastVersionPlayed() { return lastVersionPlayed_; }
	OptionEntryString &getLastVersionPlayedEntry() { return lastVersionPlayed_; }

	bool getDetailTexture() { return detailTexture_; }
	OptionEntryBool &getDetailTextureEntry() { return detailTexture_; }

	bool getUseLandscapeTexture() { return useLandscapeTexture_; }
	OptionEntryBool &getUseLandscapeTextureEntry() { return useLandscapeTexture_; }

	bool getFullScreen() { return fullScreen_; }
	OptionEntryBool &getFullScreenEntry() { return fullScreen_; }

	bool getMoreRes() { return moreRes_; }
	OptionEntryBool &getMoreResEntry() { return moreRes_; }

	bool getFullClear() { return fullClear_; }
	OptionEntryBool &getFullClearEntry() { return fullClear_; }

	bool getInvertElevation() { return invertElevation_; }
	OptionEntryBool &getInvertElevationEntry() { return invertElevation_; }

	bool getInvertMouse() { return invertMouse_; }
	OptionEntryBool &getInvertMouseEntry() { return invertMouse_; }

	bool getSwapYAxis() { return swapYAxis_; }
	OptionEntryBool &getSwapYAxisEntry() { return swapYAxis_; }

	bool getSideScroll() { return sideScroll_; }
	OptionEntryBool &getSideScrollEntry() { return sideScroll_; }

	bool getStorePlayerCamera() { return storePlayerCamera_; }
	OptionEntryBool &getStorePlayerCameraEntry() { return storePlayerCamera_; }

	bool getNoGLExt() { return noGLExt_; }
	OptionEntryBool &getNoGLExtEntry() { return noGLExt_; }

	bool getNoGLTexSubImage() { return noGLTexSubImage_; }
	OptionEntryBool &getNoGLTexSubImageEntry() { return noGLTexSubImage_; }

	bool getNoGLMultiTex() { return noGLMultiTex_; }
	OptionEntryBool &getNoGLMultiTexEntry() { return noGLMultiTex_; }

	bool getNoGLCubeMap() { return noGLCubeMap_; }
	OptionEntryBool &getNoGLCubeMapEntry() { return noGLCubeMap_; }

	bool getNoGLSphereMap() { return noGLSphereMap_; }
	OptionEntryBool &getNoGLSphereMapEntry() { return noGLSphereMap_; }

	bool getNoGLCompiledArrays() { return noGLCompiledArrays_; }
	OptionEntryBool &getNoGLCompiledArraysEntry() { return noGLCompiledArrays_; }
	
	bool getNoGLEnvCombine() { return noGLEnvCombine_; }
	OptionEntryBool &getNoGLEnvCombineEntry() { return noGLEnvCombine_; }

	bool getNoGLHardwareMipmaps() { return noGLHardwareMipmaps_; }
	OptionEntryBool &getNoGLHardwareMipmapsEntry() { return noGLHardwareMipmaps_; }

	bool getNoGLShaders() { return noGLShaders_; }
	OptionEntryBool &getNoGLShadersEntry() { return noGLShaders_; }

	int getSoundChannels() { return soundChannels_; }
	OptionEntryInt &getSoundChannelsEntry() { return soundChannels_; }

	bool getNoSound() { return noSound_; }
	OptionEntryBool &getNoSoundEntry() { return noSound_; }

	bool getNoMusic() { return noMusic_; }
	OptionEntryBool &getNoMusicEntry() { return noMusic_; }

	bool getNoSkins() { return noSkins_; }
	OptionEntryBool &getNoSkinsEntry() { return noSkins_; }

	int getTankDetail() { return tankDetail_; }
	OptionEntryInt &getTankDetailEntry() { return tankDetail_; }

	int getEffectsDetail() { return effectsDetail_; }
	OptionEntryInt &getEffectsDetailEntry() { return effectsDetail_; }

	bool getDrawLines() { return drawLines_; }
	OptionEntryBool &getDrawLinesEntry() { return drawLines_; }

	bool getDrawNormals() { return drawNormals_; }
	OptionEntryBool &getDrawNormalsEntry() { return drawNormals_; }

	bool getDrawGraphicalShadowMap() { return drawGraphicalShadowMap_; }
	OptionEntryBool &getDrawGraphicalShadowMapEntry() { return drawGraphicalShadowMap_; }

	bool getDrawCollisionGeoms() { return drawCollisionGeoms_; }
	OptionEntryBool &getDrawCollisionGeomsEntry() { return drawCollisionGeoms_; }

	bool getDrawCollisionSpace() { return drawCollisionSpace_; }
	OptionEntryBool &getDrawCollisionSpaceEntry() { return drawCollisionSpace_; }

	bool getDrawBoundingSpheres() { return drawBoundingSpheres_; }
	OptionEntryBool &getDrawBoundingSpheresEntry() { return drawBoundingSpheres_; }

	bool getDrawPlayerNames() { return drawPlayerNames_; }
	OptionEntryBool &getDrawPlayerNamesEntry() { return drawPlayerNames_; }

	bool getDrawPlayerIcons() { return drawPlayerIcons_; }
	OptionEntryBool &getDrawPlayerIconsEntry() { return drawPlayerIcons_; }

	bool getDrawPlayerSight() { return drawPlayerSight_; }
	OptionEntryBool &getDrawPlayerSightEntry() { return drawPlayerSight_; }

	bool getOldSightPosition() { return oldSightPosition_; }
	OptionEntryBool &getOldSightPositionEntry() { return oldSightPosition_; }

	bool getDrawPlayerHealth() { return drawPlayerHealth_; }
	OptionEntryBool &getDrawPlayerHealthEntry() { return drawPlayerHealth_; }

	bool getDrawPlayerColor() { return drawPlayerColor_; }
	OptionEntryBool &getDrawPlayerColorEntry() { return drawPlayerColor_; }

	bool getDrawWater() { return drawWater_; }
	OptionEntryBool &getDrawWaterEntry() { return drawWater_; }

	bool getDrawLandscape() { return drawLandscape_; }
	OptionEntryBool &getDrawLandscapeEntry() { return drawLandscape_; }

	bool getDrawSurround() { return drawSurround_; }
	OptionEntryBool &getDrawSurroundEntry() { return drawSurround_; }

	bool getDrawMovement() { return drawMovement_; }
	OptionEntryBool &getDrawMovementEntry() { return drawMovement_; }

	bool getNoWaterMovement() { return noWaterMovement_; }
	OptionEntryBool &getNoWaterMovementEntry() { return noWaterMovement_; }

	bool getNoWaterReflections() { return noWaterReflections_; }
	OptionEntryBool &getNoWaterReflectionsEntry() { return noWaterReflections_; }

	bool getNoWaterBuffers() { return noWaterBuffers_; }
	OptionEntryBool &getNoWaterBuffersEntry() { return noWaterBuffers_; }

	bool getNoWaterWaves() { return noWaterWaves_; }
	OptionEntryBool &getNoWaterWavesEntry() { return noWaterWaves_; }

	bool getNoWaterLOD() { return noWaterLOD_; }
	OptionEntryBool &getNoWaterLODEntry() { return noWaterLOD_; }

	bool getNoSkyMovement() { return noSkyMovement_; }
	OptionEntryBool &getNoSkyMovementEntry() { return noSkyMovement_; }

	bool getNoSkyLayers() { return noSkyLayers_; }
	OptionEntryBool &getNoSkyLayersEntry() { return noSkyLayers_; }

	bool getNoTrees() { return noTrees_; }
	OptionEntryBool &getNoTreesEntry() { return noTrees_; }

	bool getNoPrecipitation() { return noPrecipitation_; }
	OptionEntryBool &getNoPrecipitationEntry() { return noPrecipitation_; }

	bool getLowTreeDetail() { return lowTreeDetail_; }
	OptionEntryBool &getLowTreeDetailEntry() { return lowTreeDetail_; }

	bool getSortAccessories() { return sortAccessories_; }
	OptionEntryBool &getSortAccessoriesEntry() { return sortAccessories_; }

	bool getNoDepthSorting() { return noDepthSorting_; }
	OptionEntryBool &getNoDepthSortingEntry() { return noDepthSorting_; }

	bool getSoftwareMouse() { return softwareMouse_; }
	OptionEntryBool &getSoftwareMouseEntry() { return softwareMouse_; }

	bool getShowContextHelp() { return showContextHelp_; }
	OptionEntryBool &getShowContextHelpEntry() { return showContextHelp_; }

	bool getClientLogToFile() { return clientLogToFile_; }
	OptionEntryBool &getClientLogToFileEntry() { return clientLogToFile_; }

	int getClientLogState() { return clientLogState_; }
	OptionEntryInt &getClientLogStateEntry() { return clientLogState_; }

	bool getValidateServerIp() { return validateServerIp_; }
	OptionEntryBool &getValidateServerIpEntry() { return validateServerIp_; }

	int getSoundVolume() { return soundVolume_; }
	OptionEntryInt &getSoundVolumeEntry() { return soundVolume_; }

	int getAmbientSoundVolume() { return ambientSoundVolume_; }
	OptionEntryInt &getAmbientSoundVolumeEntry() { return ambientSoundVolume_; }

	int getMusicVolume() { return musicVolume_; }
	OptionEntryInt &getMusicVolumeEntry() { return musicVolume_; }

	int getDialogSize() { return dialogSize_; }
	OptionEntryInt &getDialogSizeEntry() { return dialogSize_; }

	int getAntiAlias() { return antiAlias_; }
	OptionEntryInt &getAntiAliasEntry() { return antiAlias_; }

	int getTexSize() { return texSize_; }
	OptionEntryInt &getTexSizeEntry() { return texSize_; }

	int  getScreenWidth() { return screenWidth_; }
	OptionEntryInt  &getScreenWidthEntry() { return screenWidth_; }

	int  getScreenHeight() { return screenHeight_; }
	OptionEntryInt  &getScreenHeightEntry() { return screenHeight_; }

	int getDepthBufferBits() { return depthBufferBits_; }
	OptionEntryInt &getDepthBufferBitsEntry() { return depthBufferBits_; }

	int getColorComponentSize() { return colorComponentSize_; }
	OptionEntryInt &getColorComponentSizeEntry() { return colorComponentSize_; }

	int getBitsPerPixel() { return bitsPerPixel_; }
	OptionEntryInt &getBitsPerPixelEntry() { return bitsPerPixel_; }

	bool getDoubleBuffer() { return doubleBuffer_; }
	OptionEntryBool &getDoubleBufferEntry() { return doubleBuffer_; }

	bool getSaveWindowPositions() { return saveWindowPositions_; }
	OptionEntryBool &getSaveWindowPositionsEntry() { return saveWindowPositions_; }

	const char * getOnlineUserName() { return onlineUserName_; }
	OptionEntryString &getOnlineUserNameEntry() { return onlineUserName_; }

	const char * getOnlineTankModel() { return onlineTankModel_; }
	OptionEntryString &getOnlineTankModelEntry() { return onlineTankModel_; }

	const char * getOnlineUserIcon() { return onlineUserIcon_; }
	OptionEntryString &getOnlineUserIconEntry() { return onlineUserIcon_; }

	Vector getOnlineColor() { return onlineColor_; }
	OptionEntryVector &getOnlineColorEntry() { return onlineColor_; }

	const char * getHostDescription() { return hostDescription_; }
	OptionEntryString &getHostDescriptionEntry() { return hostDescription_; }

	const char * getBuyTab() { return buyTab_; }
	OptionEntryString &getBuyTabEntry() { return buyTab_; }

	int getToolTipTime() { return toolTipTime_; }
	OptionEntryInt &getToolTipTimeEntry() { return toolTipTime_; }

	int getToolTipSpeed() { return toolTipSpeed_; }
	OptionEntryInt &getToolTipSpeedEntry() { return toolTipSpeed_; }

	int getFramesPerSecondLimit() { return framesPerSecondLimit_; }
	OptionEntryInt &getFramesPerSecondLimitEntry() { return framesPerSecondLimit_; }

	bool getFocusPause() { return focusPause_; }
	OptionEntryBool &getFocusPauseEntry() { return focusPause_; }

	void loadSafeValues();
	void loadDefaultValues();
	void loadFastestValues();
	bool writeOptionsToFile();
	bool readOptionsFromFile();

	std::list<OptionEntry *> &getOptions() { return options_; }

protected:
	static OptionsDisplay *instance_;
	std::list<OptionEntry *> options_;

	OptionEntryBool depricatedNoBoidSound_;
	OptionEntryBool depricatedNoBOIDS_;
	OptionEntryBoundedInt depricatedMaxModelTriPercentage_;
	OptionEntryBool depricatedUseHex_;
	OptionEntryInt depricatedDayTime_;
	OptionEntryInt depricatedSunYZAng_;
	OptionEntryInt depricatedSunXYAng_;
	OptionEntryInt bannerRowsDepricated_;
	OptionEntryString depricatedUniqueUserId_;
	OptionEntryBool depricatedNoShips_;
	OptionEntryBool depricatedDrawShipPaths_;
	OptionEntryBool depricatedFirstTimePlayed_;
	OptionEntryBool depricatedNoWaves_;
	OptionEntryBool depricatedNoCg_;
	OptionEntryBool depricatedNoAmbientSound_;

	OptionEntryBoundedInt brightness_;
	OptionEntryBoundedInt explosionParts_;
	OptionEntryBoundedInt explosionSubParts_;
	OptionEntryBoundedInt explosionParticleMult_;
	OptionEntryBool drawMovement_;
	OptionEntryInt colorComponentSize_;
	OptionEntryInt soundChannels_;
	OptionEntryInt bitsPerPixel_;
	OptionEntryInt depthBufferBits_;
	OptionEntryInt tankModelSize_;
	OptionEntryInt toolTipTime_;
	OptionEntryInt toolTipSpeed_;
	OptionEntryInt framesPerSecondLimit_;
	OptionEntryBool noFog_;
	OptionEntryBool detailTexture_;
	OptionEntryBool saveWindowPositions_;
	OptionEntryBool doubleBuffer_;
	OptionEntryBool smoothLines_;
	OptionEntryBool moreRes_;
	OptionEntryBool fullScreen_;
	OptionEntryBool fullClear_;
	OptionEntryBool noGLTexSubImage_;
	OptionEntryBool noGLCubeMap_;
	OptionEntryBool noGLSphereMap_;
	OptionEntryBool noGLExt_;
	OptionEntryBool noGLMultiTex_;
	OptionEntryBool noGLCompiledArrays_;
	OptionEntryBool noGLEnvCombine_;
	OptionEntryBool noGLHardwareMipmaps_;
	OptionEntryBool noGLShaders_;
	OptionEntryBool invertElevation_;
	OptionEntryBool invertMouse_;
	OptionEntryBool noSound_;
	OptionEntryBool noMusic_;
	OptionEntryBool noShadows_;
	OptionEntryBool noGLShadows_;
	OptionEntryBool noDrawParticles_;
	OptionEntryBool noSimulateParticles_;
	OptionEntryBool drawNormals_;
	OptionEntryBool drawGraphicalShadowMap_;
	OptionEntryBool drawLines_;
	OptionEntryFloat drawDistance_;
	OptionEntryFloat drawDistanceFade_;
	OptionEntryBool drawCollisionGeoms_;
	OptionEntryBool drawCollisionSpace_;
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
	OptionEntryBool hideFinalScore_;
	OptionEntryInt roamVarianceStart_;
	OptionEntryInt roamVarianceRamp_;
	OptionEntryInt roamVarianceTank_;
	OptionEntryBool hideMenus_;
	OptionEntryBool noROAM_;
	OptionEntryBool noTessalation_;
	OptionEntryBool noVBO_;
	OptionEntryBool noModelLOD_;
	OptionEntryBool noModelLighting_;
	OptionEntryBool useLandscapeTexture_;
	OptionEntryBool noSkyLayers_;
	OptionEntryBool noSkyMovement_;
	OptionEntryBool noPrecipitation_;
	OptionEntryBool frameTimer_;
	OptionEntryBool noWaterBuffers_;
	OptionEntryBool noWaterMovement_;
	OptionEntryBool noWaterWaves_;
	OptionEntryBool noWaterLOD_;
	OptionEntryBool noWaterReflections_;
	OptionEntryBool showContextHelp_;
	OptionEntryBool lowTreeDetail_;
	OptionEntryBool softwareMouse_;
	OptionEntryBool sideScroll_;
	OptionEntryBool storePlayerCamera_;
	OptionEntryBool swapYAxis_;
	OptionEntryBool clientLogToFile_;
	OptionEntryInt clientLogState_;
	OptionEntryBool validateServerIp_;
	OptionEntryBool noPlanDraw_;
	OptionEntryBoundedInt antiAlias_;
	OptionEntryBoundedInt dialogSize_;
	OptionEntryBoundedInt texSize_;
	OptionEntryBoundedInt tankDetail_;
	OptionEntryBoundedInt effectsDetail_;
	OptionEntryBoundedInt soundVolume_;
	OptionEntryBoundedInt ambientSoundVolume_;
	OptionEntryBoundedInt musicVolume_;
	OptionEntryInt screenWidth_;
	OptionEntryInt screenHeight_;
	OptionEntryString hostDescription_;
	OptionEntryString onlineUserName_;
	OptionEntryString onlineTankModel_;
	OptionEntryString onlineUserIcon_;
	OptionEntryVector onlineColor_;
	OptionEntryString buyTab_;
	OptionEntryString lastVersionPlayed_;
	OptionEntryBool focusPause_;

private:
	OptionsDisplay();
	virtual ~OptionsDisplay();

};

#endif
