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

#if !defined(__INCLUDE_WaterWavesh_INCLUDE__)
#define __INCLUDE_WaterWavesh_INCLUDE__

#include <common/Vector.h>
#include <common/ProgressCounter.h>
#include <landscape/WaterMap.h>
#include <vector>

class WaterWaves
{
public:
	WaterWaves();
	virtual ~WaterWaves();

	void generateWaves(float waterHeight, 
		WaterMap &map, 
		ProgressCounter *counter = 0);
	void draw();
	void simulate(float frameTime);

	GLTexture &getWavesTexture1() { return wavesTexture1_; }
	GLTexture &getWavesTexture2() { return wavesTexture2_; }
	Vector &getWavesColor() { return wavesColor_; }

protected:
	struct WaterWaveEntry 
	{
		Vector perp;
		WaterMap::WaterEntry *ptAEntry;
		WaterMap::WaterEntry *ptBEntry;
		WaterMap::WaterEntry *ptCEntry;
		WaterMap::WaterEntry *ptDEntry;
		Vector ptC;
		Vector ptD;
	};

	bool wavePoints_[256 * 256];
	std::vector<WaterWaveEntry> paths1_;
	std::vector<WaterWaveEntry> paths2_;
	float totalTime_;
	unsigned int pointCount_;
	unsigned int removedCount_;

	void findPoints(float waterHeight, ProgressCounter *counter);
	bool findNextPath(float waterHeight, WaterMap &wmap, ProgressCounter *counter);
	void findPath(std::vector<Vector> &points, int x, int y);
	void constructLines(float waterHeight, WaterMap &wmap, std::vector<Vector> &points);
	void drawBoxes(float totalTime, Vector &windDir, 
		std::vector<WaterWaveEntry> &paths);

	GLTexture wavesTexture1_;
	GLTexture wavesTexture2_;
	Vector wavesColor_;

};

#endif
