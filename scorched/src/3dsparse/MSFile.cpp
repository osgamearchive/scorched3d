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

#include <3dsparse/MSFile.h>

MSFile::MSFile(const char *fileName) : ModelsFile(fileName), lineNo_(0)
{
	success_ = loadFile(fileName);
	if (success_)
	{
		centre();
	}
}

MSFile::~MSFile()
{
}

bool MSFile::getNextLine(char *line, FILE *in)
{
	while (fgets(line, 256, in) != 0)
	{
		lineNo_++;
		if ((line[0] == '\\' && line[1] == '\\') ||
			(line[0] == '/' && line[1] == '/') ||
			(line[0] == '\0') || 
			(line[0] == '\n'))
		{
			// Skip this line
		}
		else return true;
	}

	return false;
}

bool MSFile::loadFile(const char *fileName)
{
	FILE *in = fopen(fileName, "r");
	if (!in) return false;

	bool result = loadFile(in);

	fclose(in);
	return result;
}

bool MSFile::loadFile(FILE *in)
{
	char buffer[256];
	if (!getNextLine(buffer, in)) return false; // Read no frames
	if (!getNextLine(buffer, in)) return false; // Read current frame

	// Read number meshes
	int noMeshes = 0;
	if (!getNextLine(buffer, in)) return false; 
	if (sscanf(buffer, "Meshes: %i", &noMeshes) != 1) return false;

	for (int i=0; i<noMeshes; i++)
	{
		// Read the mesh name, flags and material indices
		char meshName[256]; 
		int meshFlags, meshMatIndex;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%s %i %i", meshName, &meshFlags, &meshMatIndex) != 3) return false;
		
		// Create and add the new model
		Model *model = new Model(meshName);
		models_.push_back(model);

		// Read no vertices
		int noVertices = 0;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%i", &noVertices) != 1) return false;

		int j;
		for (j=0; j<noVertices; j++)
		{
			// Read the current vertex
			int vertexFlags, vertexBIndex;
			Vector vertexPos;
			float vertexU, vertexV;
			if (!getNextLine(buffer, in)) return false; 
			if (sscanf(buffer, "%i %f %f %f %f %f %i",
				&vertexFlags,
				&vertexPos[0], &vertexPos[2], &vertexPos[1], 
				&vertexU, &vertexV, &vertexBIndex) != 7) return false;

			model->insertVertex(vertexPos);
		}

		// Read no normals
		std::vector<Vector> normals;
		int noNormals = 0;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%i", &noNormals) != 1) return false;
		for (j=0; j<noNormals; j++)
		{
			// Read the current normal
			Vector normal;
			if (!getNextLine(buffer, in)) return false; 
			if (sscanf(buffer, "%f %f %f",
				&normal[0], &normal[2], &normal[1]) != 3) return false;

			normals.push_back(normal.Normalize());
		}

		// Read no faces
		int noFaces = 0;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%i", &noFaces) != 1) return false;
		for (j=0; j<noFaces; j++)
		{
			// Read the current face
			int faceFlags, sGroup;
			int nIndex1, nIndex2, nIndex3;
			Face face;
			if (!getNextLine(buffer, in)) return false; 
			if (sscanf(buffer, "%i %i %i %i %i %i %i %i",
				&faceFlags,
				&face.v[0], &face.v[2], &face.v[1],
				&nIndex1, &nIndex2, &nIndex3,
				&sGroup) != 8) return false;

			model->insertFace(face);
			model->setFaceNormal(normals[nIndex1], j, 0);
			model->setFaceNormal(normals[nIndex2], j, 2);
			model->setFaceNormal(normals[nIndex3], j, 1);
		}
	}

	return true;
}
