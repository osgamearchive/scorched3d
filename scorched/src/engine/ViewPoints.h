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

#if !defined(__INCLUDE_ViewPointsh_INCLUDE__)
#define __INCLUDE_ViewPointsh_INCLUDE__

#include <list>
#include <common/Vector.h>

class ScorchedContext;
class ViewPoints
{
public:
	class ViewPoint 
	{
	public:
		ViewPoint() : radius_(0.0f), from_(1.0f, 0.0f, 0.0f) { }

		void setLookFrom(Vector &from) { from_ = from; }
		Vector &getLookFrom() { return from_; }
		
		void setPosition(Vector &pos) { position_ = pos; }
		Vector &getPosition() { return position_; }

		void setRadius(float radius) { radius_ = radius; }
		float getRadius() { return radius_; }

	protected:
		float radius_;
		Vector position_;
		Vector from_;
	};

	ViewPoints();
	virtual ~ViewPoints();

	void simulate(float frameTime);
	void getValues(Vector &lookAt, 
				   Vector &lookFrom);
	void setValues(Vector &lookAt, 
				   Vector &lookFrom);
	int getLookAtCount();

	ViewPoints::ViewPoint *getNewViewPoint(unsigned int playerId);
	void releaseViewPoint(ViewPoints::ViewPoint *point);

	void setContext(ScorchedContext *context) { context_ = context; }

protected:
	std::list<ViewPoint *> points_;
	ScorchedContext *context_;
	Vector lookAt_, lookFrom_;
	float totalTime_;

};

#endif
