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
#include <common/OptionsDisplay.h>
#include <common/Defines.h> // For porting

GLuint TankMesh::sightList_ = 0;

TankMesh::TankMesh(ModelsFile &tank, bool useTextures, float detail) 
	: turretHeight_(0.0f), useTextures_(useTextures)
{
	createArrays(tank, useTextures, detail);
}

TankMesh::~TankMesh()
{
	gunArrays_.destroyGroup();
	turretArrays_.destroyGroup();
	otherArrays_.destroyGroup();
}

int TankMesh::getNoTris()
{
	return 
		gunArrays_.getNoTris() + 
		turretArrays_.getNoTris() +
		otherArrays_.getNoTris();
}

void TankMesh::createArrays(ModelsFile &aseTank, bool useTextures, float detail)
{
	// Make sure the tank is not too large
	const float maxSize = 3.0f;
	float size = (aseTank.getMax() - aseTank.getMin()).Magnitude();
	if (size > maxSize)
	{
		const float sfactor = 2.2f / size;
		aseTank.scale(sfactor);
	}
	aseTank.centre();
	modelCenter_ = (aseTank.getMax() + aseTank.getMin()) / 2.0f;

	std::list<Model*> otherModels;
	std::list<Model*> turretModels;
	std::list<Model*> gunModels;

	// Find the gun and turret meshes from all of the mesh
	// parts used to create the tank
	// Keep them and use them to draw tank's gun
	Vector turretCenter;
	Model *turretPivot = 0;
	Model *gunPivot = 0;
	std::list<Model *>::iterator itor;
	for (itor = aseTank.getModels().begin();
		itor != aseTank.getModels().end();
		itor++)
	{
		const char *name = (*itor)->getName();

		if (strstr(name, "\"Turret") == name ||
			strstr(name, "\"turret") == name)
		{
			if (strstr(name, "pivot") ||
				strstr(name, "Pivot"))
			{
				turretPivot = *itor;
			}
			else
			{
				turretModels.push_back(*itor);

				// Find the center that the tank should rotate around
				turretCenter += ((*itor)->getMax() + (*itor)->getMin()) / 2.0f;
			}
		}
		else if (strstr(name, "\"Gun") == name ||
			strstr(name, "\"gun") == name)
		{
			if (strstr(name, "pivot") ||
				strstr(name, "Pivot"))
			{
				gunPivot = *itor;
			}
			else
			{
				gunModels.push_back(*itor);
			}
		}
		else
		{
			otherModels.push_back(*itor);
		}
	}	

	// Check we have a valid tank model
	if (gunModels.empty() || turretModels.empty())
	{
		dialogMessage("Tank", "ERROR: Failed to find gun and turret from mesh");
		exit(1);
	}

	// Find the center of rotation for the turret
	if (turretPivot)
	{
		turretCenter = (turretPivot->getMax() + turretPivot->getMin()) / 2.0f;
	}
	else
	{
		turretCenter /= float(turretModels.size());
	}
	Vector gunCenter = turretCenter;
	turretCenter[2] = aseTank.getMin()[2];

	// Find the center of rotation for the gun
	if (gunPivot)
	{
		gunCenter = (gunPivot->getMax() + gunPivot->getMin()) / 2.0f;
	}
	gunOffset_ = gunCenter - turretCenter;

	modelCenter_ -= turretCenter;
	
	// Center all meshes around base of turret
	// Center gun around turret center
	// and place all of the meshes on the ground 
	for (itor = aseTank.getModels().begin();
		itor != aseTank.getModels().end();
		itor++)
	{
		const char *name = (*itor)->getName();
		if (strstr(name, "\"Gun") == name ||
			strstr(name, "\"gun") == name)
		{
			(*itor)->centre(gunCenter);
		}
		else
		{
			(*itor)->centre(turretCenter);
		}
	}

	addToSet(turretArrays_, turretModels, detail);
	addToSet(gunArrays_, gunModels, detail);
	addToSet(otherArrays_, otherModels, detail);
}

void TankMesh::addToSet(GLVertexSetGroup &vset, std::list<Model*> &models, float detail)
{
	std::list<Model*>::iterator mitor;
	for (mitor = models.begin();
		mitor != models.end();
		mitor++)
	{
		GLVertexSet *set = (*mitor)->getArray(useTextures_, detail);
		vset.addToGroup(*set);
	}	
}

void TankMesh::draw(bool drawS, float angle, Vector &position, 
					float fireOffset, float rotXY, float rotXZ,
					bool absCenter)
{
	GLState *texState = 0;
	if (useTextures_) texState = new GLState(GLState::TEXTURE_ON);

	float x = position[0];
	float y = position[1];
	float z = position[2];
	if (absCenter)
	{
		x -= modelCenter_[0];
		y -= modelCenter_[1];
		z -= modelCenter_[2];
	}

	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		otherArrays_.draw();
		drawGun(drawS, fireOffset, rotXY, rotXZ);
	glPopMatrix();

	delete texState;
}

void TankMesh::drawGun(bool drawS, float fireOffSet, float rotXY, float rotXZ)
{
	// Rotate around z axis and then draw turret
	glRotatef(rotXY, 0.0f, 0.0f, 1.0f);
	turretArrays_.draw();

	// Rotate around y axis and then draw gun
	glTranslatef(gunOffset_[0], gunOffset_[1], gunOffset_[2]);
	glRotatef(rotXZ, 1.0f, 0.0f, 0.0f);
	if (fireOffSet != 0.0f) glTranslatef(0.0f, fireOffSet, 0.0f);
	gunArrays_.draw();

	// Draw the sight
	if (drawS &&
		OptionsDisplay::instance()->getDrawPlayerSight()) drawSight();
}

void TankMesh::drawSight()
{
	if (!sightList_)
	{
		glNewList(sightList_ = glGenLists(1), GL_COMPILE);
			glBegin(GL_QUAD_STRIP);
				float x;
				for (x=135.0f; x>=90.0f; x-=9.0f)
				{
					const float deg = 3.14f / 180.0f;
					float dx = x * deg;
					float color = 1.0f - fabsf(90.0f - x) / 45.0f;

					glColor3f(1.0f * color, 0.5f * color, 0.5f * color);
					glVertex3f(+0.03f * color, 2.0f * sinf(dx), 2.0f * cosf(dx));
					glVertex3f(+0.03f * color, 10.0f * sinf(dx), 10.0f * cosf(dx));
				}
				for (x=90.0f; x<135.0f; x+=9.0f)
				{
					const float deg = 3.14f / 180.0f;
					float dx = x * deg;
					float color = 1.0f - fabsf(90.0f - x) / 45.0f;

					glColor3f(1.0f * color, 0.5f * color, 0.5f * color);
					glVertex3f(-0.03f * color, 2.0f * sinf(dx), 2.0f * cosf(dx));
					glVertex3f(-0.03f * color, 10.0f * sinf(dx), 10.0f * cosf(dx));
				}
			glEnd();
		glEndList();
	}

	GLState currentState(GLState::BLEND_OFF | GLState::TEXTURE_OFF);
	glCallList(sightList_);
}
