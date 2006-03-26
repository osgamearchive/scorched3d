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

#if !defined(AFX_GLBITMAPMODIFIER_H__3C6E8BBF_1AB2_4847_BEB1_934C97C76F05__INCLUDED_)
#define AFX_GLBITMAPMODIFIER_H__3C6E8BBF_1AB2_4847_BEB1_934C97C76F05__INCLUDED_

#include <GLEXT/GLBitmap.h>
#include <landscape/HeightMap.h>
#include <common/Vector.h>

namespace GLBitmapModifier  
{
	bool findIntersection(HeightMap &hMap,
							Vector start,
							Vector end,
							float &dist,
							float stopDist);

	void tileBitmap(GLBitmap &src, GLBitmap &dest);

	void addLightMapToBitmap(GLBitmap &bitmap, 
							HeightMap &hMap,
							Vector &sunPos,
							Vector &ambience,
							Vector &diffuse,
							ProgressCounter *counter = 0);

	void addHeightToBitmap(HeightMap &hMap,
							GLBitmap &destBitmap, 
							GLBitmap &slopeBitmap,
							GLBitmap &shoreBitmap,
							GLBitmap **heightBitmaps,
							int numberSources,
							int destBitmapScaleSize,
							ProgressCounter *counter = 0);

	void addWaterToBitmap(HeightMap &hMap,
							GLBitmap &destBitmap,
							GLBitmap &waterBitmap,
							float waterHeight);

	void removeWaterFromBitmap(HeightMap &hMap,
							GLBitmap &srcBitmap,
							GLBitmap &destBitmap,
							GLBitmap &alphaBitmap,
							float waterHeight);

	void addBorderToBitmap(GLBitmap &destBitmap,
							int borderWidth,
							float colors[3]);

	void makeBitmapTransparent(GLBitmap &output,
		GLBitmap &input,
		GLBitmap &mask);

	void addCircle(GLBitmap &destBitmap,
		float sx, float sy, float sw, float opacity);

	void addBitmap(GLBitmap &destBitmap,
		GLImage &srcBitmap,
		float sx, float sy, float scalex, float scaley);

	void scalePlanBitmap(GLBitmap &destBitmap,
		GLBitmap &srcBitmap,
		int landscapeX, int landscapeY);
};

#endif // !defined(AFX_GLBITMAPMODIFIER_H__3C6E8BBF_1AB2_4847_BEB1_934C97C76F05__INCLUDED_)
