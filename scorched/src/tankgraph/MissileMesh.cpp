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
#include <common/Defines.h> // For porting

MissileMesh::MissileMesh(ModelsFile &missile, bool useTextures, float detail) 
	: useTextures_(useTextures)
{
	createArrays(missile, useTextures, detail);
}

MissileMesh::~MissileMesh()
{
	missileArrays_.destroyGroup();
}

void MissileMesh::createArrays(ModelsFile &aseTank, bool useTextures, float detail)
{
	// Check we have a valid tank model
	if (aseTank.getModels().empty())
	{
		dialogMessage("MissileMesh", 
					  "ERROR: Failed to find any objects in missile mesh");
		exit(1);
	}

	aseTank.centre();
	float dist = (aseTank.getMax() - aseTank.getMin()).Magnitude();
	if (dist > 1.0f) aseTank.scale(1.0f/dist);

	std::list<Model *>::iterator itor;
	for (itor = aseTank.getModels().begin();
		itor != aseTank.getModels().end();
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

void MissileMesh::draw(Vector &position, Vector &direction)
{
	GLState *texState = 0;
	if (useTextures_) texState = new GLState(GLState::TEXTURE_ON);

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		//glRotatef(angle, 0.0f, 0.0f, 1.0f);
		missileArrays_.draw();
	glPopMatrix();

	delete texState;

	std::list<Vector>::iterator flareItor;
	for (flareItor =  flarePos_.begin();
		 flareItor != flarePos_.end();
		 flareItor++)
	{
		Vector &fpos = (*flareItor);
		Vector newPos = position + fpos;
		GLLenseFlare::instance()->draw(newPos, true);
	}
}
