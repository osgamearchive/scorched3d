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
#include <GLEXT/GLTexture.h>
#include <vector>

class WaterWaves
{
public:
	WaterWaves();
	virtual ~WaterWaves();

	void generateWaves(ProgressCounter *counter);
	void draw();
	void simulate(float frameTime);

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

	GLTexture texture_;
	bool wavePoints_[256 * 256];
	std::vector<WaterWaveEntry> paths_;
	float totalTime_;

	void findPoints(ProgressCounter *counter);
	bool findNextPath(ProgressCounter *counter);
	void findPath(std::vector<Vector> &points, int x, int y);
	void constructLines(std::vector<Vector> &points, int dist);
	void drawBoxes(float totalTime);

};

#endif
