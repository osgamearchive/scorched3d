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
#include <landscape/WaterMap.h>
#include <landscape/WaterMapPoints.h>
#include <landscape/PatchGrid.h>
#include <landscape/HeightMapSurround.h>
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

	// Textures created during landscape texturing
	GLBitmap &getMainMap() { return mainMap_; }
	GLBitmap &getScorchMap() { return scorchMap_; }
	GLTexture &getMainTexture() { return texture_; }
	GLTexture &getMagTexture() { return magTexture_; }
	GLTexture &getPlanATexture() { return planAlphaTexture_; }
	GLTexture &getPlanTexture() { return planTexture_; }
	GLTexture &getCloudTexture() { return cloudTexture_; }
	GLTexture &getWaterTexture() { return *waterTexture_; }

	LandscapeTextureType getTextureType() { return textureType_; }
	void setTextureType(LandscapeTextureType type) { textureType_ = type; }

	void updatePlanTexture();
	void updatePlanATexture();

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned state, float frameTime);

protected:
	static Landscape *instance_;

	// All objects that are used to draw the scene
	Wall wall_;
	WaterMap wMap_;
	WaterMapPoints wMapPoints_;
	PatchGrid patchGrid_;
	HeightMapSurround hMapSurround_;
	SurroundDefs surroundDefs_;
	Surround surround_;
	ShadowMap shadowMap_;
	Smoke smoke_;
	Sun sun_;
	LandscapeTextureType textureType_;

	// Textures used for landscape
	GLTexture texture_;
	GLTexture magTexture_;
	GLTexture planTexture_;
	GLTexture planAlphaTexture_;
	GLTexture surroundTexture_;
	GLTexture cloudTexture_;
	GLTexture *waterTexture_;
	GLBitmap mainMap_;
	GLBitmap scorchMap_;
	GLBitmap bitmapPlanAlpha_;
	GLBitmap bitmapPlan_;
	GLBitmap bitmapWater_;

	// Variables used to set when the water is refreshed
	bool resetWater_;
	float resetWaterTimer_;

private:
	Landscape();
	virtual ~Landscape();
};


#endif
