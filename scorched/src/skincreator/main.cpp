#include <stdio.h>
#include <GLEXT\GLState.h>
#include <GLEXT\GLBitmap.h>
#include <GLEXT\GLVertexTexArray.h>
#include <gl\glaux.h>
#include <3dsparse\ASEFile.h>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("ERROR: Require a .ase file name\n");
		exit(1);
	}

	ASEFile afile((const char *) argv[1], "");
	if (!afile.getSuccess())
	{
		printf("ERROR: Failed to load \"%s\" file\n", argv[1]);
		exit(1);
	}

	const int texSize = 256;

	auxInitDisplayMode(AUX_DOUBLE | AUX_RGBA);
	auxInitPosition(100,100,texSize,texSize);
	auxInitWindow("Tank Skin");
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, texSize, texSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glScalef(100.0f, 100.0f, 100.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<Model *>::iterator itor;
	for (itor = afile.getModels().begin();
		itor != afile.getModels().end();
		itor++)
	{
		GLVertexTexArray *texArray = (GLVertexTexArray *) 
			(*itor)->getArray(true, 1.0f);

		glBegin(GL_TRIANGLES);
		for (int i=0; i<texArray->getNoTris() * 3; i++)
		{
			float x = texArray->getTexCoordInfo(i).a;
			float y = texArray->getTexCoordInfo(i).b;
			glVertex2f(x, y);
		}
		glEnd();
	}

	glFlush();
	auxSwapBuffers();

	GLBitmap bitmap;
	bitmap.grabScreen();
	char buffer[1024];
	strcpy(buffer, argv[1]);
	char *pos = strrchr(buffer, '.');
	if (pos)
	{
		pos++;
		pos[0] = 'b';
		pos[1] = 'm';
		pos[2] = 'p';

		bitmap.writeToFile(buffer);

		printf("\nFile \"%s\" written correctly\n", buffer);
	}

	return 0;
}
