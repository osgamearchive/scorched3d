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


#include <sprites/ExplosionNukeRenderer.h>
#include <common/Defines.h>
#include <landscape/Landscape.h>

ExplosionNukeRenderer::ExplosionNukeRenderer(Vector &position, float size) 
	: totalTime_(0.0f), time_(0.0f), position_(position), size_(size)
{
}

ExplosionNukeRenderer::~ExplosionNukeRenderer()
{
}

void ExplosionNukeRenderer::draw(Action *action)
{

}

void ExplosionNukeRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	time_ += frameTime;

	const float stepSize = 0.25f;
	const float halfSize = size_ / 2.0f;
	while (time_ > stepSize)
	{
		time_ -= stepSize;
		totalTime_ += stepSize;

		for (int i=0; i<30; i++)
		{
			float posXY = (RAND * size_) - halfSize;
			float posYZ = (RAND * size_) - halfSize;
			float posZ = - 5.0f;

			Landscape::instance()->getSmoke().addSmoke(
				position_[0] + posXY, position_[1] + posYZ, position_[2] + posZ,
				0.0f, 0.0f, 0.0f,
				1.0f);
		}
	}

	remove = (totalTime_ > 7.0f);
}