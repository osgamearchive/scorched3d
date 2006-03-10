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
#include <tankgraph/TargetRendererImplTank.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <client/MainCamera.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <tank/TankContainer.h>
#include <common/Vector.h>
#include <common/Defines.h>
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
	// Draw the camera pointer
	float mapWidth = (float) ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapWidth();
	float mapHeight = (float) ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapHeight();
	float maxWidth = MAX(mapWidth, mapHeight);

	Vector lowerLeft(0.0f, 0.0f, 0.0f);
	Vector upperRight(mapWidth / maxWidth, mapHeight / maxWidth, 0.0f);
	lowerLeft[0] += (maxWidth - mapWidth) / maxWidth / 2.0f;
	upperRight[0] += (maxWidth - mapWidth) / maxWidth / 2.0f;
	lowerLeft[1] += (maxWidth - mapHeight) / maxWidth / 2.0f;
	upperRight[1] += (maxWidth - mapHeight) / maxWidth / 2.0f;

	// Draw the square of land
	glColor3f(1.0f, 1.0f, 1.0f);
	Landscape::instance()->getPlanATexture().draw(true);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(upperRight[0], lowerLeft[1]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(upperRight[0], upperRight[1]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(lowerLeft[0], upperRight[1]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(lowerLeft[0], lowerLeft[1]);
	glEnd();
}

void GLWPlanView::drawCameraPointer()
{
	// Draw the camera pointer
	float mapWidth = (float) ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapWidth();
	float mapHeight = (float) ScorchedClient::instance()->
		getLandscapeMaps().getGroundMaps().getMapHeight();
	float maxWidth = MAX(mapWidth, mapHeight);

	Vector mid(0.5f, 0.5f);
	// Get camera positions
	Vector currentPos = MainCamera::instance()->
		getCamera().getCurrentPos();
	Vector lookAt = MainCamera::instance()->
		getCamera().getLookAt();

	// Force camera positions between 0 and 1
	currentPos[0] += (maxWidth - mapWidth) / 2.0f;
	currentPos[1] += (maxWidth - mapHeight) / 2.0f;
	currentPos[0] /= maxWidth;
	currentPos[1] /= maxWidth;
	currentPos[2] = 0.0f;
	lookAt[0] += (maxWidth - mapWidth) / 2.0f;
	lookAt[1] += (maxWidth - mapHeight) / 2.0f;
	lookAt[0] /= maxWidth;
	lookAt[1] /= maxWidth;
	lookAt[2] = 0.0f;

	// Make sure currentpos doesn't go outside boundries
	currentPos -= mid;
	float distance = currentPos.Magnitude();
	if (distance > 0.5f)
	{
		currentPos *= 0.5f / distance;
	}
	currentPos += mid;

	// Make sure look at doesn't go outside boundries
	lookAt -= mid;
	distance = lookAt.Magnitude();
	if (distance > 0.48f)
	{
		lookAt *= 0.48f / distance;
	}
	lookAt += mid;

	// Draw direction arrows
	Vector direction = (currentPos - lookAt).Normalize2D() * 0.2f;
	Vector directionPerp = direction.get2DPerp();
	glColor3f(0.9f, 0.9f, 1.0f);
	glBegin(GL_LINES);
		glVertex3fv(currentPos);
		glVertex3fv(currentPos - (direction + directionPerp) / 2.0f);
		glVertex3fv(currentPos);
		glVertex3fv(currentPos - (direction - directionPerp) / 2.0f);
		glVertex3fv(currentPos - (direction + directionPerp) / 4.0f);
		glVertex3fv(currentPos - (direction - directionPerp) / 4.0f);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0F0F);
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_LINES);
		glVertex3fv(currentPos);
		glVertex3fv(lookAt);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void GLWPlanView::drawTanks()
{
	std::map<unsigned int, Tank *> &currentTanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	if (currentTanks.empty()) return;

	// Draw the current tank bolder
	float mapWidth = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
	float mapHeight = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();
	float maxWidth = MAX(mapWidth, mapHeight);

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
			position = tank->getPosition().getTankPosition();
			position[0] += (maxWidth - mapWidth) / 2.0f;
			position[1] += (maxWidth - mapHeight) / 2.0f;
			position /= maxWidth;

			if ((flash_ && tank->getState().getReadyState() == TankState::SNotReady) ||
				tank->getState().getReadyState() == TankState::sReady)
			{
				glVertex3fv(position);
			}

			TargetRendererImplTank *renderer = (TargetRendererImplTank *) 
				tank->getRenderer();
			if (renderer)
			{
				GLWToolTip::instance()->addToolTip(&renderer->getTips()->tankTip,
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
	float mapWidth = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
	float mapHeight = (float) 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();
	float maxWidth = MAX(mapWidth, mapHeight);
	float scale = animationTime_ / maxAnimationTime;
	Vector position;
	float rot = -currentTank->getPosition().getRotationGunXY() * degToRad;
	position = currentTank->getPosition().getTankPosition();
	position[0] += (maxWidth - mapWidth) / 2.0f;
	position[1] += (maxWidth - mapHeight) / 2.0f;
	position /= maxWidth;

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
				getFastSin(rot) * 0.07f, 
				getFastCos(rot) * 0.07f);
		glEnd();
	glPopMatrix();

}

void GLWPlanView::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;

		float mapWidth = (float) ScorchedClient::instance()->
			getLandscapeMaps().getGroundMaps().getMapWidth();
		float mapHeight = (float) ScorchedClient::instance()->
			getLandscapeMaps().getGroundMaps().getMapHeight();
		float maxWidth = MAX(mapWidth, mapHeight);

		float mapX = ((x - x_) / w_) * maxWidth;
		float mapY = ((y - y_) / h_) * maxWidth;
		mapX -= (maxWidth - mapWidth) / 2.0f;
		mapY -= (maxWidth - mapHeight) / 2.0f;

		Vector lookAt(mapX, mapY, ScorchedClient::instance()->
			getLandscapeMaps().getGroundMaps().getInterpHeight(mapX, mapY) + 5.0f);
		MainCamera::instance()->getTarget().setCameraType(TargetCamera::CamFree);
		MainCamera::instance()->getCamera().setLookAt(lookAt);
	}
}
