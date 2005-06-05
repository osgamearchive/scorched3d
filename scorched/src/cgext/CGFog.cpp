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

#include <cgext/CGFog.h>
#include <GLEXT/GLCamera.h>
#include <landscape/Landscape.h>
#include <landscape/Sky.h>

CGFog *CGFog::instance()
{
	static CGFog fog;
	return &fog;
}

CGFog::CGFog()
{
	program_ = CGLoader::instance()->loadVertexProgram("i:\\wave.cg", "main");
	programb_ = CGLoader::instance()->loadFragmentProgram("i:\\wave2.cg", "main");

	modelViewProj_ = program_->getNamedParameter("modelViewProj");
	modelView_ = program_->getNamedParameter("modelView");
	lightPosition_ = programb_->getNamedParameter("lightPos");
	viewPosition_ = program_->getNamedParameter("viewPosition");
	decal0_ = programb_->getNamedParameter("decal");
}

CGFog::~CGFog()
{
}

void CGFog::bind(unsigned int texNo0)
{
	// Set Params
	cgGLSetParameter4f(lightPosition_, 
		Landscape::instance()->getSky().getSun().getPosition()[0],
		Landscape::instance()->getSky().getSun().getPosition()[1],
		Landscape::instance()->getSky().getSun().getPosition()[2],
		0.0f);
	cgGLSetParameter4f(viewPosition_, 
		GLCamera::getCurrentCamera()->getCurrentPos()[0],
		GLCamera::getCurrentCamera()->getCurrentPos()[1],
		GLCamera::getCurrentCamera()->getCurrentPos()[2],
		0.0f);
	cgGLSetStateMatrixParameter(modelViewProj_, 
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(modelView_, 
		CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetTextureParameter(decal0_, texNo0);
	cgGLEnableTextureParameter(decal0_);

	// Bind
	program_->bind();
	programb_->bind();
}

void CGFog::unBind()
{
	// Unset params
	cgGLDisableTextureParameter(decal0_);

	// UnBind
	program_->unBind();
	programb_->unBind();
}
