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

#if !defined(__INCLUDE_TargetRendererh_INCLUDE__)
#define __INCLUDE_TargetRendererh_INCLUDE__

class TargetRenderer
{
public:
	TargetRenderer();
	virtual ~TargetRenderer();

	virtual void simulate(float frameTime) = 0;
	virtual void draw(float distance) = 0; // Called during the main drawing loop
	virtual void drawSecond(float distance) = 0; // Called during the particle drawing loop
	virtual void draw2d() = 0; // Called during the 2d drawing loop

	virtual void shieldHit() = 0;
	virtual void fired() = 0;
};

#endif // __INCLUDE_TargetRendererh_INCLUDE__