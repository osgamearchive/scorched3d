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

#include <3dsparse/ModelArrayFact.h>
#include <math.h>

GLVertexArray *ModelArrayFact::getArray(std::list<Model> &modelList, float detail)
{
	std::list<Model *> modelList2;
	std::list<Model>::iterator itor;
	for (itor = modelList.begin();
		itor != modelList.end();
		itor++)
	{
		modelList2.push_back(&*itor);
	}
	return getArray(modelList2, detail);
}

GLVertexArray *ModelArrayFact::getArray(std::list<Model*> &modelList, float detail)
{
	std::list<Vector> triangles;
	std::list<Vector> normals;
	std::list<Vector> colors;

	std::list<Model*>::iterator itor;
	for (itor = modelList.begin();
		itor != modelList.end();
		itor++)
	{
		int beforesize = (int) triangles.size();
		(*itor)->getArray(triangles, normals, detail);

		for (int a=beforesize; a<(int) triangles.size(); a++)
		{
			colors.push_back((*itor)->getColor());
		}
	}

	Vector lightpos(-0.3f, -0.2f, 1.0f);
	lightpos.Normalize();

	GLVertexArray *array = new GLVertexArray(int(triangles.size()) / 3);
	std::list<Vector>::iterator triangleItor = triangles.begin();
	std::list<Vector>::iterator normalItor = normals.begin();
	std::list<Vector>::iterator colorItor = colors.begin();
	int triPos = 0;
	while (triangleItor != triangles.end() &&
		normalItor != normals.end())
	{
		float intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, (*colorItor)[0] * intense, (*colorItor)[1] * intense, (*colorItor)[2] * intense);
		colorItor++; triangleItor++; normalItor++; triPos++;

		intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, (*colorItor)[0] * intense, (*colorItor)[1] * intense, 
						(*colorItor)[2] * intense);
		colorItor++; triangleItor++; normalItor++; triPos++;

		intense = lightpos.dotP((*normalItor).Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		array->setVertex(triPos, (*triangleItor)[0], (*triangleItor)[1], (*triangleItor)[2]); 
		array->setColor(triPos, (*colorItor)[0] * intense, (*colorItor)[1] * intense, 
						(*colorItor)[2] * intense);
		colorItor++; triangleItor++; normalItor++; triPos++;
	}

	return array;
}

GLVertexTexArray *ModelArrayFact::getTexArray(std::list<Model> &modelList, 
											Vector &max, Vector &min, 
											float detail)
{
	std::list<Model *> modelList2;
	std::list<Model>::iterator itor;
	for (itor = modelList.begin();
		itor != modelList.end();
		itor++)
	{
		modelList2.push_back(&*itor);
	}
	return getTexArray(modelList2, max, min, detail);
}

GLVertexTexArray *ModelArrayFact::getTexArray(std::list<Model*> &modelList, 
											Vector &max, Vector &min, 
											float detail)
{
	std::list<Vector> triangles;
	std::list<Vector> normals;

	std::list<Model*>::iterator itor;
	for (itor = modelList.begin();
		itor != modelList.end();
		itor++)
	{
		(*itor)->getArray(triangles, normals, detail);
	}

	Vector lightpos(0.3f, 0.2f, 1.0f);
	lightpos.Normalize();

	GLVertexTexArray *array = new GLVertexTexArray(int(triangles.size()) / 3);
	std::list<Vector>::iterator triangleItor = triangles.begin();
	std::list<Vector>::iterator normalItor = normals.begin();
	int triPos = 0;
	while (triangleItor != triangles.end() &&
		normalItor != normals.end())
	{
		Vector &normalA = (*normalItor);
		Vector &triA = (*triangleItor);
		triangleItor++; normalItor++; 
		Vector &normalB = (*normalItor);
		Vector &triB = (*triangleItor);
		triangleItor++; normalItor++; 
		Vector &normalC = (*normalItor);
		Vector &triC = (*triangleItor);
		triangleItor++; normalItor++; 

		MaxMag maxMag = MagZ;
		Vector texCoord;
		float intense = 0.0f;
		Vector faceNormal = (normalA + normalB + normalC).Normalize();
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

		intense = lightpos.dotP(normalA.Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		texCoord = getTexCoord(triA, maxMag, max, min);
		array->setVertex(triPos, triA[0], triA[1], triA[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, texCoord[0], texCoord[1]);
		triPos++;
		
		intense = lightpos.dotP(normalB.Normalize()) + 0.2f;
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		texCoord = getTexCoord(triB, maxMag, max, min);
		array->setVertex(triPos, triB[0], triB[1], triB[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, texCoord[0], texCoord[1]);
		triPos++;

		intense = lightpos.dotP(normalC.Normalize()) + 0.2f; 
		if (intense > 1.0f) intense = 1.0f; if (intense < 0.2f) intense = 0.2f;
		texCoord = getTexCoord(triC, maxMag, max, min);
		array->setVertex(triPos, triC[0], triC[1], triC[2]); 
		array->setColor(triPos, intense, intense, intense);
		array->setTexCoord(triPos, texCoord[0], texCoord[1]);
		triPos++;
	}

	return array;
}

Vector ModelArrayFact::getTexCoord(Vector &tri, MaxMag mag, Vector &max, Vector &min)
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

