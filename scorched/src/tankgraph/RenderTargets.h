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

#if !defined(__INCLUDE_RenderTargetsh_INCLUDE__)
#define __INCLUDE_RenderTargetsh_INCLUDE__

#include <engine/GameStateI.h>
#include <tankgraph/TankMenus.h>

class RenderTargets
{
public:
	static RenderTargets *instance();

	struct Renderer3D : public GameStateI
	{
		// Inherited from GameStateI
		virtual void draw(const unsigned state);
	} render3D;
	struct Renderer2D : public GameStateI
	{
		// Inherited from GameStateI
		virtual void draw(const unsigned state);
		virtual void simulate(const unsigned state, float simTime);
	} render2D;

	friend struct Renderer3D;
	friend struct Renderer2D;
protected:
	static RenderTargets *instance_;
	enum DrawType
	{
		Type3D,
		Type2D
	};
	TankMenus menus_;

	void draw(DrawType dt, const unsigned state);

private:
	RenderTargets();
	virtual ~RenderTargets();
};


#endif
