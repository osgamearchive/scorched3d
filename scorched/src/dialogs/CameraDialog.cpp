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

#include <dialogs/CameraDialog.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLOrderedItemRenderer.h>
#include <client/MainCamera.h>
#include <client/Main2DCamera.h>
#include <client/ScorchedClient.h>
#include <landscape/Landscape.h>
#include <tankgraph/TankRenderer.h>

CameraDialog *CameraDialog::instance_ = 0;

CameraDialog *CameraDialog::instance()
{
	if (!instance_)
	{
		instance_ = new CameraDialog;
	}

	return instance_;
}

CameraDialog::CameraDialog() : 
	GLWWindow("Camera", 100, 15, 200, 200, eTransparent | 
	eResizeable | eSmallTitle)
{
	targetCam_.setCameraType(TargetCamera::CamGun);
}

CameraDialog::~CameraDialog()
{
}

void CameraDialog::draw()
{
	const float inset = 5.0f;
	const float corner = 20.0f;

	GLWWindow::draw();
	
	// Add the rounded corners
	{
		GLState newState(GLState::DEPTH_ON | GLState::TEXTURE_OFF | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 4000.0f);	
			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + inset, y_ + inset);
				drawCircle(7, 13, x_ + inset + corner, 
					y_ + inset + corner, corner);
			glEnd();
		
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + w_ - inset, y_ + inset);
				drawCircle(3, 9, x_ + w_ - inset - corner, 
					y_ + inset + corner, corner);
			glEnd();
			
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + w_ - inset, y_ + h_ - inset);
				drawCircle(-1, 5, x_ + w_ - inset - corner, 
					y_ + h_ - inset - corner, corner);
			glEnd();
			
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x_ + inset, y_ + h_ - inset);
				drawCircle(11, 17, x_ + inset + corner, 
					y_ + h_ - inset - corner, corner);
			glEnd();
		glPopMatrix();
	}
	
	// Set the new viewport
	targetCam_.getCamera().setWindowSize(
		int(w_ - inset - inset), int(h_ - inset - inset));
	targetCam_.getCamera().setWindowOffset(
		int(x_ + inset), int(y_ + inset));
	targetCam_.draw();
	
	// Draw the stuff we want to see
	drawLandscape();
	
	// Return the viewport to the original
	Main2DCamera::instance()->draw(0);
	GLCameraFrustum::instance()->draw(0);
}

void CameraDialog::simulate(float frameTime)
{
	// Simulate this viewport
	targetCam_.simulate(frameTime);
}

void CameraDialog::drawLandscape()
{
	GLCameraFrustum::instance()->draw(0);
	TankRenderer::instance()->render3D.draw(0);
	Landscape::instance()->draw(0);
	TankRenderer::instance()->render3DSecond.draw(0);
	ScorchedClient::instance()->getActionController().draw(0);
	GLOrderedItemRenderer::instance()->draw(0);
}
