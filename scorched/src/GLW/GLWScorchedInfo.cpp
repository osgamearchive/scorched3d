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

#include <GLW/GLWScorchedInfo.h>
#include <GLW/GLWFont.h>
#include <client/ScorchedClient.h>
#include <tankgraph/TankModelRenderer.h>

GLWScorchedInfo::GLWScorchedInfo(float x, float y, float w, float h) : 
	GLWVisibleWidget(x, y, w, h), infoType_(eNone)
{

}

GLWScorchedInfo::~GLWScorchedInfo()
{

}

void GLWScorchedInfo::draw()
{
	GLWVisibleWidget::draw();
	
	GLState state(GLState::TEXTURE_ON | GLState::DEPTH_OFF);
	
	// Items not relating to the current player
	switch(infoType_)
	{
		case eWind:
			GLWFont::instance()->getLargePtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%i",
				ScorchedClient::instance()->getOptionsTransient().getWindSpeed());                      
		break;
	}
	
	// Get the current tank and model
	Tank *current = 
 		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!current ||
		current->getState().getState() == TankState::sNormal)
  	{
  		return;
	}
	TankModelRenderer *model = (TankModelRenderer *) 
		current->getModel().getModelIdRenderer();
		if (!model) return;
		
	// Items relating to the current player
	switch (infoType_)
	{
		case ePlayerName:
		{
			setToolTip(&model->getTips()->nameTip);
			float namewidth = (float) GLWFont::instance()->getSmallPtFont()->getWidth(
				fontSize_, current->getName());
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_ + w_ / 2.0f - (namewidth / 2.0f), y_, 0.0f,
				current->getName());
		}
		break;
		case eAutoDefenseCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%i",
				(current->getAccessories().getAutoDefense().haveDefense()?1:0));
		break;
		case eParachuteCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%i",
				current->getAccessories().getParachutes().getNoParachutes());	
		break;
		case eHealthCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%.0f",
				current->getState().getLife());
		break;
		case eShieldCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%.0f",
				current->getAccessories().getShields().getShieldPower());
		break;
		case eBatteryCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%i",
				current->getAccessories().getBatteries().getNoBatteries());
		break;
		case eFuelCount:
			GLWFont::instance()->getSmallPtFont()->draw(
				fontColor_, fontSize_,
				x_, y_, 0.0f,
				"%i",
				current->getAccessories().getFuel().getNoFuel());
		break;
	}
}

bool GLWScorchedInfo::initFromXML(XMLNode *node)
{
	//if (!GLWVisibleWidget::initFromXML(node)) return false;

	// Type node
	XMLNode *typeNode = node->getNamedChild("type");
	if (!typeNode) return false;
	if (0 == strcmp(typeNode->getContent(), "wind")) infoType_ = eWind;
	else if (0 == strcmp(typeNode->getContent(), "playername")) infoType_ = ePlayerName;
	else if (0 == strcmp(typeNode->getContent(), "autodefensecount")) infoType_ = eAutoDefenseCount;
	else if (0 == strcmp(typeNode->getContent(), "parachutecount")) infoType_ = eParachuteCount;
	else if (0 == strcmp(typeNode->getContent(), "shieldcount")) infoType_ = eShieldCount;
	else if (0 == strcmp(typeNode->getContent(), "healthcount")) infoType_ = eHealthCount;
	else if (0 == strcmp(typeNode->getContent(), "fuelcount")) infoType_ = eFuelCount;
	else if (0 == strcmp(typeNode->getContent(), "batterycount")) infoType_ = eBatteryCount;
	else
	{
		dialogMessage("GLWScorchedInfo",
			"Unknown info type \"%s\"",
			typeNode->getContent());
		return false;
	}
	
	// Font Size
	if ((fontSize_ = node->getNamedFloatChild("fontsize", "")) 
		== 0.0f) return false;
	
	// Font Color
	if ((fontColor_[0] = node->getNamedFloatChild("fontcolorr", "")) 
		== 0.0f) return false;
	if ((fontColor_[1] = node->getNamedFloatChild("fontcolorg", "")) 
		== 0.0f) return false;
	if ((fontColor_[2] = node->getNamedFloatChild("fontcolorb", "")) 
		== 0.0f) return false;

	return true;
}

