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

#include <stdio.h>
#include <engine/FrameTimer.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <common/OptionsDisplay.h>
#include <sound/Sound.h>
#include <GLEXT/GLInfo.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <engine/ActionController.h>
#include <engine/ParticleEngine.h>
#include <landscape/Landscape.h>

FrameTimer *FrameTimer::instance_ = 0;

FrameTimer *FrameTimer::instance()
{
	if (!instance_)
	{
		instance_ = new FrameTimer;
	}

	return instance_;
}

FrameTimer::FrameTimer() : totalTime_(0.0f), totalCount_(0)
{

}

FrameTimer::~FrameTimer()
{

}

void FrameTimer::simulate(const unsigned state, float frameTime)
{
	totalTime_ += frameTime;
	totalCount_++;

	if (totalTime_ > 5.0f)
	{
		unsigned int pOnScreen = 
			ScorchedClient::instance()->
				getParticleEngine().getParticlesOnScreen() +
			MainCamera::instance()->getTarget().
				getPrecipitationEngine().getParticlesOnScreen();

		unsigned int tris = GLInfo::getNoTriangles();
		if (OptionsDisplay::instance()->getFrameTimer())
		{
			int enabledGeoms = 0;
			int geoms = 
				dSpaceGetNumGeoms(
					ScorchedClient::instance()->getActionController().
						getPhysics().getSpace());
			for (int i=0; i<geoms; i++)
			{
				dGeomID geom = 
					dSpaceGetGeom(
						ScorchedClient::instance()->getActionController().
							getPhysics().getSpace(), i);
				if (dGeomIsEnabled(geom) == 1) enabledGeoms++;
			}

			Logger::log(LoggerInfo(LoggerInfo::TypePerformance, 
				formatString("%.2f FPS",
					float(totalCount_) / totalTime_)));
			Logger::log(LoggerInfo(LoggerInfo::TypePerformance, 
				formatString("%iTRI %iPART %iGEOM %iSQR %iSND", 
					tris,
					pOnScreen,
					enabledGeoms,
					Landscape::instance()->getPatchGrid().getDrawnPatches(),
					Sound::instance()->getPlayingChannels())));
		}
		totalCount_ = 0;
		totalTime_ = 0.0f;
	}
}
