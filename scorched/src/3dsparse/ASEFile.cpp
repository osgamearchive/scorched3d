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

#include <stdio.h>
#include <math.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>

extern FILE *asein;
extern int aseparse(void);
extern int aselineno;

ASEFile *ASEFile::current_ = 0;

ASEFile::ASEFile(const char *fileName,
				 const char *texName) : 
	ModelsFile(fileName), texName_(texName)
{
	printf(">\n");
	success_ = loadFile(fileName);
	printf("<\n");
	if (success_)
	{
		centre();
		calculateTexCoords();
	}
}

ASEFile::~ASEFile()
{

}

ASEFile *ASEFile::getCurrent()
{
	return current_;
}

ASEModel *ASEFile::getCurrentModel()
{
	if (!models_.empty())
	{
		return (ASEModel *) models_.back();
	}

	return 0;
}

void ASEFile::addModel(char *modelName)
{
	models_.push_back(new ASEModel(modelName, (char *) texName_));
}

bool ASEFile::loadFile(const char *fileName)
{
	asein = fopen(fileName, "r");
	if (!asein)
	{
		return false;
	}

	// Reset variables for next parser
	current_ = this;
	aselineno = 0;
	return (aseparse() == 0);
}

Vector ASEFile::getTexCoord(Vector &tri, MaxMag mag, Vector &max, Vector &min)
{
	Vector newTri = tri;
	newTri -= min;
	newTri /= (max - min);

	switch(mag)
	{
	case MagX:
		newTri /= 2.0f;
		newTri[0] = newTri[1] * 2.0f;
		newTri[1] = newTri[2];
		break;
	case MagY:
		newTri /= 2.0f;
		newTri[0] = newTri[0];
		newTri[1] = newTri[2] + 0.5f;
		break;
	case MagZ:
		newTri /= 2.0f;
		newTri[0] += 0.5f;
		newTri[1] += 0.5f;
		break;
	}

	return newTri;
}

void ASEFile::calculateTexCoords()
{
	std::list<Model *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		Model *model = *itor;
		std::vector<Face>::iterator fitor;
		for (fitor = model->getFaces().begin();
			fitor != model->getFaces().end();
			fitor++)
		{
			Face &face = *fitor;

			Vector triA = model->getVertex(face.v[0]);
			Vector triB = model->getVertex(face.v[1]);
			Vector triC = model->getVertex(face.v[2]);

			MaxMag maxMag = MagZ;
			Vector faceNormal = (face.normal[0] + face.normal[1] + face.normal[2]).Normalize();
			if (fabs(faceNormal[0]) >= fabs(faceNormal[1]) &&
				fabs(faceNormal[0]) >= fabs(faceNormal[2]))
			{
				maxMag = MagX;
			}
			else if (fabs(faceNormal[1]) >= fabs(faceNormal[0]) &&
				fabs(faceNormal[1]) >= fabs(faceNormal[2]))
			{
				maxMag = MagY;
			}

			face.tcoord[0] = getTexCoord(triA, maxMag, getMax(), getMin());
			face.tcoord[1] = getTexCoord(triB, maxMag, getMax(), getMin());
			face.tcoord[2] = getTexCoord(triC, maxMag, getMax(), getMin());
		}
	}
}
