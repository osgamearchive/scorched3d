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
#include <landscape/PatchVar.h>
#include <engine/FrameTimer.h>
#include <common/Logger.h>
#include <common/OptionsDisplay.h>
#include <GLEXT/GLBilboardRenderer.h>
#include <client/ScorchedClient.h>
#include <engine/ParticleEngine.h>

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
		unsigned int bils, texs;
		GLBilboardRenderer::instance()->getStats(bils, texs);
		unsigned int pOnScreen = 
			ScorchedClient::instance()->getParticleEngine().getParticlesOnScreen();

		unsigned int tris = (PatchVar::var?PatchVar::var->getNoTriangles():0);
		if (OptionsDisplay::instance()->getFrameTimer())
		{
			Logger::log(0, "%.2f FPS (%iT %iB %iP)", 
				float(totalCount_) / totalTime_,
				tris,
				bils,
				pOnScreen);
		}
		totalCount_ = 0;
		totalTime_ = 0.0f;
	}
}
