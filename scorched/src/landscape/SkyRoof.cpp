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
#include <landscape/LandscapeDefinition.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefn.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <math.h>

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
	Vector &ambient = 
		ScorchedClient::instance()->getLandscapeMaps().
			getLandDfn().getTex()->skyambience;
	Vector &diffuse = 
		ScorchedClient::instance()->getLandscapeMaps().
			getLandDfn().getTex()->skydiffuse;
	Vector &sunPos = Landscape::instance()->getSky().getSun().getPosition();
	Vector sunDirection = (sunPos - position).Normalize();
	
	float diffuseLight = (((normal.dotP(sunDirection)) / 2.0f) + 0.5f);
	Vector light = diffuse * diffuseLight + ambient;

	glColor3fv(light);
}

void SkyRoof::makeList()
{	
	HeightMap &rmap = ScorchedClient::instance()->
		getLandscapeMaps().getRMap();
	HeightMap &hmap = ScorchedClient::instance()->
		getLandscapeMaps().getHMap();

	float mult = float(hmap.getWidth()) / float(rmap.getWidth());
	
	glNewList(list_ = glGenLists(1), GL_COMPILE);
		for (int j=0; j<rmap.getWidth(); j++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int i=0; i<=rmap.getWidth(); i++)
			{
				Vector a(i * mult, j * mult, rmap.getHeight(i, j));
				makeNormal(a, rmap.getNormal(i, j));
				glTexCoord2f(a[0] / 64.0f, a[1] / 64.0f);
				glVertex3fv(a);
				
				Vector b(i * mult, (j + 1) * mult, rmap.getHeight(i, j + 1));
				makeNormal(b, rmap.getNormal(i, j + 1));
				glTexCoord2f(b[0] / 64.0f, b[1] / 64.0f);
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
				drawSegment(a, b, na, nb);
			}
			{
				Vector b(i * mult, 256.0f, rmap.getHeight(i, rmap.getWidth()));
				Vector nb = rmap.getNormal(i, rmap.getWidth());
				Vector a((i + 1) * mult, 256.0f, rmap.getHeight(i + 1, rmap.getWidth()));
				Vector na = rmap.getNormal(i + 1, rmap.getWidth());
				drawSegment(a, b, na, nb);
			}
			{
				Vector b(0.0f, i * mult, rmap.getHeight(0, i));
				Vector nb = rmap.getNormal(0, i);
				Vector a(0.0f, (i + 1) * mult, rmap.getHeight(0, i + 1));
				Vector na = rmap.getNormal(0, i + 1);
				drawSegment(a, b, na, nb);
			}
			{
				Vector a(256.0f, i * mult, rmap.getHeight(rmap.getWidth(), i));
				Vector na = rmap.getNormal(rmap.getWidth(), i);
				Vector b(256.0f, (i + 1) * mult, rmap.getHeight(rmap.getWidth(), i + 1));
				Vector nb = rmap.getNormal(rmap.getWidth(), i + 1);
				drawSegment(a, b, na, nb);
			}
		}

	glEndList();
}

void SkyRoof::drawSegment(Vector &a, Vector &b, Vector &na, Vector &nb)
{
	HeightMap &rmap = ScorchedClient::instance()->
		getLandscapeMaps().getRMap();
	LandscapeDefnRoofCavern *cavern = 
		(LandscapeDefnRoofCavern *) 
		ScorchedClient::instance()->getLandscapeMaps().
		getLandDfn().getDefn()->roof;
	float hemispehereRadius = cavern->width;

	const int steps = 5;

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
	diffa *= (hemispehereRadius - dista) / float(steps);
	diffb *= (hemispehereRadius - distb) / float(steps);

	Vector midPoint(128.0f, 128.0f, 0.0f);
	glBegin(GL_QUAD_STRIP);
	for (int i=0; i<=steps + 3; i++)
	{
		{
			Vector c = a + diffa;
			Vector e = (a - c).Normalize();
			Vector f = (a - b).Normalize();
			Vector n = e * f;

			glTexCoord2f(a[0] / 64.0f, a[1] / 64.0f);
			if (i < 1) n = na;
			makeNormal(a, n);
			glVertex3fv(a);
		}
		
		{
			Vector c = b + diffb;
			Vector f = (b - c).Normalize();
			Vector e = (b - a).Normalize();
			Vector n = e * f;

			glTexCoord2f(b[0] / 64.0f, b[1] / 64.0f);
			if (i < 1) n = nb;
			makeNormal(b, n);
			glVertex3fv(b);
		}

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
	
	Landscape::instance()->getSurroundTexture().draw(true);
	glCallList(list_);
}
