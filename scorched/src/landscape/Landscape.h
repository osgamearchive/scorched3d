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
#include <landscape/Water.h>
#include <landscape/PatchGrid.h>
#include <landscape/LandscapeObjects.h>
#include <landscape/LandscapePoints.h>
#include <landscape/Smoke.h>
#include <landscape/Wall.h>
#include <landscape/ShadowMap.h>

class Surround;
class Sky;
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
	Wall &getWall() { return wall_; }
	Sky &getSky() { return *sky_; }
	Water &getWater() { return water_; }
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
	GLTexture &getLandscapeTexture1() { return landTex1_; }
	GLTexture &getSurroundTexture() { return surroundTexture_; }
	GLTexture &getGroundTexture() { return groundTexture_; }
	GLTexture &getDetailTexture() { return detailTexture_; }
	GLTexture &getRoofTexture() { return roofTexture_; }

	LandscapeTextureType getTextureType() { return textureType_; }
	void setTextureType(LandscapeTextureType type) { textureType_ = type; }

	void updatePlanTexture();
	void updatePlanATexture();
	int getPlanTexSize();
	int getMapTexSize();

	unsigned int getChangeCount() { return changeCount_; }

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned state, float frameTime);

protected:
	static Landscape *instance_;

	// All objects that are used to draw the scene
	Wall wall_;
	PatchGrid patchGrid_;
	Sky *sky_;
	Surround *surround_;
	ShadowMap shadowMap_;
	Smoke smoke_;
	Water water_;
	LandscapeObjects objects_;
	LandscapeTextureType textureType_;
	LandscapePoints points_;

	// Textures used for landscape
	GLTexture texture_;
	GLTexture magTexture_;
	GLTexture planTexture_;
	GLTexture planAlphaTexture_;
	GLTexture surroundTexture_;
	GLTexture cloudTexture_;
	GLTexture detailTexture_;
	GLTexture roofTexture_;
	GLTexture landTex1_;
	GLTexture groundTexture_;
	GLBitmap mainMap_;
	GLBitmap scorchMap_;
	GLBitmap skyColorsMap_;
	GLBitmap bitmapPlanAlphaAlpha_;
	GLBitmap bitmapPlanAlpha_;
	GLBitmap bitmapPlan_;

	// Variables used to set when the water is refreshed
	bool resetLandscape_;
	float resetLandscapeTimer_;
	unsigned int changeCount_;

	void savePlan();

private:
	Landscape();
	virtual ~Landscape();
};


#endif
