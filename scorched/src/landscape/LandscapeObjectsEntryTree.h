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

#if !defined(__INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__)
#define __INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__

#include <landscape/LandscapeObjectsEntry.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>

class LandscapeObjectsEntryTree : public LandscapeObjectsEntry
{
public:
	void setup(const char *type, bool snow);

	virtual void render(float distance);

protected:
	enum TreeType
	{
		eNone,
		ePineNormal,
		ePineBurnt,
		ePineYellow,
		ePineLight,
		ePineSnow,
		ePalmNormal,
		ePalmBurnt,
	};

	static GLuint treePineList, treePineSmallList;
	static GLuint treePineSnowList, treePineSnowSmallList;
	static GLuint treePineBurntList, treePineBurntSmallList;
	static GLuint treePineYellowList, treePineYellowSmallList;
	static GLuint treePineLightList, treePineLightSmallList;
	static GLuint treePalmList, treePalmSmallList;
	static GLuint treePalmBurntList, treePalmBurntSmallList;
	static GLTexture pineTextureA_;
	static GLTexture palmTextureA_;

	TreeType normalType_;
	TreeType burntType_;
};

#endif // __INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__
