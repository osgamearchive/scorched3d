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

#include <client/Main2DCamera.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <engine/MainLoop.h>
#include <GLEXT/GLState.h>

Main2DCamera *Main2DCamera::instance_ = 0;

Main2DCamera *Main2DCamera::instance()
{
	if (!instance_)
	{
		instance_ = new Main2DCamera;
	}

	return instance_;
}

Main2DCamera::Main2DCamera() :
	GameStateI("Main2DCamera")
{

}

Main2DCamera::~Main2DCamera()
{

}

void Main2DCamera::draw(const unsigned state)
{
	if (OptionsDisplay::instance()->getFullClear())
	{
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		if (ScorchedClient::instance()->getMainLoop().getFlip())
		{
			glClearDepth(0.5f);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glClearDepth(0.5f);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	viewPort_.draw();
}
