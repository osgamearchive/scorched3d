#pragma once

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
