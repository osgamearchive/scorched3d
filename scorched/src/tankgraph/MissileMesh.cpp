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
#include <3dsparse/ModelRenderer.h>
#include <GLEXT/GLLenseFlare.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <common/Defines.h> // For porting

MissileMesh::MissileMesh(Model &missile) : 
	innerScale_(1.0f),
	scale_(1.0f)
{
	model_ = new ModelRenderer(&missile);

	// Make sure the missile is not too large
	const float maxSize = 2.0f;
	Vector min = model_->getModel()->getMin();
	Vector max = model_->getModel()->getMax();
	float size = (max - min).Magnitude();
	if (size > maxSize) innerScale_ = 2.2f / size;

	// Add lense flares (if any)
	std::vector<Mesh *>::iterator itor;
	for (itor = model_->getModel()->getMeshes().begin();
		itor != model_->getModel()->getMeshes().end();
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
	}	
}

MissileMesh::~MissileMesh()
{
	delete model_;
}

void MissileMesh::draw(Vector &position, Vector &direction, int flareType, float rotation)
{
	// Draw the missile shadow
	float aboveGround =
		position[2] - ScorchedClient::instance()->getLandscapeMaps().getHMap().
		getHeight((int) position[0], (int) position[1]);
	Landscape::instance()->getShadowMap().
		addCircle(position[0], position[1], aboveGround / 10.0f);

	// Firgure out the opengl roation matrix from the direction
	// of the fired missile
	Vector dir = direction.Normalize();
	const float radToDeg = 180.0f / 3.14f;
	float angXYRad = 3.14f - atan2f(dir[0], dir[1]);
	float angYZRad = acosf(dir[2]);
	float angXYDeg = angXYRad * radToDeg;
	float angYZDeg = angYZRad * radToDeg;

	// Apply the matrix and render the missile
	float scale = innerScale_ * scale_;
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		
		glRotatef(angXYDeg, 0.0f, 0.0f, 1.0f);
		glRotatef(angYZDeg, 1.0f, 0.0f, 0.0f);

		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glScalef(scale, scale, scale);
		model_->draw();
	glPopMatrix();

	// Draw any lense flares associated with the missile
	std::list<Vector>::iterator flareItor;
	for (flareItor =  flarePos_.begin();
		 flareItor != flarePos_.end();
		 flareItor++)
	{
		Vector &fpos = (*flareItor);

        float newX = fpos[0];
		float newY = (fpos[1] * getFastCos(angYZRad)) - (fpos[2] * getFastSin(angYZRad));
		float newZ = (fpos[1] * getFastSin(angYZRad)) + (fpos[2] * getFastCos(angYZRad)); 

		float newX2 = (newX * getFastCos(angXYRad)) - (newY * getFastSin(angXYRad));
		float newY2 = (newX * getFastSin(angXYRad)) + (newY * getFastCos(angXYRad)); 
		float newZ2 = newZ;

		Vector newPos;
		newPos[0] = position[0] + newX2 * scale;
		newPos[1] = position[1] + newY2 * scale;
		newPos[2] = position[2] + newZ2 * scale;

		GLLenseFlare::instance()->draw(newPos, false, flareType);
	}
}

void MissileMesh::setScale(float scale)
{
	scale_=scale;
}
