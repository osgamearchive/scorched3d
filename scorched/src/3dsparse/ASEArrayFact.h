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


#if !defined(__INCLUDE_ASEArrayFacth_INCLUDE__)
#define __INCLUDE_ASEArrayFacth_INCLUDE__

#include <GLEXT/GLVertexTexArray.h>
#include <3dsparse/ASEModel.h>

namespace ASEArrayFact
{
	enum MaxMag
	{
		MagX,
		MagY,
		MagZ
	};
	enum TexType
	{
		planear,
		cube
	};

	GLVertexArray *getArray(std::list<ASEModel> &models, float detail);
	GLVertexArray *getArray(std::list<ASEModel*> &models, float detail);

	GLVertexTexArray *getTexArray(std::list<ASEModel> &models,
		Vector &max, Vector &min, float detail, TexType texT = cube);
	GLVertexTexArray *getTexArray(std::list<ASEModel*> &models,
		Vector &max, Vector &min, float detail, TexType texT = cube);

	void drawTexArray(GLVertexTexArray *texArray);

	Vector getTexCoord(Vector &tri, MaxMag mag, Vector &max, Vector &min, TexType texT);
};


#endif
