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

/** 
Renders bilboards using z-ordering (painters algorithm)
i.e. the furthest away are drawn first and the nearest drawn last
This prevents problems with transparent objects not being drawn
correctly.

This class makes use of the GLOrderedItemRenderer to perform
the sorting and scheduling of the bilboards.
*/
class GLBilboardRenderer : public GLOrderedItemRendererProvider,
						   public GLOrderedItemRendererProviderSetup
{
public:
	/** Return an instance to the only GLBilboarRenderer. */
	static GLBilboardRenderer *instance();

	/**
	Bilboards are represented using this class.
	The class defines all attributes associated with the bilboard.
	Its width, height, texture etc..
	The user should make sure all required attributes are defines
	before adding a bilboard to the renderer.
	*/
	class GLBilboardOrderedEntry : public GLOrderedItemRenderer::OrderedEntry
	{
	public:
		GLBilboardOrderedEntry() : 
			width(1.0f), height(1.0f), alpha(1.0f), 
			texture(0), textureCoord(0), 
			r_color(1.0f), g_color(1.0f), b_color(1.0f) { }

		float width, height;
		float alpha;
		GLTexture *texture;
		int textureCoord;
		float r_color, g_color, b_color;
	};

	/**
	Add a new bilboard to the renderer.
	Bilboards will be rendered every frame until removed by a 
	call to removeEntry.  Bilboards should be global
	objects or allocated on the heap.
	*/
	void addEntry(GLBilboardOrderedEntry *entry);
	/**
	Remove a bilboard from the renderer.
	DeleteItem specifies if the bilboard object should be deleted
	by the bilboard renderer when it is removed.
	Items that are not deleted by the renderer should NOT be deleted 
	by the caller (ever) as the renderer may still have a reference to them.
	These bilboards must be global objects that are never deleted.
	*/
	void removeEntry(GLBilboardOrderedEntry *entry, bool deleteItem = false);

	// Inherited from Interfaces
	virtual void itemsSetup();
	virtual void itemsSimulate(float simTime);
	virtual void drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &entry);

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

private:
	GLBilboardRenderer();
	virtual ~GLBilboardRenderer();
};

#endif
