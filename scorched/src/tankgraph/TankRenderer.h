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


#if !defined(__INCLUDE_TankRendererh_INCLUDE__)
#define __INCLUDE_TankRendererh_INCLUDE__

#include <engine/GameStateI.h>
#include <tankgraph/TankMenus.h>
#include <tankgraph/TracerStore.h>

class TankRenderer
{
public:
	static TankRenderer *instance();

	TracerStore &getTracerStore() { return tracerStore_; }

	struct Renderer3D : public GameStateI
	{
		// Inherited from GameStateI
		virtual void draw(const unsigned state);
		virtual void simulate(const unsigned state, float simTime);
	} render3D;
	struct Renderer3DSecond : public GameStateI
	{
		// Inherited from GameStateI
		virtual void draw(const unsigned state);
	} render3DSecond;
	struct Renderer2D : public GameStateI
	{
		// Inherited from GameStateI
		virtual void draw(const unsigned state);
	} render2D;

	void newGame();

	friend struct Renderer3D;
	friend struct Renderer3DSecond;
	friend struct Renderer2D;
protected:
	static TankRenderer *instance_;
	enum DrawType
	{
		Type3D,
		Type3DSecond,
		Type2D
	};
	TankMenus menus_;
	TracerStore tracerStore_;

	void draw(DrawType dt, const unsigned state);

private:
	TankRenderer();
	virtual ~TankRenderer();
};


#endif
