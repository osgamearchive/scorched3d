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

#include <GLW/GLWWindView.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <3dsparse/ASEStore.h>
#include <GLEXT/GLViewPort.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <landscape/Landscape.h>
#include <math.h>

REGISTER_CLASS_SOURCE(GLWWindView);

WindDialogToolTip::WindDialogToolTip()
{
}

WindDialogToolTip::~WindDialogToolTip()
{
}

void WindDialogToolTip::populate()
{
	if ((int) ScorchedClient::instance()->
		getOptionsTransient().getWindSpeed() == 0)
	{
		setText("Wind",
			"Displays the current wind direction\n"
			"and speed.\n"
			"Currently No Wind.\n");
	}
	else
	{
		setText("Wind",
			"Displays the current wind direction\n"
			"and speed.\n"
			"Current Wind Force : %i (out of 5)\n",
			(int) ScorchedClient::instance()->
			getOptionsTransient().getWindSpeed());
	}
}

GLWWindView::GLWWindView(float x, float y, float w, float h) :
	GLWVisibleWidget(x, y, w, h),
	listNo_(0), changeCount_(0)
{
	setToolTip(new WindDialogToolTip());
	windModel_ = ASEStore::instance()->
		loadOrGetArray(getDataFile("data/meshes/wind.ase"));
}

GLWWindView::~GLWWindView()
{
	delete windModel_;
	if (glIsList(listNo_)) glDeleteLists(listNo_, 1);
}

void GLWWindView::draw()
{
	if (changeCount_ != Landscape::instance()->getChangeCount())
	{
		changeCount_ = Landscape::instance()->getChangeCount();
		if (glIsList(listNo_)) glDeleteLists(listNo_, 1);
		listNo_ = 0;
	}

	GLWVisibleWidget::draw();
	drawDisplay();
}

void GLWWindView::drawDisplay()
{
	Vector &lookFrom = MainCamera::instance()->getCamera().getCurrentPos();
	Vector &lookAt = MainCamera::instance()->getCamera().getLookAt();
	Vector dir = (lookAt - lookFrom).Normalize();

	float scale = 0.1f;
	float scale2 = MIN(w_, h_) / 90.0f;
	float angXY = atan2f(dir[0], dir[1]) / 3.14f * 180.0f;
	float angYZ = acosf(dir[2]) / 3.14f * 180.0f + 180.0f;
	if (angYZ < 280.0f) angYZ = 280.0f;

	GLState currentState(GLState::DEPTH_ON | GLState::TEXTURE_ON);
	glPushMatrix();
		glTranslatef(x_ + w_ / 2.0f, y_ + h_ / 2.0f, 0.0f);
		glRotatef(angYZ, 1.0f, 0.0f, 0.0f);
		glRotatef(angXY, 0.0f, 0.0f, 1.0f);

		// Draw the minature landscape
		glScalef(scale2, scale2, scale2);
		if (listNo_) glCallList(listNo_);
		else
		{
			glNewList(listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
				drawScene();
			glEndList();
		}

		// Draw the wind arrow for direction
		GLState texState(GLState::TEXTURE_OFF);
		if (ScorchedClient::instance()->getOptionsTransient().getWindOn())
		{
			glTranslatef(0.0f, 0.0f, 20.0f);
			glScalef(scale, scale, scale);

			glRotatef(-ScorchedClient::instance()->getOptionsTransient().getWindAngle(), 0.0f, 0.0f, 1.0f);
			drawArrow();
		}
	glPopMatrix();
}

void GLWWindView::drawScene()
{
	Landscape::instance()->getPlanTexture().draw(true);
	glColor3f(1.0f, 1.0f, 1.0f);

	HeightMap &hmp = ScorchedClient::instance()->getLandscapeMaps().getHMap();
	int sqSize = hmp.getWidth() / 16;
	const float squareSize = 30.0f;
	float heightPer = squareSize / float(hmp.getWidth()) * 2.0f;

	for (int y=0; y<=hmp.getWidth()-sqSize; y+=sqSize)
	{
		glBegin(GL_QUAD_STRIP);
		for (int x=0; x<=hmp.getWidth(); x+=sqSize)
		{
			float xPer = float(x) / float(hmp.getWidth());
			float yPer = float(y) / float(hmp.getWidth());
			float yPer2 = float(y+sqSize) / float(hmp.getWidth());

			float xPos = xPer * squareSize * 2 - squareSize;
			float yPos = yPer * squareSize * 2 - squareSize;
			float yPos2 = yPer2 * squareSize * 2 - squareSize;

			glTexCoord2f(xPer, yPer2);
			glVertex3f(xPos, yPos2, hmp.getHeight(x, y+sqSize) * heightPer);
			glTexCoord2f(xPer, yPer);
			glVertex3f(xPos, yPos, hmp.getHeight(x, y) * heightPer);
		}
		glEnd();
	}

	Landscape::instance()->getMagTexture().draw();
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-squareSize, -squareSize, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-squareSize, -squareSize, -10.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(squareSize, -squareSize, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(squareSize, -squareSize, -10.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(squareSize, squareSize, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(squareSize, squareSize, -10.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-squareSize, squareSize, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-squareSize, squareSize, -10.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-squareSize, -squareSize, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-squareSize, -squareSize, -10.0f);
	glEnd();
}

void GLWWindView::drawArrow()
{
	windModel_->draw();
}
