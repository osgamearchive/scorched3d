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

#include <math.h>
#include <limits.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLDynamicVertexArray.h>
#include <GLEXT/GLStateExtension.h>
#include <landscape/PatchSide.h>

PatchSide::PatchSide(HeightMap *hMap, PatchTexCoord *coord, 
	int left, int top, int width) : 
	triNodePool_(*TriNodePool::instance()),
	hMap_(hMap), coord_(coord),
	variance_(hMap, left, top, width),
	left_(left), top_(top), width_(width)
{

}

PatchSide::~PatchSide()
{

}

TriNode *PatchSide::getTriNode()
{
	return &baseTriNode_;
}

void PatchSide::reset()
{
	baseTriNode_.LeftChild = 0;
	baseTriNode_.RightChild = 0;
	baseTriNode_.LeftNeighbor = 0;
	baseTriNode_.RightNeighbor = 0;
}

void PatchSide::computeVariance()
{
	variance_.computeVariance();
}

void PatchSide::split(TriNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->LeftChild)
		return;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
	{
		split(tri->BaseNeighbor);
	}

	// Create children and link into mesh
	TriNode *cl = triNodePool_.getNextTriNode();
	TriNode *cr = triNodePool_.getNextTriNode();

	// If creation failed, just exit.
	if ( !cl || !cr) return;

	tri->RightChild = cr;
	tri->LeftChild  = cl;

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
	tri->LeftChild->LeftNeighbor  = tri->RightChild;

	tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
	tri->RightChild->RightNeighbor = tri->LeftChild;

	// Link our Left Neighbor to the new children
	if (tri->LeftNeighbor != NULL)
	{
		if (tri->LeftNeighbor->BaseNeighbor == tri)
			tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->LeftNeighbor == tri)
			tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->RightNeighbor == tri)
			tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (tri->RightNeighbor != NULL)
	{
		if (tri->RightNeighbor->BaseNeighbor == tri)
			tri->RightNeighbor->BaseNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->RightNeighbor == tri)
			tri->RightNeighbor->RightNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->LeftNeighbor == tri)
			tri->RightNeighbor->LeftNeighbor = tri->RightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (tri->BaseNeighbor != NULL)
	{
		if ( tri->BaseNeighbor->LeftChild )
		{
			tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
			tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
			tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
			tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
		}
		else
		{
				split( tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
		}
	}
	else
	{
		// An edge triangle, trivial case.
		tri->LeftChild->RightNeighbor = NULL;
		tri->RightChild->LeftNeighbor = NULL;
	}
}

void PatchSide::recursTessellate( TriNode *tri,
							  int leftX,  int leftY,
							  int rightX, int rightY,
							  int apexX,  int apexY,
							  int node, unsigned currentVariance )
{
	int centerX = (leftX + rightX) >> 1; // Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY) >> 1; // Compute Y coord.

	unsigned int triVariance = 100000;
	if ((unsigned int) node < variance_.getMaxPos() ) // If we are not below the variance tree
	{	
		triVariance = variance_.getVariance(node);	// Take variance into consideration
	}
	else
	{
		triVariance = 1;
	}

	if (triVariance >= currentVariance)
	{
		split(tri);														// Split this triangle.

		if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX - rightX) >= 2) || (abs(leftY - rightY) >= 2 )))	// Tessellate all the way down to one vertex per height field entry
		{
			recursTessellate( tri->LeftChild,   apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1 , currentVariance);
			recursTessellate( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1), currentVariance);
		}
	}
}

void PatchSide::tesselate(unsigned currentVariance)
{
	recursTessellate(	&baseTriNode_,
						left_ + width_,		top_ + width_,
						left_,				top_,
						left_ + width_,				top_,
						1, currentVariance);
}

void PatchSide::recursRender( TriNode *tri, 
							int leftX, int leftY, 
							int rightX, int rightY, 
							int apexX, int apexY, 
							int node, DrawType side)
{
	if ( tri->LeftChild )
	{
		int centerX = (leftX + rightX) >> 1;
		int centerY = (leftY + rightY) >> 1;

		recursRender( tri->LeftChild, apexX,   apexY, leftX, leftY, centerX, centerY, node<<1, side);
		recursRender( tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1), side);
	}
	else
	{
		GLfloat leftZ  = hMap_->getHeight(leftX, leftY);
		GLfloat rightZ = hMap_->getHeight(rightX, rightY);
		GLfloat apexZ  = hMap_->getHeight(apexX, apexY); 

		switch (side)
		{
		case typeTop:
			// Left
			GLDynamicVertexArray::instance()->addFloat((GLfloat) leftX);
			GLDynamicVertexArray::instance()->addFloat((GLfloat) leftY);
			GLDynamicVertexArray::instance()->addFloat(leftZ);
			GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(leftX).txa);
			GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(leftY).txa);
			if (GLStateExtension::hasMultiTex() &&
				GLStateExtension::getTextureUnits() > 2)
			{
				GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(leftX).txb);
				GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(leftY).txb);
			}

			// Right
			GLDynamicVertexArray::instance()->addFloat((GLfloat) rightX);
			GLDynamicVertexArray::instance()->addFloat((GLfloat) rightY);
			GLDynamicVertexArray::instance()->addFloat(rightZ);
			GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(rightX).txa);
			GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(rightY).txa);
			if (GLStateExtension::hasMultiTex() &&
				GLStateExtension::getTextureUnits() > 2)
			{
				GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(rightX).txb);
				GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(rightY).txb);
			}

			// Apex
			GLDynamicVertexArray::instance()->addFloat((GLfloat) apexX);
			GLDynamicVertexArray::instance()->addFloat((GLfloat) apexY);
			GLDynamicVertexArray::instance()->addFloat(apexZ);
			GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(apexX).txa);
			GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(apexY).txa);
			if (GLStateExtension::hasMultiTex() &&
				GLStateExtension::getTextureUnits() > 2)
			{
				GLDynamicVertexArray::instance()->addFloat(coord_->getWidthEntry(apexX).txb);
				GLDynamicVertexArray::instance()->addFloat(coord_->getHeightEntry(apexY).txb);
			}

			if (GLDynamicVertexArray::instance()->getSpace() < 100)
			{
				GLDynamicVertexArray::instance()->drawROAM();
			}
			break;
		case typeNormals:
		{
			Vector &leftNormal = hMap_->getNormal(leftX, leftY);
			Vector &rightNormal = hMap_->getNormal(rightX, rightY);
			Vector &apexNormal = hMap_->getNormal(apexX, apexY);

			glVertex3f((GLfloat) leftX, (GLfloat) leftY, (GLfloat) leftZ );
			glVertex3f((GLfloat) leftX + leftNormal[0] * 5.0f, 
					(GLfloat) leftY + leftNormal[1] * 5.0f, 
					(GLfloat) leftZ + leftNormal[2] * 5.0f);

			glVertex3f((GLfloat) rightX,(GLfloat) rightY,(GLfloat) rightZ );
			glVertex3f((GLfloat) rightX + rightNormal[0] * 5.0f, 
					(GLfloat) rightY + rightNormal[1] * 5.0f, 
					(GLfloat) rightZ + rightNormal[2] * 5.0f);

			glVertex3f((GLfloat) apexX, (GLfloat) apexY, (GLfloat) apexZ );
			glVertex3f((GLfloat) apexX + apexNormal[0] * 5.0f, 
					(GLfloat) apexY + apexNormal[1] * 5.0f, 
					(GLfloat) apexZ + apexNormal[2] * 5.0f);
		}
		break;
		}
	}
}

void PatchSide::draw(DrawType side)
{
	recursRender (	&baseTriNode_,
			left_ + width_,		top_ + width_,
			left_,				top_,	
			left_ + width_,				top_,
			1, side);
}
