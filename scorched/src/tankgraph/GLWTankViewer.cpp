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
#include <3dsparse/ModelStore.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWTranslate.h>

static const float TankSquareSize = 60.0f;
static const float TankHalfSquareSize = TankSquareSize / 2.0f;
static const float TankPadding = 20.0f;
static const float TankInfo = 165.0f;

GLWTankViewer::GLWTankViewer(float x, float y, int numH, int numV) :
	GLWidget(x, y, 
					 TankSquareSize * numH + TankPadding, 
					 TankSquareSize * numV + TankPadding),
	scrollBar_(w_ - 12.0f, y + 2.0f, h_ - 4.0f, 0, 0, numV),
	infoWindow_(x + TankSquareSize * numH + TankPadding + 10.0f, 
				y + TankSquareSize * numV + TankPadding - TankInfo - 35.0f, 
				TankInfo, TankInfo, true),
	numH_(numH), numV_(numV),
	rot_(0.0f), selected_(0),
	rotXY_(0.0f), rotYZ_(0.0f), 
	rotXYD_(1.0f), rotYZD_(1.0f),
	team_(0),
	catagoryChoice_(x + TankSquareSize * numH + TankPadding + 10.0f, 
					y + TankSquareSize * numV + TankPadding - 25.0f,
					TankInfo)
{
	std::set<std::string> &catagories = 
		ScorchedClient::instance()->getTankModels().getModelCatagories();
	std::set<std::string>::iterator catItor;
	for (catItor = catagories.begin();
		 catItor != catagories.end();
		 catItor++)
	{
		catagoryChoice_.addText((*catItor).c_str());
	}

	catagoryChoice_.setHandler(this);
	catagoryChoice_.setText("All");
	select(0, 0, GLWSelectorEntry(catagoryChoice_.getText()));

	catagoryChoice_.setToolTip(new GLWTip("Model Catagory",
		"Displays the currently selected model catagory.\n"
		"To make models easier to locate\n"
		"tank models are grouped by catagory."));
	infoWindow_.setToolTip(new GLWTip("Current Model",
		"Displays the currently selected tank model.\n"
		"This is the model this player will use in game.\n"
		"Choose a new model from the selection on the\n"
		"left."));
}

GLWTankViewer::~GLWTankViewer()
{

}

void GLWTankViewer::setTeam(int team)
{
	team_ = team;
	select(0, 0, GLWSelectorEntry(catagoryChoice_.getText()));
}

void GLWTankViewer::select(unsigned int id, 
						   const int pos, 
						   GLWSelectorEntry value)
{
	std::vector<TankModel *> newmodels;

	std::vector<TankModel *> &models = 
		ScorchedClient::instance()->getTankModels().getModels();
	std::vector<TankModel *>::iterator modelItor;
	for (modelItor = models.begin();
		 modelItor != models.end();
		 modelItor++)
	{
		TankModel *tankModel = (*modelItor);
		if (tankModel->isOfCatagory(value.getText()))
		{
			// Check if this tank is allowed for this team
			if (!tankModel->isOfTeam(team_))
			{
				continue;
			}

			// Get the model file to determine if the file is too large
			if (OptionsDisplay::instance()->getTankDetail() != 2)
			{
				Model *model = ModelStore::instance()->loadModel(
					tankModel->getTankModelID());
				if (strcmp(tankModel->getId().getTankModelName(), "Random") != 0)
				{
					// Check if the model uses too many triangles
					int triangles = model->getNumberTriangles();
					if (OptionsDisplay::instance()->getTankDetail() == 0)
					{
						if (triangles > 250) continue;
					}
					else if (OptionsDisplay::instance()->getTankDetail() == 1)
					{
						if (triangles > 500) continue;
					}
				}
			}

			newmodels.push_back(tankModel);
		}
	}

	setTankModels(newmodels);
}

void GLWTankViewer::setTankModels(std::vector<TankModel *> &models)
{
	models_.clear();
	models_ = models;
	scrollBar_.setMax((int) (models_.size() / numH_) + 1);
	scrollBar_.setCurrent(0);
	selected_ = 0;

	// Sort the models 
	bool sorted = false;
	while (!sorted)
	{
		sorted = true;
		for (int i=0; i<(int) models_.size()-1; i++)
		{
			TankModel *tmp = models_[i];
			TankModel *test = models_[i+1];
			if (test->lessThan(tmp))
			{
				models_[i] = test;
				models_[i+1] = tmp;
				sorted = false;
			}
		}
	}

	// Move random model to the top
	std::vector<TankModel *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		TankModel *model = (*itor);
		if (0==strcmp(model->getId().getTankModelName(),"Random"))
		{
			models_.erase(itor);
			std::vector<TankModel *> tmpVector;
			tmpVector.push_back(model);
			tmpVector.insert(tmpVector.end(), models_.begin(), models_.end());
			models_ = tmpVector;
			break;
		}
	}
}

const char *GLWTankViewer::getModelName()
{
	const char *name = "None";
	if (!models_.empty())
	{
		name = models_[selected_]->getId().getTankModelName();
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

	scrollBar_.simulate(frameTime);
}

void GLWTankViewer::draw()
{
	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 6.0f, false);
	glEnd();

	infoWindow_.draw();
	scrollBar_.draw();

	GLState depthState(GLState::DEPTH_ON);
	const float heightdiv = (h_ / float(numV_));
	const float widthdiv = ((w_ - TankPadding - 10) / float(numH_));

	int pos = 0;
	for (int posV=0; posV<numV_; posV++)
	{
		for (int posH=0; posH<numH_; posH++, pos++)
		{
			int vectorPos = (scrollBar_.getCurrent() * numH_) + pos;
			if (vectorPos < (int) models_.size() &&
				vectorPos >= 0)
			{
				float posX = x_ + widthdiv * posH + TankHalfSquareSize + 2.0f;
				float posY = y_ + heightdiv * posV + TankHalfSquareSize + 2.0f;

				bool currselected = (vectorPos == selected_);
				if (currselected)
				{
					glColor3f(0.4f, 0.4f, 0.6f);
					glBegin(GL_LINE_LOOP);
						float SelectSize = TankHalfSquareSize - 2.0f;
						glVertex2f(posX - SelectSize, posY - SelectSize);
						glVertex2f(posX + SelectSize, posY - SelectSize);
						glVertex2f(posX + SelectSize, posY + SelectSize);
						glVertex2f(posX - SelectSize, posY + SelectSize);
					glEnd();
				}

				if (GLWToolTip::instance()->addToolTip(&toolTip_, 
					GLWTranslate::getPosX() + posX - TankHalfSquareSize, 
					GLWTranslate::getPosY() + posY - TankHalfSquareSize, 
					TankSquareSize,
					TankSquareSize))
				{
					toolTip_.setText(
						"Model",
						models_[vectorPos]->getId().getTankModelName());			
				}

				float scale = 22.0f / 60.0f * TankSquareSize;
				glPushMatrix();
					glTranslatef(posX, posY - 5.0f, 0.0f);

					glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
					if (currselected) glRotatef(rot_, 0.0f, 0.0f, 1.0f);
					glScalef(scale, scale, scale);

					drawItem(vectorPos, currselected);
				glPopMatrix();
			}
		}
	}

	if (selected_ >= 0 &&
		selected_ < (int) models_.size())
	{
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

	GLState depthStateOff(GLState::DEPTH_OFF);
	catagoryChoice_.draw();
}

void GLWTankViewer::mouseDown(float x, float y, bool &skipRest)
{
	scrollBar_.mouseDown(x, y, skipRest);
	if (!skipRest)
	{
		catagoryChoice_.mouseDown(x, y, skipRest);
		if (!skipRest)
		{
			if (inBox(x, y, x_, y_, w_, h_))
			{
				int posY = int((y - y_) / (h_ / numV_));
				int posX = int((x - x_) / (w_ / numH_));
				
				int vectorPos = posX + posY * numH_ + scrollBar_.getCurrent() * numH_;
				if (vectorPos < (int) models_.size() &&
					vectorPos >= 0)
				{
					selected_ = vectorPos;
				}
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

	Vector color(0.3f, 0.3f, 0.3f);
	GLWFont::instance()->getSmallPtFont()->
		drawWidth(int(TankSquareSize * 2 + TankPadding), 
			color, 10.0f, -70.0f, 75.0f, 0.0f, 
			models_[pos]->getId().getTankModelName());
	GLWFont::instance()->getSmallPtFont()->
		drawWidth(int(TankSquareSize * 2 + TankPadding), 
			color, 10.0f, -70.0f, 63.0f, 0.0f, 
			"(%i Triangles)", models_[pos]->getNoTris());
}
void GLWTankViewer::drawItem(int pos, bool selected)
{
	// Tank
	Vector tankPos;
	if (selected)
	{
		models_[pos]->draw(false, 0.0f, tankPos, 0.0f, rotXY_, rotYZ_);
	}
	else
	{
		models_[pos]->draw(false, 0.0f, tankPos, 0.0f, 45.0f, 45.0f);
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
	DIALOG_ASSERT(models_.size());

	// Ensure that all models have been loaded
	//select(0, 0, "All");

	// Select the appropriate model
	int currentSel = 0;
	std::vector<TankModel *>::iterator itor;
	for (itor = models_.begin();
		 itor != models_.end();
		 itor++, currentSel ++)
	{
		TankModel *current = (*itor);
		if (0 == strcmp(current->getId().getTankModelName(), name))
		{
			selected_ = currentSel;
			return;
		}
	}
}
