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


#include <landscape/CloudSim.h>
#include <GLEXT/GLState.h>

#define CELLS_X 64 //128
#define CELLS_Y 64 //128
#define CELLS_Z 16 //32
#define RETURN_CELL(x, y, z) cells_[((x)) + ((y) * CELLS_X) + \
	((z) * CELLS_X * CELLS_Y)];

CloudSim::CloudSim() : set_(Cell::SetA)
{
	const unsigned int cellSize = CELLS_X * CELLS_Y * CELLS_Z;
	cells_ = new Cell[cellSize];	


	Cell *current = &RETURN_CELL(32, 32, 8);
	current->setHum(set_);
	current = &RETURN_CELL(33, 32, 8);
	current->setHum(set_);
}

CloudSim::~CloudSim()
{
	delete [] cells_;
}

void CloudSim::simulate()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);

	set_ = ((set_==Cell::SetA)?Cell::SetB:Cell::SetA);
	int x, y, z;
	for (x=2; x<CELLS_X-2; x++)
	{
		for (y=2; y<CELLS_Y-2; y++)
		{
			for (z=2; z<CELLS_Z-2; z++)
			{
				Cell *current = &RETURN_CELL(x, y, z);
				
				Cell *xp1 = &RETURN_CELL(x+1, y,   z);
				Cell *yp1 = &RETURN_CELL(x,   y+1, z);
				Cell *zp1 = &RETURN_CELL(x  , y  , z+1);
				Cell *xm1 = &RETURN_CELL(x-1, y  , z);
				Cell *ym1 = &RETURN_CELL(x  , y-1, z);
				Cell *zm1 = &RETURN_CELL(x  , y  , z-1);
				Cell *xp2 = &RETURN_CELL(x+2, y  , z);
				Cell *yp2 = &RETURN_CELL(x,   y+2, z);
				Cell *xm2 = &RETURN_CELL(x-2, y  , z);
				Cell *ym2 = &RETURN_CELL(x,   y-2, z);
				Cell *zm2 = &RETURN_CELL(x,   y  , z-2);
				
				bool fnAct = 
					xp1->getPrevAct(set_) ||
					yp1->getPrevAct(set_) ||
					zp1->getPrevAct(set_) ||
					xm1->getPrevAct(set_) ||
					ym1->getPrevAct(set_) ||
					zm1->getPrevAct(set_) ||
					xp2->getPrevAct(set_) ||
					yp2->getPrevAct(set_) ||
					xm2->getPrevAct(set_) ||
					ym2->getPrevAct(set_) ||
					zm2->getPrevAct(set_);
				
				if (x == 32 && y == 32 && z == 8)
				{
					bool cloud = current->getCurrentCld(set_);
				}

				if (fnAct)
				{
					bool cloud = current->getCurrentCld(set_);
				}

				current->calcNextValue(set_, fnAct);

				bool cloud = current->getCurrentCld(set_);
				if (cloud)
				{
					glVertex3f(x * 4.0f, y * 4.0f, z * 4.0f + 16.0f);
				}
			}
		}
	}
	glEnd();
}
