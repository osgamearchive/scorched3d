////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_Waterh_INCLUDE__)
#define __INCLUDE_Waterh_INCLUDE__

#include <landscape/WaterMap.h>
#include <landscape/WaterWaves.h>
#include <landscape/WaterMapPoints.h>
#include <common/ProgressCounter.h>

class Water
{
public:
	Water();
	virtual ~Water();

	void draw();
	void reset();
	void recalculate();
	void generate(ProgressCounter *counter = 0);
	void simulate(float frameTime);
	bool explosion(Vector position, float size);
	void addWave(Vector position, float height);

	bool getWaterOn() { return waterOn_; }
	float getWaterHeight() { return height_; }
	GLBitmap &getWaterBitmap() { return bitmapWater_; }

protected:
	bool waterOn_;
	bool resetWater_;
	float resetWaterTimer_;
	float height_;
	WaterMap wMap_;
	WaterMapPoints wMapPoints_;
	WaterWaves wWaves_;
	GLTexture landTexWater_;
	GLBitmap bitmapWater_;

};

#endif // __INCLUDE_Waterh_INCLUDE__
