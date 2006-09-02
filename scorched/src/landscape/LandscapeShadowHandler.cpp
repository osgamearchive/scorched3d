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

#include <landscape/LandscapeShadowHandler.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Sky.h>
#include <tankgraph/RenderTargets.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLCameraFrustum.h>

LandscapeShadowHandler *LandscapeShadowHandler::instance()
{
	static LandscapeShadowHandler *instance_ = new LandscapeShadowHandler();
	return instance_;
}

LandscapeShadowHandler::LandscapeShadowHandler() : initialized_(false)
{
}

LandscapeShadowHandler::~LandscapeShadowHandler()
{
}

void LandscapeShadowHandler::simulate(const unsigned state, float frameTime)
{
}

void LandscapeShadowHandler::initialize()
{
	// Create the shadow texture
	// Set to false to allow rendering a non-depth scene that we can view
	if (!shadowTexture_.createBufferTexture(2048 * 2, 2048 * 2, true)) 
	{
		dialogExit("Scorched3D", "Failed to create shadow texture");
	}

	// Create the frame buffer
	// Set to GL_COLOR_ATTACHMENT0_EXT to allow viewing a non-depth scene
	if (!frameBuffer_.create(shadowTexture_, GL_DEPTH_ATTACHMENT_EXT))
	{
		dialogExit("Scorched3D", "Failed to create shadow frame buffer");
	}
}

void LandscapeShadowHandler::draw(const unsigned state)
{
	// Check if we are doing hardware shadows
	if (!GLStateExtension::hasHardwareShadows()) return;
	if (!initialized_)
	{
		initialize();
		initialized_ = true;
	}

	// Turn off texturing
	GLState glstate(GLState::TEXTURE_OFF | GLState::DEPTH_ON);

	// Get the sun's position and landscape dimensions
	Vector sunPosition = Landscape::instance()->getSky().getSun().getPosition();
	sunPosition /= 2.0f;
	float landWidth = ScorchedClient::instance()->getLandscapeMaps().
		getGroundMaps().getMapWidth() / 2.0f;
	float landHeight = ScorchedClient::instance()->getLandscapeMaps().
		getGroundMaps().getMapHeight() / 2.0f;

	// Bind the frame buffer so we can render into it
	frameBuffer_.bind();

	// Setup the view from the sun
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(0, 0, shadowTexture_.getWidth(), shadowTexture_.getHeight());
	gluPerspective(60.0f, 1.0f, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		sunPosition[0], sunPosition[1], sunPosition[2], 
		landWidth, landHeight, 0.0f ,
		0.0f, 0.0f, 1.0f);

	// Save the matrixs used for the sun
	glGetDoublev(GL_MODELVIEW_MATRIX, lightModelMatrix_);
	glGetDoublev(GL_PROJECTION_MATRIX, lightProjMatrix_);

	// Clear and setup the offset
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set poly offset so that the shadows dont get precision artifacts
    glPolygonOffset(2.5f, 10.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);

	// Draw 
	drawLandscape();

	// Reset offset
    glDisable(GL_POLYGON_OFFSET_FILL);

	// Stop drawing to frame buffer
	frameBuffer_.unBind();
}

void LandscapeShadowHandler::drawLandscape()
{
	GLCameraFrustum::instance()->draw(0);

	Landscape::instance()->draw(true);
	RenderTargets::instance()->render3D.draw(0);
}
