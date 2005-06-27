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

#include <landscape/WaterWaves.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLInfo.h>

WaterWaves::WaterWaves() : 
	totalTime_(0.0f), pointCount_(0), removedCount_(0)
{

}

WaterWaves::~WaterWaves()
{
}

void WaterWaves::generateWaves(float waterHeight, WaterMap &wmap, ProgressCounter *counter)
{
	memset(wavePoints_, 0, 256 * 256 * sizeof(bool));
	for (int a=0; a<64; a++)
	{
		for (int b=0; b<64; b++)
		{
			waveDistance_[a + b * 64] = 255.0f;
		}
	}
	paths1_.clear();
	paths2_.clear();
	pointCount_ = 0;
	removedCount_ = 0;

	// Find all of the points that are equal to a certain height (the water height)
	if (counter) counter->setNewOp("Creating waves 1");
	findPoints(waterHeight, counter);

	// Do this after the findPoints so the waveDistance array is always completed
	if (OptionsDisplay::instance()->getNoWaves()) return;
	if (!OptionsDisplay::instance()->getDrawWater()) return;

	// Find the list of points that are next to eachother
	if (counter) counter->setNewOp("Creating waves 2");
	while (findNextPath(waterHeight, wmap, counter)) {}
}

void WaterWaves::findPoints(float waterHeight, ProgressCounter *counter)
{
	for (int y=1; y<255; y++)
	{
		if (counter) counter->setNewPercentage(float(y)/float(255)*100.0f);
		for (int x=1; x<255; x++)
		{
			float height =
				ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getHeight(x, y);
			if (height > waterHeight - 4.0f && height < waterHeight)
			{
				float height2=
					ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getHeight(x+1, y);
				float height3=
					ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getHeight(x-1, y);
				float height4=
					ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getHeight(x, y+1);
				float height5=
					ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getHeight(x, y-1);

				if (height2 > waterHeight || height3 > waterHeight ||
					height4 > waterHeight || height5 > waterHeight)
				{
					if ((x % 2 == 0) && (y % 2 == 0))
					{
						Vector posA(float(x), float(y), 0.0f);
						for (int b=0; b<64; b++)
						{
							for (int a=0; a<64; a++)
							{
								Vector posB(float(a * 4.0f), float(b * 4.0f), 0.0f);
								float distance = (posB - posA).Magnitude();
								waveDistance_[a + b * 64] = MIN(waveDistance_[a + b * 64], distance);
							}
						}
					}

					wavePoints_[x + y * 256] = true;
					pointCount_ ++;
				}
			}
		}
	}
}

bool WaterWaves::findNextPath(float waterHeight, WaterMap &wmap, ProgressCounter *counter)
{
	static std::vector<Vector> points;
	points.clear();

	if (counter) counter->setNewPercentage(float(removedCount_)/float(pointCount_)*100.0f);
	for (int y=1; y<255; y++)
	{
		for (int x=1; x<255; x++)
		{
			if (wavePoints_[x + y * 256]) 
			{
				findPath(points, x, y);
				constructLines(waterHeight, wmap, points);
				return true;			
			}
		}
	}

	return false;
}

void WaterWaves::findPath(std::vector<Vector> &points, int x, int y)
{
	points.push_back(Vector(float(x), float(y), 0.0f));

	wavePoints_[x + y * 256] = false;
	removedCount_ ++;
	if (wavePoints_[(x+1) + y * 256]) findPath(points, x+1, y);
	else if (wavePoints_[(x-1) + y * 256]) findPath(points, x-1, y);
	else if (wavePoints_[x + (y-1) * 256]) findPath(points, x, y-1);
	else if (wavePoints_[x + (y+1) * 256]) findPath(points, x, y+1);
	else if (wavePoints_[(x-1) + (y-1) * 256]) findPath(points, x-1, y-1);
	else if (wavePoints_[(x-1) + (y+1) * 256]) findPath(points, x-1, y+1);
	else if (wavePoints_[(x+1) + (y+1) * 256]) findPath(points, x+1, y+1);
	else if (wavePoints_[(x+1) + (y-1) * 256]) findPath(points, x+1, y-1);
}

void WaterWaves::constructLines(float waterHeight, WaterMap &wmap, std::vector<Vector> &points)
{
	Vector ptA;
	Vector ptB;
	Vector point = points.front();
	int dist = int(RAND * 10.0f + 1.0f);
	for (int i=0; i<(int)points.size(); i++)
	{
		Vector &current = points[i];
		int diffX = int(current[0]) - int(point[0]);
		int diffY = int(current[1]) - int(point[1]);
		int actualdist = diffX * diffX + diffY * diffY;
		if (actualdist  >= dist)
		{
			WaterWaveEntry entry;
			Vector grad = point-current;
			Vector perp = grad.get2DPerp().Normalize2D();

			int newx = int(current[0] + perp[0] * 3.0f);
			int newy = int(current[1] + perp[1] * 3.0f);
			if (newx <= 0 || newy <= 0 ||
				newx >= 256 || newy >= 256)
			{
			}
			else
			{
				if (ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(
					newx, newy) > waterHeight)
				{
					perp =- perp;
					ptA = current - perp / 3.0f;
					ptB = point - perp / 3.0f;
					entry.perp = perp;
					entry.ptC = point + perp * 6.0f;
					entry.ptD = current + perp * 6.0f;
				}
				else
				{
					ptA = point - perp / 3.0f;
					ptB = current - perp / 3.0f;
					entry.perp = perp;
					entry.ptC = current + perp * 6.0f;
					entry.ptD = point + perp * 6.0f;
				}

				entry.ptAEntry = &wmap.getNearestWaterPoint(ptA);
				entry.ptBEntry = &wmap.getNearestWaterPoint(ptB);
				entry.ptCEntry = &wmap.getNearestWaterPoint(entry.ptC);
				entry.ptDEntry = &wmap.getNearestWaterPoint(entry.ptD);

				if (RAND > 0.5f) paths1_.push_back(entry);
				else paths2_.push_back(entry);
			}

			if (i > 1)
			{
				i-= 1;
				point = points[i];
			}
			else point = current;
			
			dist = int(RAND * 10.0f + 1.0f);
		}
	}
}

void WaterWaves::simulate(float frameTime)
{
	totalTime_ += frameTime / 2.0f;
	if (totalTime_ > 6.0f) totalTime_ = 0.0f;
}

void WaterWaves::draw()
{
	if (OptionsDisplay::instance()->getNoWaves()) return;

	Vector windDir = 
		ScorchedClient::instance()->getOptionsTransient().getWindDirection();
	Vector windDirPerp = windDir.Normalize();

	GLState state(GLState::BLEND_OFF | GLState::TEXTURE_ON | GLState::BLEND_ON); 
	glDepthMask(GL_FALSE);

	wavesTexture1_.draw(true);
	drawBoxes(totalTime_ + 0.0f, windDirPerp, paths1_);
	drawBoxes(totalTime_ + 2.0f, windDirPerp, paths1_);
	drawBoxes(totalTime_ + 4.0f, windDirPerp, paths1_);

	wavesTexture2_.draw(true);
	drawBoxes(totalTime_ + 1.0f, windDirPerp, paths2_);
	drawBoxes(totalTime_ + 3.0f, windDirPerp, paths2_);
	drawBoxes(totalTime_ + 5.0f, windDirPerp, paths2_);

	glDepthMask(GL_TRUE);
}

void WaterWaves::drawBoxes(float totalTime, Vector &windDirPerp, 
						   std::vector<WaterWaveEntry> &paths)
{
	float newTime = totalTime;
	if (newTime > 6.0f) newTime -= 6.0f;

	// First set of boxes
	// Some set of magic to try to get it look kind of ok!
	float alpha = 1.0f;
	float frontlen = newTime + 0.2f;
	float endlen = newTime / 2.0f;
	if (newTime < 1.0f)
	{
		alpha = newTime;
	}
	if (newTime > 4.0f)
	{
		frontlen = 4.2f - (newTime - 4.0f) / 3.0f;
		endlen = 2.0f - (newTime - 4.0f) / 3.0f;

		alpha = (2.0f - (newTime - 4.0f)) / 2.0f;
	}
	frontlen *= 2.0f;
	endlen *= 2.0f;

	// Draw the actual texture boxes
	glColor4f(wavesColor_[0], wavesColor_[1], wavesColor_[2], alpha);
	glBegin(GL_QUADS);
		Vector ptA, ptB, ptC, ptD;
		std::vector<WaterWaveEntry>::iterator itor = paths.begin();
		std::vector<WaterWaveEntry>::iterator enditor = paths.end();
		for (; itor != enditor; itor++)
		{
			WaterWaveEntry &p = *itor;
				
			if (((p.perp[0] * windDirPerp[0]) + (p.perp[1] * windDirPerp[1])) > 0.0f) 
				continue;

			ptA = p.ptD - p.perp * frontlen;
			ptA[2] = p.ptAEntry->height + 0.05f;
			ptB = p.ptC - p.perp * frontlen;
			ptB[2] = p.ptBEntry->height + 0.05f;

			ptC = p.ptC - p.perp * endlen;
			ptC[2] = p.ptCEntry->height + 0.05f;
			ptD = p.ptD - p.perp * endlen;
			ptD[2] = p.ptCEntry->height + 0.05f;
			
			glTexCoord2f(1.0f, 1.0f);
			glVertex3fv(ptA);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv(ptB);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(ptC);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv(ptD);
		}
	glEnd();
	GLInfo::addNoTriangles(paths.size() - 2);
}
