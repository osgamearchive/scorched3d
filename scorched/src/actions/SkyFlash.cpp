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

#include <actions/SkyFlash.h>
#include <engine/ScorchedContext.h>
#include <landscape/Landscape.h>
#include <landscape/Sky.h>

REGISTER_ACTION_SOURCE(SkyFlash);

SkyFlash::SkyFlash() 
{
}

SkyFlash::~SkyFlash()
{
}

void SkyFlash::init()
{
}

void SkyFlash::simulate(float frameTime, bool &remove)
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		Landscape::instance()->getSky().flashSky();
	}
#endif // #ifndef S3D_SERVER

	remove = true;
	Action::simulate(frameTime, remove);
}

bool SkyFlash::writeAction(NetBuffer &buffer)
{
	return true;
}

bool SkyFlash::readAction(NetBufferReader &reader)
{
	return true;
}
