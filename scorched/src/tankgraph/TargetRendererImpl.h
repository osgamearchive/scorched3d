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

#if !defined(__INCLUDE_TargetRendererImplh_INCLUDE__)
#define __INCLUDE_TargetRendererImplh_INCLUDE__

#include <target/Target.h>
#include <target/TargetRenderer.h>

class TargetRendererImpl : public TargetRenderer
{
public:
	TargetRendererImpl();
	virtual ~TargetRendererImpl();

	void setMakeParticle() { particleMade_ = false; }

	enum HighlightType
	{
		eNoHighlight,
		ePlayerHighlight,
		eOtherHighlight
	};
	static void setHighlightType(HighlightType type) { highlightType_ = type; }
	void createParticle(Target *target);

protected:
	static HighlightType highlightType_;
	bool particleMade_;

	void drawShield(Target *target, float shieldHit, float totalTime);
	void drawParachute(Target *target);
};

#endif // __INCLUDE_TargetRendererImplh_INCLUDE__
