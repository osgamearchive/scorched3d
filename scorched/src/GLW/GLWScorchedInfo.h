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

#if !defined(AFX_GLWScorchedInfo_H__E5E95ACF_A6F4_4552_B05C_75CA02ACB6D2__INCLUDED_)
#define AFX_GLWScorchedInfo_H__E5E95ACF_A6F4_4552_B05C_75CA02ACB6D2__INCLUDED_

#include <GLW/GLWVisibleWidget.h>
#include <XML/XMLParser.h>
#include <common/Vector.h>

class GLWScorchedInfo : public GLWVisibleWidget
{
public:
	enum InfoType
	{
		eNone,
		eWind,
		ePlayerName,
		eAutoDefenseCount,
		eParachuteCount,
		eHealthCount,
		eShieldCount,
		eFuelCount,
		eBatteryCount
	};

	GLWScorchedInfo(float x = 0.0f, float y = 0.0f, 
		float w = 0.0f, float h = 0.0f);
	virtual ~GLWScorchedInfo();

	virtual void draw();
	virtual bool initFromXML(XMLNode *node);


METACLASSID
protected:
	InfoType infoType_;
	float fontSize_;
	Vector fontColor_;
};

#endif // !defined(AFX_GLWScorchedInfo_H__E5E95ACF_A6F4_4552_B05C_75CA02ACB6D2__INCLUDED_)

