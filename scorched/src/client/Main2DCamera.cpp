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


// Main2DCamera.cpp: implementation of the Main2DCamera class.
//
//////////////////////////////////////////////////////////////////////

#include <client/MainCamera.h>
#include <client/Main2DCamera.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Main2DCamera *Main2DCamera::instance_ = 0;

Main2DCamera *Main2DCamera::instance()
{
	if (!instance_)
	{
		instance_ = new Main2DCamera;
	}

	return instance_;
}

Main2DCamera::Main2DCamera()
{

}

Main2DCamera::~Main2DCamera()
{

}

void Main2DCamera::draw(const unsigned state)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(0, 0, 
		MainCamera::instance()->getCamera().getWidth(), 
		MainCamera::instance()->getCamera().getHeight());
	glOrtho(0.0, MainCamera::instance()->getCamera().getWidth(), 
			0.0, MainCamera::instance()->getCamera().getHeight(), 
			-100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
