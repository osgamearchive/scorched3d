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


// PatchGrid.cpp: implementation of the PatchGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4786)

#include <math.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <client/MainCamera.h>
#include <tank/TankContainer.h>
#include <landscape/PatchVar.h>
#include <landscape/PatchGrid.h>
#include <landscape/TriNodePool.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PatchGrid::PatchGrid(HeightMap *hMap, int patchSize) :
	hMap_(hMap), lastPos_(-1, -2, -3), patchSize_(patchSize),
	simulationTime_(0.0f)
{
 	width_ = (hMap_->getWidth()+1) / patchSize;
	patches_ = new Patch*[width_ * width_];
	for (int y=0; y<width_; y++)
	{
		for (int x=0; x<width_; x++)
		{
			patches_[x + y * width_] = 
				new Patch(hMap, 
							x * patchSize, 
							y * patchSize, 
							patchSize, 
							hMap->getWidth() - 1);
		}
	}
}

PatchGrid::~PatchGrid()
{
	delete [] patches_;
}

void PatchGrid::simulate(float frameTime)
{
	const float SimulationTimeStep = 0.25f;

	simulationTime_ += frameTime;
	while (simulationTime_ > SimulationTimeStep)
	{
		simulationTime_ -= SimulationTimeStep;
		forceCalculate(3);
	}
}

void PatchGrid::forceCalculate(int threshold)
{
	unsigned tessCount = 0;
	Patch **patch = patches_;
	for (int p=0; p<width_ * width_; p++)
	{			
		if ((*patch)->getRecalculate())
		{
			tessCount++;
			(*patch)->getRecalculate() = false;
			(*patch)->computeVariance();
		}
		if (tessCount >= threshold) return;

		patch++;
	}
}

void PatchGrid::recalculate(int posX, int posY, int dist)
{
	// Make sure the next frame has everything reset
	// and recalculated
	lastPos_ = Vector(-1, -2, -3);

	// Recompute the variances for any patches that
	// may have changed
	Patch **patch = patches_;
	for (int p=0; p<width_ * width_; p++)
	{
		Patch *current = (*patch);

		int distX = abs(current->getX() - posX);
		int distY = abs(current->getY() - posY);

		if (distX < dist + current->getWidth() &&
			distY < dist + current->getWidth())
		{
			current->getRecalculate() = true;
			current->getForceVariance() = false;
		}

		patch++;
	}

	// Ensure that any patches with tanks on them are drawn with
	// a very low varaince
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *current = (*itor).second;
		if (current->getState().getState() == TankState::sNormal)
		{
			float posX = current->getPhysics().getTankPosition()[0];
			float posY = current->getPhysics().getTankPosition()[1];

			posX /= float(patchSize_);
			posY /= float(patchSize_);

			int posXI = (int) posX;
			int posYI = (int) posY;

			if (posXI < 0) posXI = 0;
			else if (posXI >= width_) posXI = width_ - 1;
			if (posYI < 0) posYI = 0;
			else if (posYI >= width_) posYI = width_ - 1;

			patches_[posXI + posYI * width_]->getForceVariance() = true;
		}
	}
}

void PatchGrid::reset()
{
	// Only need to recalculate patches if the viewing camera has changed
	// position
	Vector &pos = MainCamera::instance()->getCamera().getCurrentPos();
	if (pos == lastPos_) return;
	lastPos_ = pos;

	// Reset the triangles being used
	TriNodePool::instance()->reset();

	// Link all the patches together.
	Patch **patch = patches_;
	for (int y=0; y<width_; y++)
	{
		for (int x=0; x<width_; x++)
		{
			(*patch)->reset();
			
			if ( x < width_-1 ) (*patch)->getLeftTri()->LeftNeighbor = (*(patch+1))->getRightTri();
			else (*patch)->getLeftTri()->LeftNeighbor = 0;

			if ( x > 0 ) (*patch)->getRightTri()->LeftNeighbor = (*(patch-1))->getLeftTri();
			else (*patch)->getRightTri()->LeftNeighbor = 0;	

			if ( y > 0 ) (*patch)->getLeftTri()->RightNeighbor = (*(patch-width_))->getRightTri();
			else (*patch)->getLeftTri()->RightNeighbor = 0;

			if ( y < width_-1 ) (*patch)->getRightTri()->RightNeighbor = (*(patch+width_))->getLeftTri();
			else (*patch)->getRightTri()->RightNeighbor = 0;

			patch++;
		}
	}

	// Tessalate
	patch = patches_;
	for (int p=0; p<width_ * width_; p++)
	{
		static Vector point;
		point[0] = float((*patch)->getX() + ((*patch)->getWidth() / 2));
		point[1] = float((*patch)->getY() + ((*patch)->getWidth() / 2));
		point[2] = float(hMap_->getHeight((int) point[0], (int) point[1]));
		point -= pos;

		unsigned variance = 30;
		if ((*patch)->getForceVariance())
		{
			variance = 10;
		}
		float distance = point.Magnitude();
		if (distance > 60)
		{
			variance += unsigned ((distance - 60) * 1);
		}
		if (variance > 100) variance = 100;

		(*patch)->tessalate(variance);
		patch++;
	}
}

void PatchGrid::draw(PatchSide::DrawType sides)
{
	if (!PatchVar::var)
	{
		PatchVar::var = new GLVar(1000);
	}

	reset();

	GLCameraFrustum *frustum = GLCameraFrustum::instance();

	unsigned tessCount = 0;
	glColor3f(1.0f, 1.0f, 1.0f);
	switch(sides)
	{
	case PatchSide::typeTop:
	{
		Patch **patch = patches_;
		for (int p=0; p<width_ * width_; p++)
		{			
			static Vector point;
			point[0] = float((*patch)->getX() + ((*patch)->getWidth() / 2));
			point[1] = float((*patch)->getY() + ((*patch)->getWidth() / 2));
			point[2] = float(hMap_->getHeight((*patch)->getX() + ((*patch)->getWidth() / 2), 
				(*patch)->getY() + ((*patch)->getWidth() /2)));

			if (frustum->sphereInFrustum(point, (*patch)->getWidth() + 5.0f))
			{
				if ((*patch)->getRecalculate() &&
					tessCount < 0)
				{
					tessCount++;
					(*patch)->getRecalculate() = false;
					(*patch)->computeVariance();
				}

				(*patch)->draw(sides);
			}

			patch++;
		}
		PatchVar::var->draw();
	}
	break;
	case PatchSide::typeNormals:
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		Patch **patch = patches_;
		for (int p=0; p<width_ * width_; p++)
		{
			(*patch++)->draw(sides);
		}
		glEnd();
	}
	break;
	}
}
