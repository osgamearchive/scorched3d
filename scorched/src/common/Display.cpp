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
#include <common/Defines.h>

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
	if (!videoInfo)
	{
		dialogMessage("Display", "ERROR: Failed to get the video information");
		return false;
	}
         
	/* set opengl double buffering */
	int doubleBuffer = OptionsDisplay::instance()->getDoubleBuffer()?1:0;
	if (SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, doubleBuffer ) == -1) 
	{
		dialogMessage("Display", "ERROR: Failed to set double buffer");
		return false;
	}

	/* set opengl component size */
	int componentSize = OptionsDisplay::instance()->getColorComponentSize();
	if (SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, componentSize) == -1 ||
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, componentSize) == -1 ||
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, componentSize) == -1 ||
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, componentSize) == -1)
	{
		dialogMessage("Display", "ERROR: Failed to set 5 bits per pixel");
		return false;
	}

	// At least 24 bits depth buffer
	int depthBufferBits = OptionsDisplay::instance()->getDepthBufferBits();
	if (SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depthBufferBits ) == -1)
	{
		dialogMessage("Display", "ERROR: Failed to set the depth buffer to %i bits",
			depthBufferBits);
		return false;
	}
        
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
		int videoFlags = SDL_OPENGL | SDL_ANYFORMAT;  
		int flags = ( full ? videoFlags|SDL_FULLSCREEN : videoFlags);
		surface = SDL_SetVideoMode( width, height, 
			videoInfo->vfmt->BitsPerPixel, 
			flags);
		if (!surface)
		{
			char buffer[256];
			SDL_VideoDriverName(buffer, 256);
			dialogMessage("Display", "ERROR: Failed to set video mode.\n"
				"Error Message: %s\n"
				"----------------------------\n"
				"Requested Display Mode:-\n"
				"Driver:%s\n"
				"Resolution: %s:%ix%ix%i (depth = %i)\n"
				"DoubleBuffer=%s\n"
				"ColorComponentSize=%i\n",
				SDL_GetError(),
				buffer, 
				(full?"fullscreen":"windowed"), width, height, videoInfo->vfmt->BitsPerPixel, 
				OptionsDisplay::instance()->getDepthBufferBits(),
				OptionsDisplay::instance()->getDoubleBuffer()?"On":"Off",
				OptionsDisplay::instance()->getColorComponentSize());
			return false;
		}

		if (surface) return true;
	}
	return false;
}

