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
#include <common/Display.h>
#include <common/OptionsDisplay.h>

Display *Display::instance_ = 0;

Display *Display::instance()
{
	if (!instance_)
	{
		instance_ = new Display;
	}

	return instance_;
}

Display::Display() : init_(false)
{

}

Display::~Display()
{
}

bool Display::init()
{
	init_ = false;
	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo) return false;
         
	/* opengl sdl mode  */
	videoFlags  = SDL_OPENGL;  
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;      
	//videoFlags |= SDL_RESIZABLE; // We cannot resize the window as we loose all texture

	/* checks if surfaces can be stored in memory */
	if ( videoInfo->hw_available ) videoFlags |= SDL_HWSURFACE;
	else videoFlags |= SDL_SWSURFACE;

	/* check if hardware blits can be done */
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;

	/* set opengl double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

	// At least 24 bits depth buffer
	int depthBufferBits = OptionsDisplay::instance()->getDepthBufferBits();
	if (SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depthBufferBits ) == -1) return false;
        
	init_ = true;
	return init_;
}

void Display::autoSettings(int &width, int &height, bool &full)
{
	SDL_Rect **modes;
		
	/* minimal default mode */
	width = 640;
	height = 480;
	full = false;
	
	/* try and fetch the best mode */
	modes=SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
	if (modes > (SDL_Rect **)0) {
		height = modes[0]->h; 
		width = modes[0]->w;
		full = true;
	}
}

bool Display::changeSettings(int width, int height, bool full)
{
	if (init_)
	{	
		/* create display surface */
		surface = SDL_SetVideoMode( width, height, 
			videoInfo->vfmt->BitsPerPixel, 
			( full ? videoFlags|SDL_FULLSCREEN : videoFlags));

		if (surface) return true;
	}
	return false;
}

