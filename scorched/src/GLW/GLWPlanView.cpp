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

#include <GLW/GLWPlanView.h>
#include <GLW/GLWTranslate.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLViewPort.h>
#include <tankgraph/TankModelRenderer.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <client/MainCamera.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <tank/TankContainer.h>
#include <common/Vector.h>
#include <math.h>

REGISTER_CLASS_SOURCE(GLWPlanView);

static const float degToRad = 3.14f / 180.0f;
static const float maxAnimationTime = 2.0f;

GLWPlanView::GLWPlanView(float x, float y, float w, float h) :
	GLWidget(x, y, w, h),
	animationTime_(0.0f), flashTime_(0.0f),
	flash_(true)
{
	setToolTip(new GLWTip("Plan View",
		"Shows the position of the the tanks\n"
		"on a overhead map of the island.\n"
		"Flashing tanks are still to make a move.\n"
		"Clicking on the plan will move the camera\n"
		"to look at that point."));
}

GLWPlanView::~GLWPlanView()
{
}

void GLWPlanView::simulate(float frameTime)
{
	flashTime_ += frameTime;
	if (flashTime_ > 0.3f)
	{
		flashTime_ = 0.0f;
		flash_ = !flash_;
	}

	animationTime_ += frameTime;
	if (animationTime_ > maxAnimationTime)
	{
		animationTime_ = 0.0f;
	}
}

void GLWPlanView::draw()
{
	GLWidget::draw();
	drawMap();
}

void GLWPlanView::drawMap()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::BLEND_ON | GLState::TEXTURE_ON);

	glPushMatrix();
		glTranslatef(x_ + 2.0f, y_ + 2.0f, 0.0f);
		glScalef(w_ - 4.0f, h_ - 4.0f, 1.0f);

		drawTexture();
		{
			GLState currentState2(GLState::TEXTURE_OFF);
			drawCameraPointer();
			drawTanks();
		}
	glPopMatrix();
}

void GLWPlanView::drawTexture()
{
	// Draw the square of land
	glColor3f(1.0f, 1.0f, 1.0f);
	Landscape::instance()->getPlanATexture().draw(true);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
}

void GLWPlanView::drawCameraPointer()
{
	// Draw the camera pointer
	glColor3f(0.9f, 0.9f, 1.0f);
	float mapWidth = (float) ScorchedClient::instance()->
		getLandscapeMaps().getHMap().getWidth();
	Vector currentPos = MainCamera::instance()->
		getCamera().getCurrentPos();
	Vector lookAt = MainCamera::instance()->
		getCamera().getLookAt();
	Vector direction = (currentPos - lookAt).Normalize2D() * 0.2f;

	static Vector mid(128.0f, 128.0f);
	currentPos[2] = 0.0f;
	float dist = (currentPos - mid).Magnitude();
	if (dist > 128.0f)
	{
		currentPos -= mid;
		currentPos *= 128.0f / dist;
		currentPos += mid;
	}
	currentPos /= mapWidth;

	Vector directionPerp = direction.get2DPerp();
	glBegin(GL_LINES);
		glVertex3fv(currentPos);
		glVertex3fv(currentPos - (direction + directionPerp) / 2.0f);
		glVertex3fv(currentPos);
		glVertex3fv(currentPos - (direction - directionPerp) / 2.0f);
		glVertex3fv(currentPos - (direction + directionPerp) / 4.0f);
		glVertex3fv(currentPos - (direction - directionPerp) / 4.0f);
	glEnd();
}

void GLWPlanView::drawTanks()
{
	std::map<unsigned int, Tank *> &currentTanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	if (currentTanks.empty()) return;

	// Draw the current tank bolder
	float width = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth();
	Vector position;
	drawCurrentTank();

	// Draw the rest of the tanks
	// as points on the plan view
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	bool first = true;
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = currentTanks.begin();
		itor != currentTanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal &&
			!tank->getState().getSpectator())
		{
			glColor3fv(tank->getColor());
			position = tank->getPhysics().getTankPosition();
			position /= width;

			if ((flash_ && tank->getState().getReadyState() == TankState::SNotReady) ||
				tank->getState().getReadyState() == TankState::sReady)
			{
				glVertex3fv(position);
			}

			TankModelRenderer *model = (TankModelRenderer *) 
			tank->getModel().getModelIdRenderer();
			if (model)
			{
				GLWToolTip::instance()->addToolTip(&model->getTips()->tankTip,
					GLWTranslate::getPosX() + x_ + position[0] * w_ - 4.0f, 
					GLWTranslate::getPosY() + y_ + position[1] * h_ - 4.0f, 
					8.0f, 8.0f);
			}
		}
	}
	glEnd();
}

void GLWPlanView::drawCurrentTank()
{
	// Draw the extra rings around the currently playing player
	// on the plan view

	// Check that the current player should be shown on the plan
	Tank *currentTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!currentTank || 
		currentTank->getState().getState() != TankState::sNormal) return;
	if (ScorchedClient::instance()->getGameState().getState() != 
		ClientState::StatePlaying) return;

	// Calculate the constants
	float width = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth();
	float scale = animationTime_ / maxAnimationTime;
	Vector position;
	float rot = -currentTank->getPhysics().getRotationGunXY() * degToRad;
	position = currentTank->getPhysics().getTankPosition();
	position /= width;

	// Build a display list to build the ring
	// only once
	static GLuint listNo = 0;
	if (!listNo)
	{
		glNewList(listNo = glGenLists(1), GL_COMPILE);
			glPointSize(1.0f);
			glBegin(GL_LINES);
				for (float a=0; a<=6.28; a+=0.4f)
				{
					glVertex2f(sinf(a) * 0.07f, cosf(a) * 0.07f);
				}
			glEnd();
		glEndList();
	}

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);

		// Draw a expanding ring around the current tank
		glColor3fv(currentTank->getColor());
		glPushMatrix();
			glScalef(scale, scale, scale);
			glCallList(listNo);
		glPopMatrix();

		// Draw a line pointing where the current tank is looking
		glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(
				DefinesUtil::getFastSin(rot) * 0.07f, 
				DefinesUtil::getFastCos(rot) * 0.07f);
		glEnd();
	glPopMatrix();

}

void GLWPlanView::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;

		float mapWidth = (float) ScorchedClient::instance()->
			getLandscapeMaps().getHMap().getWidth();

		float mapX = ((x - x_) / w_) * mapWidth;
		float mapY = ((y - y_) / h_) * mapWidth;

		Vector lookAt(mapX, mapY, ScorchedClient::instance()->
			getLandscapeMaps().getHMap().getInterpHeight(mapX, mapY) + 5.0f);
		MainCamera::instance()->getTarget().setCameraType(TargetCamera::CamFree);
		MainCamera::instance()->getCamera().setLookAt(lookAt);
	}
}
