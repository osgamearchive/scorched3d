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

#include <landscape/WaterMapPoints.h>
#include <landscape/MapPoints.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <client/ScorchedClient.h>
#include <common/OptionsTransient.h>

WaterMapPoints::WaterMapPoints()
{
}

WaterMapPoints::~WaterMapPoints()
{
}

void WaterMapPoints::draw()
{
	GLState currentState(GLState::TEXTURE_OFF);
	for (int i=0; i<(int) pts_.size(); i++)
	{
		Position *current = &pts_[i];
		glPushMatrix();
			glTranslatef(current->x, current->y, 
				current->entry->height + 0.6f);
			glRotatef(current->entry->normal[0] * 90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(current->entry->normal[2] * 90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.15f, 0.15f, 0.15f);
			switch(ScorchedClient::instance()->getOptionsTransient().getWallType())
			{
			case OptionsTransient::wallWrapAround:
				MapPoints::instance()->getBorderModelWrap()->draw();
				break;
			case OptionsTransient::wallBouncy:
				MapPoints::instance()->getBorderModelBounce()->draw();
				break;
			case OptionsTransient::wallConcrete:
				MapPoints::instance()->getBorderModelConcrete()->draw();
				break;
			default:
				break;
			}
		glPopMatrix();
	}
}

void WaterMapPoints::generate(WaterMap &map, int mapWidth, int mapHeight)
{
	int pointsWidth = mapWidth / 64; // One point every 64 units
	int pointsHeight = mapHeight / 64; // One point every 64 units

	pts_.clear();
	int i;
	for (i=0; i<pointsWidth; i++)
	{
		float pos = float(mapWidth) / float(pointsWidth-1) * float(i);

		findPoint(map, pos, 0.0f);
		findPoint(map, pos, float(mapHeight));
	}
	for (i=1; i<pointsHeight-1; i++)
	{
		float pos = float(mapHeight) / float(pointsHeight-1) * float(i);

		findPoint(map, 0.0f, pos);
		findPoint(map, float(mapWidth), pos);
	}
}

void WaterMapPoints::findPoint(WaterMap &map, float x, float y)
{
	Position pos;
	Vector point(x, y);
	pos.x = x;
	pos.y = y;
	pos.entry = &map.getNearestWaterPoint(point);
	pts_.push_back(pos);
}
