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


#include <tankgraph/GLWTankViewer.h>
#include <tankgraph/TankModelStore.h>
#include <GLW/GLWFont.h>

GLWTankViewer::GLWTankViewer(float x, float y) :
	GLWVisibleWidget(x, y, 80.0f, 200.0f),
	scrollBar_(w_ - 6.0f, y + 2.0f, h_ - 4.0f, 0, 0, 3),
	infoWindow_(x + 90.0f, y + 50.0f, 150.0f, 150.0f, true),
	rot_(0.0f), selected_(0),
	rotXY_(0.0f), rotYZ_(0.0f), 
	rotXYD_(1.0f), rotYZD_(1.0f)
{
	scrollBar_.setMax(
		int(TankModelStore::instance()->getModels().size()));
	scrollBar_.setCurrent(
		int(TankModelStore::instance()->getModels().size()));
}

GLWTankViewer::~GLWTankViewer()
{

}

const char *GLWTankViewer::getModelName()
{
    std::vector<TankModel *> &models = 
		TankModelStore::instance()->getModels();
	const char *name = "None";
	if (models.size())
	{
		name = models[selected_]->getId().getModelName();
	}
	return name;
}

void GLWTankViewer::simulate(float frameTime)
{
	rot_ += frameTime * 45.0f;
	rotXY_ += frameTime * rotXYD_ * 5.0f;
	rotYZ_ += frameTime * rotYZD_ * 5.0f;
	if (rotXY_ < -45.0f || rotXY_ > 45.0f)
	{
		rotXYD_ *= -1.0f;
	}
	if (rotYZ_ < 0.0f || rotYZ_ > 45.0f)
	{
		rotYZD_ *= -1.0f;
	}
}

void GLWTankViewer::draw()
{
	glBegin(GL_LINES);
		drawBox(x_, y_, w_, h_, false);
	glEnd();

	infoWindow_.draw();

	scrollBar_.draw();

	std::vector<TankModel *> &models = 
		TankModelStore::instance()->getModels();

	GLState depthState(GLState::DEPTH_ON);
	const float heightdiv = (h_ / 3.0f);
	const float widthdiv = ((w_ - 15.0f) / 2.0f);
	for (int pos = 0; pos < 3; pos ++)
	{
		int vectorPos = pos + scrollBar_.getCurrent() - 3;
		if (vectorPos < (int) models.size() &&
			vectorPos >= 0)
		{
			vectorPos = ((int) models.size()-1) - vectorPos;
			float posX = x_ +  widthdiv;
			float posY = y_ + heightdiv * pos + (heightdiv / 2.0f);

			bool currselected = (vectorPos == selected_);
			if (currselected)
			{
				const float squareSize = 30.0f;
				glColor3f(0.4f, 0.4f, 0.6f);
				glBegin(GL_LINE_LOOP);
					glVertex2f(posX - squareSize, posY - squareSize);
					glVertex2f(posX + squareSize, posY - squareSize);
					glVertex2f(posX + squareSize, posY + squareSize);
					glVertex2f(posX - squareSize, posY + squareSize);
				glEnd();
			}

			glPushMatrix();
				glTranslatef(posX, posY - 5.0f, 0.0f);

				glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
				if (currselected) glRotatef(rot_, 0.0f, 0.0f, 1.0f);
				glScalef(22.0f, 22.0f, 22.0f);

				drawItem(vectorPos, currselected);
			glPopMatrix();
		}
	}

	const float infoX = infoWindow_.getX() + (infoWindow_.getW() / 2.0f);
	const float infoY = infoWindow_.getY() + (infoWindow_.getH() / 2.0f) - 15.0f;
	glPushMatrix();
		glTranslatef(infoX, infoY, 0.0f);
		drawCaption(selected_);
		glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(rot_, 0.0f, 0.0f, 1.0f);
		glScalef(50.0f, 50.0f, 50.0f);
	
		drawItem(selected_, true);
	glPopMatrix();
}

void GLWTankViewer::mouseDown(float x, float y, bool &skipRest)
{
	scrollBar_.mouseDown(x, y, skipRest);
	if (!skipRest)
	{
		if (inBox(x, y, x_, y_, w_, h_))
		{
			int pos = int((y - y_) / (h_ / 3.0f));
			int vectorPos = (2 - pos) + (scrollBar_.getMax() - scrollBar_.getCurrent());

			std::vector<TankModel *> &models = 
				TankModelStore::instance()->getModels();

			if (vectorPos < (int) models.size() &&
				vectorPos >= 0)
			{
				selected_ = vectorPos;
			}
		}
	}
}

void GLWTankViewer::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	scrollBar_.mouseDrag(mx, my, x, y, skipRest);
}

void GLWTankViewer::mouseUp(float x, float y, bool &skipRest)
{
	scrollBar_.mouseUp(x, y, skipRest);
}

void GLWTankViewer::drawCaption(int pos)
{
	GLState state(GLState::DEPTH_OFF);

	std::vector<TankModel *> &models = 
		TankModelStore::instance()->getModels();

	Vector color(0.7f, 0.3f, 0.3f);
	GLWFont::instance()->getFont()->
		drawLen(22, color, 10.0f, -73.0f, 75.0f, 0.0f, 
			 models[pos]->getId().getModelName());
	GLWFont::instance()->getFont()->
		drawLen(22, color, 10.0f, -73.0f, 65.0f, 0.0f, 
			"(%i Tris)", models[pos]->getNoTris());
}
void GLWTankViewer::drawItem(int pos, bool selected)
{
	std::vector<TankModel *> &models = 
		TankModelStore::instance()->getModels();

	// Tank
	Vector tankPos;
	if (selected)
	{
		models[pos]->draw(false, 0.0f, tankPos, 0.0f, rotXY_, rotYZ_);
	}
	else
	{
		models[pos]->draw(false, 0.0f, tankPos, 0.0f, 45.0f, 45.0f);
	}

	// Ground
	GLState state(GLState::TEXTURE_OFF);
	glColor3f(181.0f / 255.0f, 204.0f / 255.0f, 237.0f / 255.0f);
	glBegin(GL_QUADS);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
	glEnd();

	//Shadow
	/*{
		glDepthMask(GL_FALSE);
		glPushMatrix();
			glScalef(1.0f, 1.0f, 0.0f);
			models[pos]->draw(false, tankPos, 0.0f, rotXY_, rotYZ_);
		glPopMatrix();
		glDepthMask(GL_TRUE);
	}*/
}

void GLWTankViewer::selectModelByName(const char *name)
{
	std::vector<TankModel *> &models = 
		TankModelStore::instance()->getModels();

	DIALOG_ASSERT(models.size());

	int currentSel = 0;
	std::vector<TankModel *>::iterator itor;
	for (itor = models.begin();
		 itor != models.end();
		 itor++, currentSel ++)
	{
		TankModel *current = (*itor);
		if (0 == strcmp(current->getId().getModelName(), name))
		{
			selected_ = currentSel;
			return;
		}
	}
}
