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

#if !defined(__INCLUDE_LandscapeObjectsh_INCLUDE__)
#define __INCLUDE_LandscapeObjectsh_INCLUDE__

#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLOrderedItemRenderer.h>
#include <map>

class LandscapeObjects : public GLOrderedItemRendererProvider
{
public:
	LandscapeObjects();
	virtual ~LandscapeObjects();

	void draw();
	void generate(RandomGenerator &generator, ProgressCounter *counter = 0);
	void drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &entry);

	void removeAllTrees();
	void removeTrees(unsigned int x, unsigned int y);
	void burnTrees(unsigned int x, unsigned int y);

protected:
	struct LandscapeObjectOrderedEntry : public GLOrderedItemRenderer::OrderedEntry
	{
		GLuint treeType;
		float treeRotation;
		float treeColor;
		float treeSize;
	};

	GLuint tree1, tree2, tree3;
	GLuint treeBurnt, treeSnow;
	std::multimap<unsigned int, LandscapeObjectOrderedEntry*> entries_;
	GLTexture texture_;

};

#endif
