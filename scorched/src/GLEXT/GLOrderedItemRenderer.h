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

#if !defined(__INCLUDE_GLOrderedItemRendererh_INCLUDE__)
#define __INCLUDE_GLOrderedItemRendererh_INCLUDE__

#include <engine/GameStateI.h>
#include <map>
#include <list>
#include <vector>

// Renders items using z-ordering
// i.e. the furthest away are drawn first
// and the nearest drawn last
class GLOrderedItemRendererProvider;
class GLOrderedItemRendererProviderSetup;
class GLOrderedItemRenderer : public GameStateI
{
public:
	class OrderedEntry
	{
	public:
		OrderedEntry() : provider_(0), 
			requiredItem_(true), removeItem_(false), deleteItem_(true),
			distance(0.0f), 
			posX(0.0f), posY(0.0f), posZ(0.0f) {}

		float posX, posY, posZ;
		float distance;
		bool requiredItem_;
		bool removeItem_, deleteItem_;
		GLOrderedItemRendererProvider *provider_;
	};

	// Public Interface, use add entry to add bilboard
	static GLOrderedItemRenderer *instance();
	void addEntry(OrderedEntry *entry);
	void rmEntry(OrderedEntry *entry, bool deleteItem);
	void addSetup(GLOrderedItemRendererProviderSetup *setup);

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned int state, float simTime);

protected:
	static GLOrderedItemRenderer *instance_;
	int numberOfBilboards_;
	std::vector<OrderedEntry *> requiredEntries_;
	std::vector<OrderedEntry *> tmpRequiredEntries_;
	std::list<GLOrderedItemRendererProviderSetup *> setups_;

private:
	GLOrderedItemRenderer();
	virtual ~GLOrderedItemRenderer();
};

class GLOrderedItemRendererProvider
{
public:
	virtual ~GLOrderedItemRendererProvider();

	virtual void drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &entry) = 0;
};

class GLOrderedItemRendererProviderSetup
{
public:
	virtual ~GLOrderedItemRendererProviderSetup();
	
	virtual void itemsSetup() = 0;
};

#endif
