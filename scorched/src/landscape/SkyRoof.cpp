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

#include <landscape/SkyRoof.h>
#include <landscape/Sky.h>
#include <landscape/Sun.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <math.h>

static const float ambientLightConst = 0.3f;
static const float directLightConst = 0.7f;

SkyRoof::SkyRoof() : list_(0)
{
}

SkyRoof::~SkyRoof()
{
}

void SkyRoof::generate()
{
	if (list_) glDeleteLists(list_, 1);
	list_ = 0;
}

void SkyRoof::makeNormal(Vector &position, Vector &normal)
{
	Vector &sunPos = Landscape::instance()->getSky().getSun().getPosition();
	Vector sunDirection = (sunPos - position).Normalize();
	
	float diffuseLight = directLightConst * (((normal.dotP(sunDirection)) / 2.0f) + 0.5f);
	float ambientLight = ambientLightConst;
	float lightIntense = diffuseLight + ambientLight;
	
	glColor3f(lightIntense, lightIntense, lightIntense);
}
	
void SkyRoof::makeList()
{	
	HeightMap &rmap = ScorchedClient::instance()->
		getLandscapeMaps().getRMap();
	HeightMap &hmap = ScorchedClient::instance()->
		getLandscapeMaps().getHMap();

	float mult = float(hmap.getWidth()) / float(rmap.getWidth());
	float texmult = mult / 4.0f;
	
	glNewList(list_ = glGenLists(1), GL_COMPILE);
		for (int j=0; j<rmap.getWidth(); j++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int i=0; i<=rmap.getWidth(); i++)
			{
				Vector a(i * mult, j * mult, rmap.getHeight(i, j));
				makeNormal(a, rmap.getNormal(i, j));
				glTexCoord2f(i * texmult, j * texmult);
				glVertex3fv(a);
				
				Vector b(i * mult, (j + 1) * mult, rmap.getHeight(i, j + 1));
				makeNormal(b, rmap.getNormal(i, j + 1));
				glTexCoord2f(i * texmult, (j+1) * texmult);
				glVertex3fv(b);
			}
			glEnd();
		}

		for (int i=0; i<rmap.getWidth(); i++)
		{
			{
				Vector a(i * mult, 0.0f, rmap.getHeight(i, 0));
				Vector na = rmap.getNormal(i,0);
				Vector b((i + 1) * mult, 0.0f, rmap.getHeight(i + 1, 0));
				Vector nb = rmap.getNormal(i + 1, 0);
				drawSegment(a, b, na, nb, texmult);
			}
			{
				Vector b(i * mult, 256.0f, rmap.getHeight(i, rmap.getWidth()));
				Vector nb = rmap.getNormal(i, rmap.getWidth());
				Vector a((i + 1) * mult, 256.0f, rmap.getHeight(i + 1, rmap.getWidth()));
				Vector na = rmap.getNormal(i + 1, rmap.getWidth());
				drawSegment(a, b, na, nb, texmult);
			}
			{
				Vector b(0.0f, i * mult, rmap.getHeight(0, i));
				Vector nb = rmap.getNormal(0, i);
				Vector a(0.0f, (i + 1) * mult, rmap.getHeight(0, i + 1));
				Vector na = rmap.getNormal(0, i + 1);
				drawSegment(a, b, na, nb, texmult);
			}
			{
				Vector a(256.0f, i * mult, rmap.getHeight(rmap.getWidth(), i));
				Vector na = rmap.getNormal(rmap.getWidth(), i);
				Vector b(256.0f, (i + 1) * mult, rmap.getHeight(rmap.getWidth(), i + 1));
				Vector nb = rmap.getNormal(rmap.getWidth(), i + 1);
				drawSegment(a, b, na, nb, texmult);
			}
		}

	glEndList();
}

void SkyRoof::drawSegment(Vector &a, Vector &b, Vector &na, Vector &nb, float texmult)
{
	HeightMap &rmap = ScorchedClient::instance()->
		getLandscapeMaps().getRMap();
	const float hemispehereRadius = 1500.0f;
	const int steps = 10;

	float heighta = a[2];
	float heightb = b[2];
	Vector midPointA(128.0f, 128.0f, heighta);
	Vector midPointB(128.0f, 128.0f, heightb);

	Vector diffa = (a - midPointA);
	Vector diffb = (b - midPointB);
	float dista = diffa.Magnitude();
	float distb = diffb.Magnitude();
	diffa.StoreNormalize();
	diffb.StoreNormalize();
	diffa *= (1800.0f - dista) / float(steps);
	diffb *= (1800.0f - distb) / float(steps);

	Vector midPoint(128.0f, 128.0f, 0.0f);
	glBegin(GL_QUAD_STRIP);
	for (int i=0; i<=steps + 3; i++)
	{
		glTexCoord2f(a[0] * texmult, a[1] * texmult);
		Vector dira(0.0f, 0.0f, -1.0f);// = (midPoint - a).Normalize();
		makeNormal(a, na);
		glVertex3fv(a);
		
		glTexCoord2f(b[0] * texmult, b[1] * texmult);
		Vector dirb(0.0f, 0.0f, -1.0f);// = (midPoint - b).Normalize();
		makeNormal(b, nb);
		glVertex3fv(b);

		a += diffa;
		b += diffb;
		a[2] = heighta * cosf(1.57f * float(i) / float(steps));
		b[2] = heightb * cosf(1.57f * float(i) / float(steps));
	}	
	glEnd();
}

void SkyRoof::draw()
{
	if (!list_) makeList();
	
	GLState state(GLState::TEXTURE_OFF);
	glCallList(list_);
}

