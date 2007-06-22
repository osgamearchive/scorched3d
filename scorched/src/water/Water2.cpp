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

#include <water/Water2.h>
#include <common/Vector.h>
#include <common/Vector4.h>
#include <common/ProgressCounter.h>
#include <common/OptionsTransient.h>
#include <client/ScorchedClient.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapemap/LandscapeMaps.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLImageFactory.h>
#include "ocean_wave_generator.h"

#include <water/Water2Constants.h>

Water2::Water2()
{
}

Water2::~Water2()
{
	
}

Water2Patches &Water2::getPatch(float time)
{
	return patches_[int(time) % wave_phases];
}

void Water2::generate(LandscapeTexBorderWater *water, ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Water motion");

	// Calculate water for position n
	float windSpeed = ScorchedClient::instance()->
		getOptionsTransient().getWindSpeed() * 2.0f + 3.0f;
	Vector windDir = ScorchedClient::instance()->
		getOptionsTransient().getWindDirection();
	if (windDir == Vector::nullVector)
	{
		windDir = Vector(0.8f, 0.8f);
	}

	ocean_wave_generator<float> 
		owg(wave_resolution, // Resolution
			windDir, // Wind dir
			windSpeed,  // wind speed m/s
			float(wave_resolution) * (1e-8f), // scale factor for heights
			float(wave_waterwidth), // wavetile_length
			wave_tidecycle_time); // wave_tidecycle_time

	// For each frame get the height data
	std::vector<Vector> displacements[256];
	for (unsigned i=0; i<wave_phases; i++) 
	{
		if (counter) counter->setNewPercentage(float(i * 100) / float(wave_phases));

		// Set frame number
		float currentTime = wave_tidecycle_time * float(i) / float(wave_phases);
		float timeMod = myfmod(currentTime, wave_tidecycle_time);
		owg.set_time(timeMod);

		// Calculate Zs
		std::vector<float> heights;
		owg.compute_heights(heights);

		// Calculate X,Ys
		owg.compute_displacements(-2.0f, displacements[i]);
		DIALOG_ASSERT(heights.size() == displacements[i].size());

		// Form a vector with the correct X,Y,Zs
		int j = 0;
		for (int y=0; y<wave_resolution; y++)
		{
			for (int x=0; x<wave_resolution; x++, j++)	
			{
				displacements[i][j][2] = water->height + heights[j];
			}
		}

		// Create the patches
		patches_[i].generate(displacements[i], wave_resolution, wave_patch_width);
	}

	if (indexs_.getNoPositions() == 0)
	{
		// Create the indexes
		indexs_.generate(wave_patch_width);

		// Create visibility mesh
		Vector offset(-1536.0f, -1536.0f, 0.0f);
		visibility_.generate(offset, wave_resolution * 13, wave_resolution, wave_patch_width);
	}

	// compute amount of foam per vertex sample
	if (GLStateExtension::hasShaders())
	{
		LandscapeDefn &defn = *ScorchedClient::instance()->getLandscapeMaps().
			getDefinitions().getDefn();

		GLImageHandle aofImage[256];
		for (int i=0; i<256; i++)
		{
			aofImage[i] = 
				GLImageFactory::createBlank(wave_resolution, wave_resolution, false, 0);
		}
		std::vector<float> aof(wave_resolution*wave_resolution);

		float rndtab[37];
		for (unsigned k = 0; k < 37; ++k)
			rndtab[k] = RAND;

		// factor to build derivatives correctly
		const float deriv_fac = wavetile_length_rcp * wave_resolution;
		const float lambda = 1.0; // lambda has already been multiplied with x/y displacements...
		const float decay = 4.0/wave_phases;
		const float decay_rnd = 0.25/wave_phases;
		const float foam_spawn_fac = 0.25;//0.125;


		for (int landfoam=0; landfoam<=1; landfoam++)
		{

			if (counter) counter->setNewOp((char *) formatString("Water waves %i", (landfoam+1)));

		for (unsigned k = 0; k < wave_phases * 2; ++k) {
			if (counter) counter->setNewPercentage(float(k * 50) / float(wave_phases));

			const std::vector<Vector>& wd = displacements[k % wave_phases];
			// compute for each sample how much foam is added (spawned)
			for (unsigned y = 0; y < wave_resolution; ++y) {
				unsigned ym1 = (y + wave_resolution - 1) & (wave_resolution-1);
				unsigned yp1 = (y + 1) & (wave_resolution-1);
				for (unsigned x = 0; x < wave_resolution; ++x) {
					unsigned xm1 = (x + wave_resolution - 1) & (wave_resolution-1);
					unsigned xp1 = (x + 1) & (wave_resolution-1);
					float dispx_dx = (wd[y*wave_resolution+xp1][0] - wd[y*wave_resolution+xm1][0]) * deriv_fac;
					float dispx_dy = (wd[yp1*wave_resolution+x][0] - wd[ym1*wave_resolution+x][0]) * deriv_fac;
					float dispy_dx = (wd[y*wave_resolution+xp1][1] - wd[y*wave_resolution+xm1][1]) * deriv_fac;
					float dispy_dy = (wd[yp1*wave_resolution+x][1] - wd[ym1*wave_resolution+x][1]) * deriv_fac;
					float Jxx = 1.0f + lambda * dispx_dx;
					float Jyy = 1.0f + lambda * dispy_dy;
					float Jxy = lambda * dispy_dx;
					float Jyx = lambda * dispx_dy;
					float J = Jxx*Jyy - Jxy*Jyx;

					if (landfoam == 1)
					{
						// Not quite right!! but it will do
						int lx = int(float(x) * float(defn.landscapewidth) / float(wave_resolution));
						int ly = int(float(y) * float(defn.landscapeheight) / float(wave_resolution));

						float height = ScorchedClient::instance()->getLandscapeMaps().
							getGroundMaps().getHeight(lx, ly);
						if (height > water->height - 1.0f) 
						{
							J = -(1.0f - (height - water->height));
						}
					}
					//float heightdiff = MAX(1.0f, 3.0f - MAX(0.0f, 5.0f - height));
					//J *= heightdiff;

					//printf("x,y=%u,%u, Jxx,yy=%f,%f Jxy,yx=%f,%f J=%f\n",
					//       x,y, Jxx,Jyy, Jxy,Jyx, J);
					//double foam_add = (J < 0.3) ? ((J < -1.0) ? 1.0 : (J - 0.3)/-1.3) : 0.0;
					float foam_add = (J < 0.0f) ? ((J < -1.0f) ? 1.0f : -J) : 0.0f;			

					aof[y*wave_resolution+x] += foam_add * foam_spawn_fac;
					// spawn foam also on neighbouring fields
					aof[ym1*wave_resolution+x] += foam_add * foam_spawn_fac * 0.5f;
					aof[yp1*wave_resolution+x] += foam_add * foam_spawn_fac * 0.5f;
					aof[y*wave_resolution+xm1] += foam_add * foam_spawn_fac * 0.5f;
					aof[y*wave_resolution+xp1] += foam_add * foam_spawn_fac * 0.5f;
				}
			}

			// compute decay, depends on time with some randomness
			unsigned ptr = 0;
			for (unsigned y = 0; y < wave_resolution; ++y) {
				for (unsigned x = 0; x < wave_resolution; ++x) {
					float aofVal = std::max(std::min(aof[ptr], 1.0f) - (decay + decay_rnd * rndtab[(3*x + 5*y) % 37]), 0.0f);

					aof[ptr] = aofVal;

					if (k >= wave_phases) {
						aofImage[k - wave_phases].getBits()[ptr * 3 + landfoam] = (unsigned char) (255.0f * aofVal);
					}

					++ptr;
				}
			}

			// store amount of foam data when in second iteration
			if (k >= wave_phases && landfoam == 1) {
				Water2Patches &patches = patches_[k - wave_phases];

				patches.getAOF().create(aofImage[k - wave_phases]);
			}
		}

		}
	}
}
