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

#include <cgext/CGLoader.h>
#include <common/Defines.h>

CGLoader *CGLoader::instance_ = 0;

CGLoader *CGLoader::instance()
{
	if (!instance_) instance_ = new CGLoader;
	return instance_;
}

CGLoader::CGLoader() :
	cgContext_(0),
	init_(false), counter_(0)
{
}

CGLoader::~CGLoader()
{
	if (cgContext_) cgDestroyContext(cgContext_);
}

bool CGLoader::cgEnabled()
{
#ifdef HAVE_CG
	return init_;
#endif
	return false;
}

bool CGLoader::init()
{
	if (!cgEnabled()) return true;

	// Setup Cg
	// Create A New Context For Our Cg Program(s)
	cgContext_ = cgCreateContext();

	// Validate Our Context Generation Was Successful
	if (!cgContext_)
	{
		init_ = false;
		dialogMessage("CGLoader", "Failed To Create Cg Context");
		return false;
	}

	init_ = true;
	return true;
}

CGProgram *CGLoader::loadVertexProgram(const char *fileName,
	const char *entryFn)
{
	if (!init_) return 0;

	// Get The Latest GL Vertex Profile
	CGprofile cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);

	// Validate Our Profile Determination Was Successful
	if (cgVertexProfile == CG_PROFILE_UNKNOWN)
	{
		dialogMessage("CGLoader", "%s : Invalid profile type",
			fileName);
		return false;
	}

	// Set The Current Profile
	cgGLSetOptimalOptions(cgVertexProfile);

	// Load And Compile The Vertex Shader From File
	CGprogram cgProgram = cgCreateProgramFromFile(cgContext_, 
		CG_SOURCE, fileName, cgVertexProfile, 
		entryFn, 
		0);

	// Validate Success
	if (!cgProgram)
	{
		// We Need To Determine What Went Wrong
		CGerror Error = cgGetError();

		// Show A Message Box Explaining What Went Wrong
		dialogMessage("CGLoader", "%s : %s",
			fileName, cgGetErrorString(Error));
		init_ = false;
		return 0;
	}

	// Load The Program
	cgGLLoadProgram(cgProgram);

	// Return this new program
	return new CGProgram(cgProgram, cgVertexProfile);
}

CGProgram *CGLoader::loadFragmentProgram(const char *fileName,
	const char *entryFn)
{
	if (!init_) return 0;

	// Get The Latest GL Fragment Profile
	CGprofile cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	// Validate Our Profile Determination Was Successful
	if (cgFragmentProfile == CG_PROFILE_UNKNOWN)
	{
		dialogMessage("CGLoader", "%s : Invalid profile type", 
			fileName);
		return false;
	}

	// Set The Current Profile
	cgGLSetOptimalOptions(cgFragmentProfile);

	// Load And Compile The Vertex Shader From File
	CGprogram cgProgram = cgCreateProgramFromFile(cgContext_, 
		CG_SOURCE, fileName, cgFragmentProfile, 
		entryFn, 
		0);

	// Validate Success
	if (!cgProgram)
	{
		// We Need To Determine What Went Wrong
		CGerror Error = cgGetError();

		// Show A Message Box Explaining What Went Wrong
		dialogMessage("CGLoader", "%s : %s", 
			fileName, cgGetErrorString(Error));
		init_ = false;
		return 0;
	}

	// Load The Program
	cgGLLoadProgram(cgProgram);

	// Return this new program
	return new CGProgram(cgProgram, cgFragmentProfile);
}
