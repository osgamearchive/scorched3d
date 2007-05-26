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

#include <water/Water2PatchVisibility.h>
#include <water/Water2Patches.h>
#include <graph/OptionsDisplay.h>
#include <landscape/Landscape.h>
#include <landscape/Sky.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLState.h>

#include <water/Water2Constants.h>

const unsigned int Water2PatchVisibility::NotVisible = 9999;

Water2PatchVisibility::VisibilityEntry::VisibilityEntry() :
	visibilityIndex(Water2PatchVisibility::NotVisible)
{
}

Water2PatchVisibility::Water2PatchVisibility() : visibility_(0), size_(0)
{
}

Water2PatchVisibility::~Water2PatchVisibility()
{
	delete [] visibility_;
}

void Water2PatchVisibility::generate(Vector &offset, unsigned int totalSize, 
	unsigned int patchesSize, unsigned int patchSize)
{
	size_ = totalSize / patchSize;
	if (!visibility_) visibility_ = new VisibilityEntry[size_ * size_];

	Vector middle(128.0f, 128.0f);

	int i=0;
	for (int y=0; y<size_; y++)
	{
		for (int x=0; x<size_; x++, i++)
		{
			VisibilityEntry &entry = visibility_[i];

			entry.position = Vector(
				float(x) * 32.0f + 16.0f + offset[0],
				float(y) * 32.0f + 16.0f + offset[1],
				5.0f + offset[2]);
			entry.offset = Vector(
				256.0f * (x / (patchesSize / patchSize)) + offset[0],
				256.0f * (y / (patchesSize / patchSize)) + offset[1],
				offset[2]);
			entry.ignore = (((middle - entry.offset).Magnitude()) > 1700.0f);
		}
	}
}

void Water2PatchVisibility::draw(Water2Patches &patches,
	Water2PatchIndexs &indexes, Vector &cameraPosition,
	int vattr_aof_index)
{
	// Figure the visibility for all patches
	int i=0;
	for (int y=0; y<size_; y++)
	{
		for (int x=0; x<size_; x++, i++)
		{
			VisibilityEntry &entry = visibility_[i];
			if (entry.ignore)
			{
				entry.visibilityIndex = indexes.getNoPositions() - 1;
				entry.onScreen = false;
				continue;
			}

			Vector &position = entry.position;
			int index = indexes.getNoPositions() - 1;
			if (!OptionsDisplay::instance()->getNoWaterMovement())
			{
				float distance = (cameraPosition - position).Magnitude();
				index = int(distance - 50.0f) / 70;
				index = MAX(0, MIN(index, indexes.getNoPositions() - 1));
			}

			entry.onScreen = GLCameraFrustum::instance()->sphereInFrustum(position, 32.0f);
			entry.visibilityIndex = index;
		}
	}

	// Draw all patches
	i = 0;
	for (int y=0; y<size_; y++)
	{
		for (int x=0; x<size_; x++, i++)
		{
			VisibilityEntry &entry = visibility_[i];
			if (!entry.onScreen) continue;

			unsigned int index = entry.visibilityIndex;
			if (index == NotVisible) continue;

			unsigned int borders = 0;
			unsigned int leftIndex = getVisibility(x - 1, y);
			unsigned int rightIndex = getVisibility(x + 1, y);
			unsigned int topIndex = getVisibility(x, y + 1);
			unsigned int bottomIndex = getVisibility(x, y - 1);

			if (leftIndex != NotVisible && leftIndex > index) 
				borders |= Water2PatchIndex::BorderLeft;
			if (rightIndex != NotVisible && rightIndex > index) 
				borders |= Water2PatchIndex::BorderRight;
			if (topIndex != NotVisible && topIndex > index) 
				borders |= Water2PatchIndex::BorderTop;
			if (bottomIndex != NotVisible && bottomIndex > index) 
				borders |= Water2PatchIndex::BorderBottom;

			glPushMatrix();
				glTranslatef(entry.offset[0], entry.offset[1], entry.offset[2]);

				// Set lighting position
				Landscape::instance()->getSky().getSun().setLightPosition(true);


				Vector transl(
					myfmod(cameraPosition[0], float(128)),
					myfmod(cameraPosition[1], float(128)));
				glActiveTexture(GL_TEXTURE0);
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				const float noisetilescale = 1.0f/32.0f;//meters (128/16=8, 8tex/m).
				glScalef(noisetilescale, noisetilescale, 1.0f);	// fixme adjust scale
				glTranslatef(transl[0], transl[1], 0);
				glMatrixMode(GL_MODELVIEW);
				glActiveTexture(GL_TEXTURE1);


				// get projection and modelview matrix
				static float proj[16], model[16];
				glGetFloatv(GL_PROJECTION_MATRIX, proj);
				glGetFloatv(GL_MODELVIEW_MATRIX, model);

				// Setup texture coords
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				// rescale coordinates [-1,1] to [0,1]
				glTranslatef(0.5f,0.5f,0.0f);
				glScalef(0.5f,0.5f,1.0f);
				glMultMatrixf(proj);
				glMultMatrixf(model);
				glMatrixMode(GL_MODELVIEW);
				glActiveTexture(GL_TEXTURE0);

				Water2Patch *patch = patches.getPatch(
					x % patches.getSize(), y % patches.getSize());
				patch->draw(indexes, index, borders, vattr_aof_index);
			glPopMatrix();
		}
	}
}
