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

#include <common/Defines.h>
#include <3dsparse/Model.h>
#include <3dsparse/MeshLOD.h>
#include <3dsparse/ASEStore.h>
#include <GLEXT/GLVertexTexArray.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Model::Model(char *name) : 
	name_(name), computedCollapseCosts_(false), 
	color_(0.5f, 0.5f, 0.5f)
{

}

Model::~Model()
{

}

void Model::setColor(Vector &color)
{
	color_ = color;
}

void Model::insertVertex(Vector &newVertex)
{
	if (vertexes_.empty())
	{
		max_ = newVertex;
		min_ = newVertex;
	}
	else
	{
		max_[0] = MAX(max_[0], newVertex[0]);
		max_[1] = MAX(max_[1], newVertex[1]);
		max_[2] = MAX(max_[2], newVertex[2]);

		min_[0] = MIN(min_[0], newVertex[0]);
		min_[1] = MIN(min_[1], newVertex[1]);
		min_[2] = MIN(min_[2], newVertex[2]);
	}

	vertexes_.push_back(newVertex);
}

void Model::insertFace(Face &newFace)
{
	faces_.push_back(newFace);
}

void Model::setFaceTCoord(Vector &tcoord, int face, int index)
{
	DIALOG_ASSERT(index < 3);
	DIALOG_ASSERT(face < (int) faces_.size());

	faces_[face].tcoord[index] = tcoord;
}

void Model::setFaceNormal(Vector &normal, int face, int index)
{
	DIALOG_ASSERT(index < 3);
	DIALOG_ASSERT(face < (int) faces_.size());

	faces_[face].normal[index] = normal;
}

Vector &Model::getVertex(int pos)
{
	DIALOG_ASSERT(pos < (int) vertexes_.size());
	return vertexes_[pos];
}

void Model::centre(Vector &centre)
{
	std::vector<Vector>::iterator itor;
	for (itor = vertexes_.begin();
		itor != vertexes_.end();
		itor++)
	{
		(*itor) -= centre;
	}
	max_ -= centre;
	min_ -= centre;
}

void Model::scale(float scalef)
{
	std::vector<Vector>::iterator itor;
	for (itor = vertexes_.begin();
		itor != vertexes_.end();
		itor++)
	{
		(*itor) *= scalef;
	}
	max_ *= scalef;
	min_ *= scalef;
}

void Model::computeCollapseCosts()
{
	if (!computedCollapseCosts_)
	{
		MeshLOD::progressiveMesh(vertexes_, faces_, map_);
		computedCollapseCosts_ = true;
	}
}

int Model::mapIndex(int i, float currentReduction)
{
	if (currentReduction == 1.0f) return i;

	int newVertexVal = (int (currentReduction * (float) vertexes_.size()));

	int returnval = i;
	while (returnval > newVertexVal)
	{
		returnval=map_[returnval];
	}

	return returnval;
}

GLVertexArray *Model::getArray(bool useTextures, float detail)
{
	GLVertexArray *result = 0;
	if (useTextures && getTextureName()[0]) result = getTexArray(detail);
	else result = getNoTexArray(detail);
	return result;
}

GLVertexArray *Model::getNoTexArray(float detail)
{
	// Get the list or normals and triangles for this detail level
	std::list<Vector> triangles;
	std::list<Vector> normals;
	std::list<Vector> texCoords;
	formArray(triangles, normals, texCoords, detail);

	Vector lightpos(-0.3f, -0.2f, 1.0f);
	lightpos.Normalize();

	GLVertexArray *array = new GLVertexArray(int(triangles.size()) / 3);
	std::list<Vector>::iterator triangleItor = triangles.begin();
	std::list<Vector>::iterator normalItor = normals.begin();
	int triPos = 0;
	while (triangleItor != triangles.end() &&
		normalItor != normals.end())
	{
		float intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, color_[0] * intense, color_[1] * intense, color_[2] * intense);
		triangleItor++; normalItor++; triPos++;

		intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, color_[0] * intense, color_[1] * intense, 
						color_[2] * intense);
		triangleItor++; normalItor++; triPos++;

		intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, color_[0] * intense, color_[1] * intense, 
						color_[2] * intense);
		triangleItor++; normalItor++; triPos++;
	}

	return array;
}

GLVertexArray *Model::getTexArray(float detail)
{
	std::list<Vector> triangles;
	std::list<Vector> normals;
	std::list<Vector> texCoords;
	formArray(triangles, normals, texCoords, detail);

	Vector lightpos(0.3f, 0.2f, 1.0f);
	lightpos.Normalize();

	GLTexture *texture = ASEStore::instance()->loadTexture(getTextureName());
	DIALOG_ASSERT(texture);
	GLVertexTexArray *array = new GLVertexTexArray(texture, 
		int(triangles.size()) / 3);
	std::list<Vector>::iterator triangleItor = triangles.begin();
	std::list<Vector>::iterator normalItor = normals.begin();
	std::list<Vector>::iterator texCoordsItor = texCoords.begin();
	int triPos = 0;
	while (triangleItor != triangles.end() &&
		normalItor != normals.end() &&
		texCoordsItor != texCoords.end())
	{
		Vector &normalA = (*normalItor);
		Vector &triA = (*triangleItor);
		Vector &coordA = (*texCoordsItor);
		triangleItor++; normalItor++; texCoordsItor++;
		Vector &normalB = (*normalItor);
		Vector &triB = (*triangleItor);
		Vector &coordB = (*texCoordsItor);
		triangleItor++; normalItor++; texCoordsItor++;
		Vector &normalC = (*normalItor);
		Vector &triC = (*triangleItor);
		Vector &coordC = (*texCoordsItor);
		triangleItor++; normalItor++; texCoordsItor++;

		float intense = lightpos.dotP(normalA.Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, triA[0], triA[1], triA[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, coordA[0], coordA[1]);
		triPos++;
		
		intense = lightpos.dotP(normalB.Normalize()) + 0.2f;
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, triB[0], triB[1], triB[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, coordB[0], coordB[1]);
		triPos++;

		intense = lightpos.dotP(normalC.Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, triC[0], triC[1], triC[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, coordC[0], coordC[1]);
		triPos++;
	}

	return array;
}

void Model::formArray(std::list<Vector> &triList, 
						std::list<Vector> &normalList,
						std::list<Vector> &texCoordList,
						float detail)
{
	if (detail != 1.0f) computeCollapseCosts();

	std::vector<Face>::iterator itor;
	for (itor = faces_.begin();
		itor != faces_.end();
		itor++)
	{
		int posA = mapIndex(itor->v[0], detail);
		int posB = mapIndex(itor->v[1], detail);
		int posC = mapIndex(itor->v[2], detail);
		if ( posA == posB || posA == posC || posB == posC )
		{
			// Do not draw			
		}
		else
		{
			triList.push_back(getVertex(posA));
			triList.push_back(getVertex(posB));
			triList.push_back(getVertex(posC));

			texCoordList.push_back(itor->tcoord[0]);
			texCoordList.push_back(itor->tcoord[1]);
			texCoordList.push_back(itor->tcoord[2]);

			normalList.push_back(itor->normal[0]);
			normalList.push_back(itor->normal[1]);
			normalList.push_back(itor->normal[2]);
		}
	}
}
