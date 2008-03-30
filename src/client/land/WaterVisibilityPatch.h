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

#if !defined(__INCLUDE_WaterVisibilityPatchh_INCLUDE__)
#define __INCLUDE_WaterVisibilityPatchh_INCLUDE__

#include <common/Vector.h>

class MipMapPatchIndexs;
class MipMapPatchIndex;

class WaterVisibilityPatch
{
public:
	WaterVisibilityPatch();
	~WaterVisibilityPatch();

	void setLocation(int x, int y,
		WaterVisibilityPatch *leftPatch, 
		WaterVisibilityPatch *rightPatch, 
		WaterVisibilityPatch *topPatch, 
		WaterVisibilityPatch *bottomPatch,
		float *waterIndexErrors);
	void setVisible(Vector &cameraPos, bool visible);

	int getVisibilityIndex() { return visible_?visibilityIndex_:-1; }
	int getPatchX() { return patchX_; }
	int getPatchY() { return patchY_; }
	Vector &getOffset() { return offset_; }
	Vector &getPosition() { return position_; }

	WaterVisibilityPatch *getLeftPatch() { return leftPatch_; }
	WaterVisibilityPatch *getRightPatch() { return rightPatch_; }
	WaterVisibilityPatch *getTopPatch() { return topPatch_; }
	WaterVisibilityPatch *getBottomPatch() { return bottomPatch_; }

protected:
	int x_, y_;
	int patchX_, patchY_, patchIndex_;
	int visibilityIndex_;
	bool visible_;
	float *waterIndexErrors_;
	Vector offset_, position_;
	WaterVisibilityPatch *leftPatch_;
	WaterVisibilityPatch *rightPatch_; 
	WaterVisibilityPatch *topPatch_;
	WaterVisibilityPatch *bottomPatch_;
};

#endif // __INCLUDE_WaterVisibilityPatchh_INCLUDE__