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


// Patch.cpp: implementation of the Patch class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <landscape/Patch.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Patch::Patch(HeightMap *map, int x, int y, int width, int totalWidth) :
	map_(map), x_(x), y_(y), width_(width),
	leftSide_(map, x, y, width, totalWidth),
	rightSide_(map, x+width, y+width, -width, totalWidth),
	forceVariance_(false), recalculate_(false)
{

}

Patch::~Patch()
{

}

void Patch::reset()
{
	leftSide_.reset();
	rightSide_.reset();

	leftSide_.getTriNode()->BaseNeighbor = rightSide_.getTriNode();
	rightSide_.getTriNode()->BaseNeighbor = leftSide_.getTriNode();
}

void Patch::computeVariance()
{
	leftSide_.computeVariance();
	rightSide_.computeVariance();
}

void Patch::tessalate(unsigned currentVariance)
{
	leftSide_.tesselate(currentVariance);
	rightSide_.tesselate(currentVariance);
}

void Patch::draw(PatchSide::DrawType side)
{
	leftSide_.draw(side);
	rightSide_.draw(side);
}

TriNode *Patch::getLeftTri()
{
	return leftSide_.getTriNode();
}

TriNode *Patch::getRightTri()
{
	return rightSide_.getTriNode();
}
