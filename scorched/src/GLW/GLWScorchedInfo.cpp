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
#include <GLW/GLWTranslate.h>
#include <client/ScorchedClient.h>
#include <tankgraph/TankModelRenderer.h>
#include <weapons/AccessoryStore.h>

REGISTER_CLASS_SOURCE(GLWHudCondition);

GLWHudCondition::GLWHudCondition()
{
}

GLWHudCondition::~GLWHudCondition()
{
}

bool GLWHudCondition::getResult(GLWidget *widget)
{
	return TankModelRendererHUD::drawText();
}

REGISTER_CLASS_SOURCE(GLWScorchedInfo);

GLWScorchedInfo::GLWScorchedInfo(float x, float y, float w, float h) : 
	GLWVisibleWidget(x, y, w, h), infoType_(eNone), noCenter_(false)
{

}

GLWScorchedInfo::~GLWScorchedInfo()
{

}

void GLWScorchedInfo::draw()
{
	Vector *fontColor = &fontColor_;
	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();
	if (inBox((float) mouseX - GLWTranslate::getPosX(), 
		(float) mouseY - GLWTranslate::getPosY(), x_, y_, w_, h_))
	{
		fontColor = &selectedColor_;
	}

	GLWVisibleWidget::draw();
	
	GLState state(GLState::TEXTURE_ON | GLState::DEPTH_OFF);
	
	// Items not relating to the current player
	switch(infoType_)
	{
		case eWind:
		{
			static char buffer[256];
			if (ScorchedClient::instance()->
				getOptionsTransient().getWindSpeed() == 0)
			{
				sprintf(buffer, "No Wind");
			}
			else
			{
				sprintf(buffer, "Force %.0f", 
					ScorchedClient::instance()->
					getOptionsTransient().getWindSpeed());
			}
			float windwidth = (float) GLWFont::instance()->
				getSmallPtFont()->getWidth(
				fontSize_, buffer);
			float offSet = 0.0f;
			if (!noCenter_) offSet = w_ / 2.0f - (windwidth / 2.0f);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_ + offSet, y_, 0.0f,
				buffer);                    
		}
		break;
	}
	
	// Get the current tank and model
	Tank *current = 
 		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!current ||
		current->getState().getState() != TankState::sNormal)
  	{
  		return;
	}
	TankModelRenderer *model = (TankModelRenderer *) 
		current->getModel().getModelIdRenderer();
	if (!model) return;

	Weapon *weapon = current->getAccessories().getWeapons().getCurrent();
	if (!weapon) return;

	// Items relating to the current player
	switch (infoType_)
	{
		case ePlayerName:
		{
			setToolTip(&model->getTips()->nameTip);
			float namewidth = (float) GLWFont::instance()->getSmallPtFont()->getWidth(
				fontSize_, current->getName());
			float offSet = 0.0f;
			if (!noCenter_) offSet = w_ / 2.0f - (namewidth / 2.0f);
			GLWFont::instance()->getSmallPtFont()->draw(
				current->getColor(), fontSize_,
				x_ + offSet, y_, 0.0f,
				current->getName());
		}
		break;
		case eAutoDefenseCount:
			setToolTip(&model->getTips()->autodTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				(current->getAccessories().getAutoDefense().haveDefense()?"On":"Off"));
		break;
		case eParachuteCount:
			setToolTip(&model->getTips()->paraTip);
			if (!current->getAccessories().getParachutes().parachutesEnabled())
			{
				GLWFont::instance()->getSmallPtFont()->draw(
					*fontColor, fontSize_,
					x_, y_, 0.0f,
					"Off");
			}
			else
			{
				int count = current->getAccessories().
					getParachutes().getNoParachutes();
				char buffer[128];
				if (count >= 0) sprintf(buffer, "%i", count);
				else sprintf(buffer, "In");
				GLWFont::instance()->getSmallPtFont()->draw(
					*fontColor, fontSize_,
					x_, y_, 0.0f,
					"%s", buffer);
			}
		break;
		case eHealthCount:
			setToolTip(&model->getTips()->healthTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%.0f",
				current->getState().getLife());
		break;
		case eShieldCount:
			setToolTip(&model->getTips()->shieldTip);
			if (!current->getAccessories().getShields().getCurrentShield())
			{
				GLWFont::instance()->getSmallPtFont()->draw(
					*fontColor, fontSize_,
					x_, y_, 0.0f,
					"Off");
			}
			else
			{
				GLWFont::instance()->getSmallPtFont()->draw(
					*fontColor, fontSize_,
					x_, y_, 0.0f,
					"%.0f",
					current->getAccessories().getShields().getShieldPower());
			}
		break;
		case eBatteryCount:
			setToolTip(&model->getTips()->batteryTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%i",
				current->getAccessories().getBatteries().getNoBatteries());
		break;
		case eFuelCount:
			setToolTip(&model->getTips()->fuelTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%i",
				current->getAccessories().getFuel().getNoFuel());
		break;
		case eWeaponName:
		{
			setToolTip(&model->getTips()->weaponTip);
			int count = current->getAccessories().getWeapons().getWeaponCount(
				current->getAccessories().getWeapons().getCurrent());
			static char buffer[256];
			const char *format = "%s (%i)";
			if (count < 0) format = "%s (In)";
			sprintf(buffer, format, 
				current->getAccessories().getWeapons().getCurrent()->getName(),
				count);
			float weaponWidth = (float) GLWFont::instance()->getSmallPtFont()->
				getWidth(fontSize_, buffer);

			float offSet = 0.0f;
			if (!noCenter_) offSet = w_ / 2.0f - (weaponWidth / 2.0f);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_ + offSet, y_, 0.0f,
				buffer);
		}
		break;
		case eWeaponCount:
		{
			setToolTip(&model->getTips()->weaponTip);
			int count = current->getAccessories().getWeapons().getWeaponCount(
				current->getAccessories().getWeapons().getCurrent());
			const char *format = "%i";
			if (count < 0) format = "In";
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				format,
				count);
		}
		break;
		case eRotation:
			setToolTip(&model->getTips()->rotationTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%.1f",
				360.0f - current->getPhysics().getRotationGunXY());
		break;
		case eRotationDiff:
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%+.1f",
				current->getPhysics().getRotationXYDiff());
		break;
		case eElevation:
			setToolTip(&model->getTips()->elevationTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%.1f",
				current->getPhysics().getRotationGunYZ());
		break;
		case eElevationDiff:
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%+.1f",
				current->getPhysics().getRotationYZDiff());
		break;
		case ePower:
			setToolTip(&model->getTips()->powerTip);
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%.1f",
				current->getPhysics().getPower());
		break;
		case ePowerDiff:
			GLWFont::instance()->getSmallPtFont()->draw(
				*fontColor, fontSize_,
				x_, y_, 0.0f,
				"%+.1f",
				current->getPhysics().getPowerDiff());
		break;
	}
}

void GLWScorchedInfo::mouseDown(float x, float y, bool &skipRest)
{
	Tank *current = 
 		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!current ||
		current->getState().getState() != TankState::sNormal)
  	{
  		return;
	}
	TankModelRenderer *model = (TankModelRenderer *) 
		current->getModel().getModelIdRenderer();
	if (!model) return;
	GLWTankTips *tankTips = model->getTips();

	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;

		switch (infoType_)
		{
			case ePlayerName:
			break;
			case eAutoDefenseCount:
				tankTips->autodTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eParachuteCount:
				tankTips->paraTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eHealthCount:
			break;
			case eShieldCount:
				tankTips->shieldTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eBatteryCount:
				tankTips->batteryTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eFuelCount:
				tankTips->fuelTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eWeaponName:
				tankTips->weaponTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eWeaponCount:
				tankTips->weaponTip.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eRotation:
				tankTips->undoMenu.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case eElevation:
				tankTips->undoMenu.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
			case ePower:
				tankTips->undoMenu.showItems(GLWTranslate::getPosX() + x, 
					GLWTranslate::getPosY() + y);
			break;
		}
	}
	else
	{
		GLWVisibleWidget::mouseDown(x, y, skipRest);
	}
}

bool GLWScorchedInfo::initFromXML(XMLNode *node)
{
	if (!GLWVisibleWidget::initFromXML(node)) return false;

	// Type node
	XMLNode *typeNode = node->getNamedChild("type", true);
	if (!typeNode) return false;
	if (0 == strcmp(typeNode->getContent(), "wind")) infoType_ = eWind;
	else if (0 == strcmp(typeNode->getContent(), "playername")) infoType_ = ePlayerName;
	else if (0 == strcmp(typeNode->getContent(), "autodefensecount")) infoType_ = eAutoDefenseCount;
	else if (0 == strcmp(typeNode->getContent(), "parachutecount")) infoType_ = eParachuteCount;
	else if (0 == strcmp(typeNode->getContent(), "shieldcount")) infoType_ = eShieldCount;
	else if (0 == strcmp(typeNode->getContent(), "healthcount")) infoType_ = eHealthCount;
	else if (0 == strcmp(typeNode->getContent(), "fuelcount")) infoType_ = eFuelCount;
	else if (0 == strcmp(typeNode->getContent(), "batterycount")) infoType_ = eBatteryCount;
	else if (0 == strcmp(typeNode->getContent(), "weaponname")) infoType_ = eWeaponName;
	else if (0 == strcmp(typeNode->getContent(), "weaponcount")) infoType_ = eWeaponCount;
	else if (0 == strcmp(typeNode->getContent(), "rotation")) infoType_ = eRotation;
	else if (0 == strcmp(typeNode->getContent(), "elevation")) infoType_ = eElevation;
	else if (0 == strcmp(typeNode->getContent(), "power")) infoType_ = ePower;
	else if (0 == strcmp(typeNode->getContent(), "rotationdiff")) infoType_ = eRotationDiff;
	else if (0 == strcmp(typeNode->getContent(), "elevationdiff")) infoType_ = eElevationDiff;
	else if (0 == strcmp(typeNode->getContent(), "powerdiff")) infoType_ = ePowerDiff;
	else
	{
		dialogMessage("GLWScorchedInfo",
			"Unknown info type \"%s\"",
			typeNode->getContent());
		return false;
	}
	
	// Font Size
	if ((fontSize_ = node->getNamedFloatChild("fontsize", true)) 
		== XMLNode::ErrorFloat) return false;
	
	// Font Color
	if ((fontColor_[0] = node->getNamedFloatChild("fontcolorr", true)) 
		== XMLNode::ErrorFloat) return false;
	if ((fontColor_[1] = node->getNamedFloatChild("fontcolorg", true)) 
		== XMLNode::ErrorFloat) return false;
	if ((fontColor_[2] = node->getNamedFloatChild("fontcolorb", true)) 
		== XMLNode::ErrorFloat) return false;

	// Font Selected Size
	if ((selectedColor_[0] = node->getNamedFloatChild("selfontcolorr", false)) 
		== XMLNode::ErrorFloat) selectedColor_[0] = fontColor_[0];
	if ((selectedColor_[1] = node->getNamedFloatChild("selfontcolorg", false)) 
		== XMLNode::ErrorFloat) selectedColor_[1] = fontColor_[1];
	if ((selectedColor_[2] = node->getNamedFloatChild("selfontcolorb", false)) 
		== XMLNode::ErrorFloat) selectedColor_[2] = fontColor_[2];

	// No Center
	XMLNode *centerNode = node->getNamedChild("nocenter");
	if (centerNode)
	{
		noCenter_ = (0 == strcmp(centerNode->getContent(), "true"));
	}

	return true;
}
