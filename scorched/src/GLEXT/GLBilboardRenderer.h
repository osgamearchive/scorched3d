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

#if !defined(__INCLUDE_GLBilboardRendererh_INCLUDE__)
#define __INCLUDE_GLBilboardRendererh_INCLUDE__

#include <GLEXT/GLTexture.h>
#include <engine/GameStateI.h>
#include <map>

// Renders bilboards using z-ordering
// i.e. the furthest away are drawn first
// and the nearest drawn last
class GLBilboardRenderer : public GameStateI
{
public:
	class Entry
	{
	public:
		Entry() : 
			width(1.0f), height(1.0f), alpha(1.0f), 
			texture(0), textureCoord(0) { }

		float posX, posY, posZ;
		float width, height;
		float alpha;
		GLTexture *texture;
		int textureCoord;
	};

	// Public Interface, use add entry to add bilboard
	static GLBilboardRenderer *instance();
	void addEntry(Entry *entry);

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned int state, float simTime);

protected:
	static GLBilboardRenderer *instance_;
	std::multimap<float, Entry *> entries_;
	float totalTime_;
	bool showMessages_;
	int totalSwitches_;
	int totalBilboards_;

private:
	GLBilboardRenderer();
	virtual ~GLBilboardRenderer();
};

#endif
