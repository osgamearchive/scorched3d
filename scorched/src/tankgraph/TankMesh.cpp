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
#include <tankgraph/TankMesh.h>
#include <3dsparse/ModelsFile.h>
#include <common/Defines.h> // For porting

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLuint TankMesh::sightList_ = 0;

TankMesh::TankMesh(ModelsFile &tank, bool useTextures, float detail) 
	: gunArray_(0), turretArray_(0), otherArrays_(0), turretHeight_(0.0f), 
	otherArraySize_(0), useTextures_(useTextures)
{
	createArrays(tank, useTextures, detail);
}

TankMesh::~TankMesh()
{
	delete gunArray_;
	delete turretArray_;
	if (otherArrays_)
	{
		for (int i=0; i<otherArraySize_; i++)
		{
			delete otherArrays_[i];
		}
		delete [] otherArrays_;
	}
}

int TankMesh::getNoTris()
{
	if (!gunArray_ || !turretArray_ || !otherArraySize_) return 0;

	int tris = gunArray_->getNoTris() + turretArray_->getNoTris();
	for (int i=0; i<otherArraySize_; i++)
	{
		tris += otherArrays_[i]->getNoTris();
	}
	return  tris;
}

void TankMesh::createArrays(ModelsFile &aseTank, bool useTextures, float detail)
{
	Model *turretModel = 0;
	Model *gunModel = 0;
	std::list<Model*> otherModels;

	// Find the gun and turret meshes from all of the mesh
	// parts used to create the tank
	// Keep them and use them to draw tank's gun
	Vector turretCenter;
	std::list<Model *>::iterator itor;
	for (itor = aseTank.getModels().begin();
		itor != aseTank.getModels().end();
		itor++)
	{
		const char *name = (*itor)->getName();
		if (stricmp(name, "\"Turret\"") == 0)
		{
			// Find the center that the tank should rotate around
			turretCenter = ((*itor)->getMax() + (*itor)->getMin()) / 2.0f;
			turretModel = *itor;
		}
		else if (stricmp(name, "\"Gun\"") == 0)
		{
			gunModel = *itor;
		}
		else
		{
			otherModels.push_back(*itor);
		}
	}	

	// Check we have a valid tank model
	if (!gunModel || !turretModel)
	{
		dialogMessage("Tank", "ERROR: Failed to find gun and turret from mesh");
		exit(1);
	}

	// Center all meshes around base of turret
	// Center gun around turret center
	// and place all of the meshes on the ground 
	Vector oldCenter = (aseTank.getMax() + aseTank.getMin()) / 2.0f;
	Vector newCenter = turretCenter;
	newCenter[2] = aseTank.getMin()[2];
	turretHeight_ = turretCenter[2] - aseTank.getMin()[2];
	for (itor = aseTank.getModels().begin();
		itor != aseTank.getModels().end();
		itor++)
	{
		const char *name = (*itor)->getName();
		if (stricmp(name, "\"Gun\"") == 0)
		{
			(*itor)->centre(turretCenter);
		}
		else
		{
			(*itor)->centre(newCenter);
		}
	}

	// Retreive actual vertex arrays from the meshes
	otherArrays_ = new GLVertexArray*[otherModels.size()];
	gunArray_ = gunModel->getArray(useTextures, detail);
	turretArray_ = turretModel->getArray(useTextures, detail);

	std::list<Model*>::iterator mitor;
	for (mitor = otherModels.begin();
		mitor != otherModels.end();
		mitor++)
	{
		otherArrays_[otherArraySize_++] = (*mitor)->getArray(useTextures, detail);
	}
}

void TankMesh::draw(bool drawS, float angle, Vector &position, 
					float fireOffset, float rotXY, float rotXZ)
{
	GLState *texState = 0;
	if (useTextures_) texState = new GLState(GLState::TEXTURE_ON);

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		for (int i=0; i<otherArraySize_; i++)
		{
			otherArrays_[i]->draw();
		}
		drawGun(drawS, fireOffset, rotXY, rotXZ);
	glPopMatrix();

	delete texState;
}

void TankMesh::drawGun(bool drawS, float fireOffSet, float rotXY, float rotXZ)
{
	// Rotate around z axis and then draw turret
	glRotatef(rotXY, 0.0f, 0.0f, 1.0f);
	if (turretArray_) turretArray_->draw();

	// Rotate around y axis and then draw gun
	glTranslatef(0.0f, 0.0f, turretHeight_);
	glRotatef(rotXZ, 1.0f, 0.0f, 0.0f);
	if (fireOffSet != 0.0f) glTranslatef(0.0f, fireOffSet, 0.0f);
	if (gunArray_) gunArray_->draw();

	// Draw the sight
	if (drawS) drawSight();
}

void TankMesh::drawSight()
{
	if (!sightList_)
	{
		glNewList(sightList_ = glGenLists(1), GL_COMPILE);
			glBegin(GL_QUAD_STRIP);
				float x;
				for (x=90.0f; x<135.0f; x+=9.0f)
				{
					const float deg = 3.14f / 180.0f;
					float dx = x * deg;

					glColor4f(1.0f, 0.5f, 0.5f, 1.0f - fabsf(90.0f - x) / 45.0f);
					glVertex3f(0.0f, 10.0f * sinf(dx), 10.0f * cosf(dx));
					glVertex3f(0.0f, 2.0f * sinf(dx), 2.0f * cosf(dx));
				}
			glEnd();
			glBegin(GL_QUAD_STRIP);
				for (x=90.0f; x<135.0f; x+=9.0f)
				{
					const float deg = 3.14f / 180.0f;
					float dx = x * deg;

					glColor4f(1.0f, 0.5f, 0.5f, 1.0f - fabsf(90.0f - x) / 45.0f);
					glVertex3f(0.0f, 2.0f * sinf(dx), 2.0f * cosf(dx));
					glVertex3f(0.0f, 10.0f * sinf(dx), 10.0f * cosf(dx));
				}
			glEnd();
		glEndList();
	}

	GLState currentState(GLState::BLEND_ON | GLState::TEXTURE_OFF);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glCallList(sightList_);
}
