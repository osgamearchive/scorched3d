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

#ifndef _gLWTankViewer_h
#define _gLWTankViewer_h

#include <GLW/GLWScrollW.h>
#include <GLW/GLWVisiblePanel.h>
#include <GLW/GLWDropDown.h>
#include <GLW/GLWToolTip.h>
#include <tankgraph/TankModel.h>
#include <vector>

class GLWTankViewer : public GLWVisibleWidget,
					  public GLWDropDownI
{
public:
	GLWTankViewer(float x = 0.0f, float y = 0.0f, 
		int numH = 0, int numV = 0);
	virtual ~GLWTankViewer();

	void selectModelByName(const char *name);
	const char *getModelName();

	void setTankModels(std::vector<TankModel *> &models);

	// Inhertied from GLWDropDownI
	virtual void select(unsigned int id, const int pos, GLWDropDownEntry value);

	// Inhertied from GLWidget
	virtual void draw();
	virtual void simulate(float frameTime);
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, 
						   bool &skipRest);

	REGISTER_CLASS_HEADER(GLWTankViewer);
protected:
	GLWDropDown catagoryChoice_;
	GLWScrollW scrollBar_;
	GLWVisiblePanel infoWindow_;
	GLWTip toolTip_;
	std::vector<TankModel *> models_;
	int numH_, numV_;
	float rot_;
	float rotXY_, rotYZ_;
	float rotXYD_, rotYZD_;
	int selected_;

	virtual void drawItem(int pos, bool selected);
	virtual void drawCaption(int pos);

private:
	GLWTankViewer(const GLWTankViewer &);
	const GLWTankViewer & operator=(const GLWTankViewer &);

};

#endif // _gLWTankViewer_h

