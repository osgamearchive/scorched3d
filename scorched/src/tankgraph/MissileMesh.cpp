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
#include <tankgraph/MissileMesh.h>
#include <3dsparse/ModelsFile.h>
#include <GLEXT/GLLenseFlare.h>
#include <landscape/Landscape.h>
#include <client/ScorchedClient.h>
#include <common/Defines.h> // For porting

MissileMesh::MissileMesh(ModelsFile &missile, bool useTextures, float detail) 
	: useTextures_(useTextures), scale_(1.0f)
{
	createArrays(missile, useTextures, detail);
}

MissileMesh::~MissileMesh()
{
	missileArrays_.destroyGroup();
}

void MissileMesh::createArrays(ModelsFile &aseMissile, bool useTextures, float detail)
{
	// Check we have a valid tank model
	if (aseMissile.getModels().empty())
	{
		dialogMessage("MissileMesh", 
					  "ERROR: Failed to find any objects in missile mesh");
		exit(1);
	}

	// Make sure model is not too large
	aseMissile.centre();

	// Make sure the missile is not too large
	const float maxSize = 2.0f;
	float size = (aseMissile.getMax() - aseMissile.getMin()).Magnitude();
	if (size > maxSize)
	{
		const float sfactor = 2.2f / size;
		aseMissile.scale(sfactor);
	}

	std::list<Model *>::iterator itor;
	for (itor = aseMissile.getModels().begin();
		itor != aseMissile.getModels().end();
		itor++)
	{
		const char *name = (*itor)->getName();

		if (strstr(name, "\"Flare") == name ||
			strstr(name, "\"flare") == name)
		{
			// Find the center that the flare should be eminated from
			Vector center = ((*itor)->getMax() + (*itor)->getMin()) / 2.0f;
			flarePos_.push_back(center);
		}
		else
		{
			GLVertexSet *set = (*itor)->getArray(useTextures_, detail);
			missileArrays_.addToGroup(*set);
		}
	}	
}

void MissileMesh::draw(Vector &position, Vector &direction, int flareType, float rotation)
{
	// Draw the missile shadow
	float aboveGround =
		position[2] - ScorchedClient::instance()->getLandscapeMaps().getHMap().
		getHeight((int) position[0], (int) position[1]);
	Landscape::instance()->getShadowMap().
		addCircle(position[0], position[1], aboveGround / 10.0f);

	unsigned state = useTextures_?GLState::TEXTURE_ON:GLState::TEXTURE_OFF;
	GLState currentState(state);

	// Firgure out the opengl roation matrix from the direction
	// of the fired missile
	Vector dir = direction.Normalize();
	const float radToDeg = 180.0f / 3.14f;
	float angXYRad = 3.14f - atan2f(dir[0], dir[1]);
	float angYZRad = acosf(dir[2]);
	float angXYDeg = angXYRad * radToDeg;
	float angYZDeg = angYZRad * radToDeg;

	// Apply the matrix and render the missile
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		
		glRotatef(angXYDeg, 0.0f, 0.0f, 1.0f);
		glRotatef(angYZDeg, 1.0f, 0.0f, 0.0f);

		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glScalef(scale_, scale_, scale_);
		missileArrays_.draw();
	glPopMatrix();

	// Draw any lense flares associated with the missile
	std::list<Vector>::iterator flareItor;
	for (flareItor =  flarePos_.begin();
		 flareItor != flarePos_.end();
		 flareItor++)
	{
		Vector &fpos = (*flareItor);

        float newX = fpos[0];
		float newY = (fpos[1] * cosf(angYZRad)) - (fpos[2] * sinf(angYZRad));
		float newZ = (fpos[1] * sinf(angYZRad)) + (fpos[2] * cosf(angYZRad)); 

		float newX2 = (newX * cosf(angXYRad)) - (newY * sinf(angXYRad));
		float newY2 = (newX * sinf(angXYRad)) + (newY * cosf(angXYRad)); 
		float newZ2 = newZ;

		Vector newPos;
		newPos[0] = position[0] + newX2;
		newPos[1] = position[1] + newY2;
		newPos[2] = position[2] + newZ2;

		GLLenseFlare::instance()->draw(newPos, dir, flareType);
	}
}

void MissileMesh::setScale(float scale)
{
	scale_=scale;
}
