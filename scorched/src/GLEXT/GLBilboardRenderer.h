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
#include <GLEXT/GLOrderedItemRenderer.h>
#include <map>

// Renders bilboards using z-ordering
// i.e. the furthest away are drawn first
// and the nearest drawn last
class GLBilboardRenderer : public GLOrderedItemRendererProvider,
						   public GLOrderedItemRendererProviderSetup
{
public:
	static GLBilboardRenderer *instance();

	class Entry : public GLOrderedItemRenderer::OrderedEntry
	{
	public:
		Entry() : 
			width(1.0f), height(1.0f), alpha(1.0f), 
			texture(0), textureCoord(0), style(0) { }

		float width, height;
		float alpha;
		GLTexture *texture;
		int textureCoord;
		int style;
	};

	// Public Interface, use add entry to add bilboard
	void addEntry(Entry *entry);

	// Inherited from Interfaces
	virtual void itemsSetup();
	virtual void itemsSimulate(float simTime);
	virtual void drawItem(GLOrderedItemRenderer::OrderedEntry &entry);

protected:
	static GLBilboardRenderer *instance_;

	// Stats
	float totalTime_;
	bool showMessages_;
	int totalSwitches_;
	int totalBilboards_;

	// State
	Vector bilX_;
	Vector bilY_;
	float bilboardsAllowed_;
	float bilboardCount_;
	float bilboardDrawn_;
	int bilboardsThisFrame_;

private:
	GLBilboardRenderer();
	virtual ~GLBilboardRenderer();
};

#endif
