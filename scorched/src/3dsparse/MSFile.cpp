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
	char * wincr; 
	while (fgets(line, 256, in) != 0)
	{
		lineNo_++;
		if (wincr=strchr(line,'\r')) 
		{ 
			*wincr='\n'; 
			*(wincr + 1) = '\0'; 
		} 

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

	bool result = loadFile(in, fileName);

	fclose(in);
	return result;
}

bool MSFile::loadFile(FILE *in, const char *fileName)
{
	char filePath[256];
	strcpy(filePath, fileName);

	char *sep;
	while (sep=strchr(filePath, '\\')) *sep = '/';
	sep = strrchr(filePath, '/');
	if (sep) *sep = '\0';

	char buffer[256];
	if (!getNextLine(buffer, in)) return false; // Read no frames
	if (!getNextLine(buffer, in)) return false; // Read current frame

	// Read number meshes
	int noMeshes = 0;
	if (!getNextLine(buffer, in)) return false; 
	if (sscanf(buffer, "Meshes: %i", &noMeshes) != 1) return false;

	std::vector<int> meshMaterials;
	for (int i=0; i<noMeshes; i++)
	{
		// Read the mesh name, flags and material indices
		char meshName[256]; 
		int meshFlags, meshMatIndex;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%s %i %i", meshName, &meshFlags, &meshMatIndex) != 3) return false;
		meshMaterials.push_back(meshMatIndex);
		
		// Create and add the new model
		Model *model = new Model(meshName);
		models_.push_back(model);

		// Read no vertices
		int noVertices = 0;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%i", &noVertices) != 1) return false;

		int j;
		std::vector<Vector> tcoords;
		for (j=0; j<noVertices; j++)
		{
			// Read the current vertex
			int vertexFlags, vertexBIndex;
			Vector vertexPos, texCoord;
			if (!getNextLine(buffer, in)) return false; 
			if (sscanf(buffer, "%i %f %f %f %f %f %i",
				&vertexFlags,
				&vertexPos[0], &vertexPos[2], &vertexPos[1], 
				&texCoord[0], &texCoord[1], &vertexBIndex) != 7) return false;
			texCoord[1]=1.0f-texCoord[1];

			tcoords.push_back(texCoord);
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
				&nIndex1, &nIndex3, &nIndex2,
				&sGroup) != 8) return false;

			model->insertFace(face);
			model->setFaceNormal(normals[nIndex1], j, 0);
			model->setFaceNormal(normals[nIndex2], j, 1);
			model->setFaceNormal(normals[nIndex3], j, 2);

			model->setFaceTCoord(tcoords[face.v[0]], j, 0);
			model->setFaceTCoord(tcoords[face.v[1]], j, 1);
			model->setFaceTCoord(tcoords[face.v[2]], j, 2);
		}
	}

	// Read number meshes
	int noMaterials = 0;
	if (!getNextLine(buffer, in)) return false; 
	if (sscanf(buffer, "Materials: %i", &noMaterials) != 1) return false;

	for (int m=0; m<noMaterials; m++)
	{
		// material: name
		char materialName[256];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%s", materialName) != 1) return false;

		// ambient
		float ambient[4];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f %f %f %f", 
			&ambient[0], &ambient[1], &ambient[2], &ambient[3]) != 4) return false;

		// diffuse
		float diffuse[4];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f %f %f %f", 
			&diffuse[0], &diffuse[1], &diffuse[2], &diffuse[3]) != 4) return false;
		Vector dcolor(diffuse[0], diffuse[1], diffuse[2]);

		// specular
		float specular[4];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f %f %f %f", 
			&specular[0], &specular[1], &specular[2], &specular[3]) != 4) return false;

		// emissive
		float emissive[4];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f %f %f %f", 
			&emissive[0], &emissive[1], &emissive[2], &emissive[3]) != 4) return false;

		// shininess
		float shininess;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f", &shininess) != 1) return false;

		// transparency
		float transparency;
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%f", &transparency) != 1) return false;

		// color map
		char textureName[256];
		char fullTextureName[256];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%s", textureName) != 1) return false;
		textureName[strlen(textureName)-1] = '\0';
		sprintf(fullTextureName, "%s/%s", filePath, &textureName[1]);
		while (sep=strchr(fullTextureName, '\\')) *sep = '/';

		// alphamap
		char textureNameAlpha[256];
		char fullTextureAlphaName[256];
		if (!getNextLine(buffer, in)) return false; 
		if (sscanf(buffer, "%s", textureNameAlpha) != 1) return false;
		textureNameAlpha[strlen(textureNameAlpha)-1] = '\0';
		sprintf(fullTextureAlphaName, "%s/%s", filePath, &textureNameAlpha[1]);
		while (sep=strchr(fullTextureAlphaName, '\\')) *sep = '/';

		int modelIndex = 0;
		std::list<Model *>::iterator mitor;
		for (mitor = models_.begin();
			mitor != models_.end();
			mitor++, modelIndex++)
		{
			if (meshMaterials[modelIndex] == m)
			{
				Model *model = *mitor;
				model->setTextureName(fullTextureName);
				if (textureNameAlpha[1])
				{
					model->setATextureName(fullTextureAlphaName);
				}
				model->setColor(dcolor);
			}
		}
	}

	return true;
}
