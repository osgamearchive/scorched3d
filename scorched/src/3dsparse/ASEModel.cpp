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

#include <common/Defines.h>
#include <3dsparse/ASEModel.h>

ASEModel::ASEModel(char *name) : Model(name)
{

}

ASEModel::~ASEModel()
{

}

void ASEModel::setTmRow(Vector &row, int index)
{
	DIALOG_ASSERT(index < 3);
	tmRow_[index] = row.Normalize();
}

void ASEModel::setFaceNormal(Vector &normal, int face, int index)
{
	DIALOG_ASSERT(index < 3);
	DIALOG_ASSERT(face < (int) faces_.size());

	Vector n(normal[0] * tmRow_[0][0] + normal[1] * tmRow_[1][0] + normal[2] * tmRow_[2][0],
		normal[0] * tmRow_[0][1] + normal[1] * tmRow_[1][1] + normal[2] * tmRow_[2][1],
		normal[0] * tmRow_[0][2] + normal[1] * tmRow_[1][2] + normal[2] * tmRow_[2][2]);

	faces_[face].normal[index] = n;
}
