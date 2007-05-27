////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <water/Water2Patch.h>
#include <water/Water2Constants.h>
#include <common/Logger.h>
#include <graph/OptionsDisplay.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLStateExtension.h>

Water2Patch::Water2Patch() : data_(0), size_(0)
{
}

Water2Patch::~Water2Patch()
{
	delete [] data_;
}

static Vector getPosition(std::vector<Vector> &heights, 
	int x, int y, int startX, int startY, int totalSize)
{
	int currentX = startX + x;
	int currentY = startY + y;
	int currentXNormalized = currentX % totalSize;
	int currentYNormalized = currentY % totalSize;
	if (currentXNormalized < 0) currentXNormalized += totalSize;
	if (currentYNormalized < 0) currentYNormalized += totalSize;

	int position = currentXNormalized + currentYNormalized * totalSize;
	DIALOG_ASSERT(position >= 0 && position < totalSize * totalSize);

	Vector &height = heights[position];

	Vector result(
		height[0] + float(currentX) * 2.0f,
		height[1] + float(currentY) * 2.0f,
		height[2]);
	return result;
}

void Water2Patch::generate(std::vector<Vector> &heights,
	int size, int totalSize,
	int posX, int posY)
{
	size_ = size;
	if (!data_) data_ = new Data[(size + 1) * (size + 1)];

	int startX = posX * size;
	int startY = posY * size;

	Data *data = data_;
	for (int y=0; y<=size; y++)
	{
		for (int x=0; x<=size; x++, data++)
		{
			// Calculate the position
			Vector position = getPosition(heights, x, y, startX, startY, totalSize);

			// Set the position
			data->x = position[0];
			data->y = position[1];
			data->z = position[2];
			data->aof = 0.0f;
		}
	}

	data = data_;
	for (int y=0; y<=size; y++)
	{
		for (int x=0; x<=size; x++, data++)
		{
			// Calculate the normal
			Vector current = getPosition(heights, x, y, startX, startY, totalSize);
			Vector other1 = getPosition(heights, x + 1, y, startX, startY, totalSize);
			Vector other2 = getPosition(heights, x, y + 1, startX, startY, totalSize);
			Vector other3 = getPosition(heights, x - 1, y, startX, startY, totalSize);
			Vector other4 = getPosition(heights, x, y - 1, startX, startY, totalSize);

			Vector dir1 = other1 - current;
			Vector dir2 = other2 - current;
			Vector dir3 = other3 - current;
			Vector dir4 = other4 - current;

			Vector normal1 = dir1 * dir2;
			Vector normal2 = dir2 * dir3;
			Vector normal3 = dir3 * dir4;
			Vector normal4 = dir4 * dir1;

			Vector normal = (normal1 + normal2 + normal3 + normal4).Normalize();

			// Set the normal
			data->nx = normal[0];
			data->ny = normal[1];
			data->nz = normal[2];
		}
	}
}

void Water2Patch::draw(Water2PatchIndexs &indexes, int indexPosition, int borders, int vattr_aof_index)
{
	Water2PatchIndex &index = indexes.getIndex(indexPosition, borders);

	// Vertices On
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Data), &data_[0].x);

	// Normals On
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Data), &data_[0].nx);

	if (vattr_aof_index != 0)
	{
		// Aof On
		glEnableVertexAttribArray(vattr_aof_index);
		glVertexAttribPointer(vattr_aof_index, 1, GL_FLOAT, GL_FALSE, sizeof(Data), &data_[0].aof);
	}
	
	// Draw elements
	glDrawElements(GL_TRIANGLE_STRIP, 
		index.getSize(), 
		GL_UNSIGNED_INT, 
		index.getIndices());

	// Vertices Off
	glDisableClientState(GL_VERTEX_ARRAY);

	// Normals Off
	glDisableClientState(GL_NORMAL_ARRAY);

	if (vattr_aof_index != 0)
	{
		// Aof Off
		glDisableVertexAttribArray(vattr_aof_index);
	}

	if(OptionsDisplay::instance()->getDrawNormals()) // Draw normals
	{
		GLState state(GLState::TEXTURE_OFF);
		glBegin(GL_LINES);
		for (int i=0; i<index.getSize(); i++)
		{
			Data &data = data_[index.getIndices()[i]];
			glVertex3f(data.x, data.y, data.z);
			glVertex3f(data.x + data.nx * 4.0f,
				data.y + data.ny * 4.0f,
				data.z + data.nz * 4.0f);
		}
		glEnd();
	}
}

Water2Patch::Data *Water2Patch::getData(int x, int y)
{
	DIALOG_ASSERT(x >= 0 && y >= 0 &&
		x <= size_ && y <= size_);
	return &data_[x + y * (size_ + 1)];
}
