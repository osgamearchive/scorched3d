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

#if !defined(AFX_WATERMAP_H__19A193C1_7DDC_42A8_8D15_7837DCC1DC46__INCLUDED_)
#define AFX_WATERMAP_H__19A193C1_7DDC_42A8_8D15_7837DCC1DC46__INCLUDED_

#include <GLEXT/GLState.h>
#include <common/Vector.h>
#include <landscape/WaterMapSurround.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLTextureCubeMap.h>
#include <GLEXT/GLBitmap.h>

class WaterMap  
{
public:
	struct WaterEntry
	{
		float height;
		float force;
		float velocity;
		float texX;
		float texY;
		float depth;
		float scaling;
		bool dontDraw;
		bool *notVisible;
		Vector normal;
	} *heights_;

	WaterMap(int width, int squareWidth);
	virtual ~WaterMap();

	void draw();
	void simulate(float frameTime);
	void addWave(int posX, int posY, float height);

	WaterEntry &getNearestWaterPoint(Vector &point);
	int getWidth() { return width_; }
	float getWidthMult() { return widthMult_; }
	bool &getDrawNormals() { return drawNormals_; }
	bool &getDrawVisiblePoints() { return drawVisiblePoints_; }
	GLTextureBase *&getWaterTexture() { return waterTexture_; }
	GLTexture &getWaterDetail() { return waterDetail_; }

	void reset();

protected:
	int width_;
	int squareSize_;
	int noVisiblesWidth_;
	float widthMult_;
	WaterMapSurround surround_;
	struct VisibleEntry
	{
		bool notVisible;
		Vector pos;
	} *visible_;

	bool drawNormals_;
	bool drawVisiblePoints_;
	GLTextureBase *waterTexture_;
	GLTexture waterDetail_;

	WaterEntry &getEntry(int i, int j) { return heights_[i + j * width_]; }
	void generateSplash();
	void calculateRipples();
	void drawWater();
	void drawNormals();
	void refreshVisiblity();
};

#endif // !defined(AFX_WATERMAP_H__19A193C1_7DDC_42A8_8D15_7837DCC1DC46__INCLUDED_)
