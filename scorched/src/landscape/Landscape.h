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

#if !defined(__INCLUDE_Landscapeh_INCLUDE__)
#define __INCLUDE_Landscapeh_INCLUDE__

#include <engine/GameStateI.h>
#include <common/ResourceFile.h>
#include <landscape/WaterMap.h>
#include <landscape/WaterWaves.h>
#include <landscape/WaterMapPoints.h>
#include <landscape/PatchGrid.h>
#include <landscape/HeightMapSurround.h>
#include <landscape/LandscapeObjects.h>
#include <landscape/Surround.h>
#include <landscape/Smoke.h>
#include <landscape/Wall.h>
#include <landscape/ShadowMap.h>
#include <landscape/Sun.h>

class Landscape : public GameStateI
{
public:
	static Landscape *instance();

	enum LandscapeTextureType
	{
		eDefault,
		eMovement,
		eOther
	};

	void generate(ProgressCounter *counter = 0);
	void recalculate(int posX, int posY, int dist);
	void restoreLandscapeTexture();
	void reset();

	// Access to internal objects
	Smoke &getSmoke() { return smoke_; }
	ShadowMap &getShadowMap() { return shadowMap_; }
	WaterMap &getWater() { return wMap_; }
	Sun &getSun() { return sun_; }
	Wall &getWall() { return wall_; }
	LandscapeObjects &getObjects() { return objects_; }

	// Textures created during landscape texturing
	GLBitmap &getMainMap() { return mainMap_; }
	GLBitmap &getScorchMap() { return scorchMap_; }
	GLBitmap &getSkyColorsMap() { return skyColorsMap_; }
	GLTexture &getMainTexture() { return texture_; }
	GLTexture &getMagTexture() { return magTexture_; }
	GLTexture &getPlanATexture() { return planAlphaTexture_; }
	GLTexture &getPlanTexture() { return planTexture_; }
	GLTexture &getCloudTexture() { return cloudTexture_; }
	GLTextureBase &getWaterTexture() { return *waterTexture_; }
	GLTexture &getWaterDetail() { return waterDetail_; }
	GLTexture &getWaves1Texture() { return waves1Texture_; }
	GLTexture &getWaves2Texture() { return waves2Texture_; }
	GLTexture &getLandscapeTextureWater() { return landTexWater_; }
	GLTexture &getLandscapeTexture1() { return landTex1_; }

	LandscapeTextureType getTextureType() { return textureType_; }
	void setTextureType(LandscapeTextureType type) { textureType_ = type; }
	ResourceFile &getResources() { return resources_; }

	void updatePlanTexture();
	void updatePlanATexture();

	unsigned int getChangeCount() { return changeCount_; }

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned state, float frameTime);

protected:
	static Landscape *instance_;

	// All objects that are used to draw the scene
	Wall wall_;
	WaterMap wMap_;
	WaterMapPoints wMapPoints_;
	WaterWaves wWaves_;
	PatchGrid patchGrid_;
	HeightMapSurround hMapSurround_;
	SurroundDefs surroundDefs_;
	Surround surround_;
	ShadowMap shadowMap_;
	Smoke smoke_;
	Sun sun_;
	LandscapeObjects objects_;
	LandscapeTextureType textureType_;
	ResourceFile resources_;

	// Textures used for landscape
	GLTexture texture_;
	GLTexture magTexture_;
	GLTexture planTexture_;
	GLTexture planAlphaTexture_;
	GLTexture surroundTexture_;
	GLTexture cloudTexture_;
	GLTexture detailTexture_;
	GLTextureBase *waterTexture_;
	GLTexture waterDetail_;
	GLTexture landTex1_;
	GLTexture landTexWater_;
	GLTexture waves1Texture_;
	GLTexture waves2Texture_;
	GLBitmap mainMap_;
	GLBitmap scorchMap_;
	GLBitmap skyColorsMap_;
	GLBitmap bitmapPlanAlphaAlpha_;
	GLBitmap bitmapPlanAlpha_;
	GLBitmap bitmapPlan_;
	GLBitmap bitmapWater_;

	// Variables used to set when the water is refreshed
	bool resetWater_;
	float resetWaterTimer_;
	unsigned int changeCount_;

	void savePlan();

private:
	Landscape();
	virtual ~Landscape();
};


#endif
