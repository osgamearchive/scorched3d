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

#if !defined(AFX_WATERMAPSURROUND_H__625E6F81_8391_48B7_9477_77E2EDC9C73E__INCLUDED_)
#define AFX_WATERMAPSURROUND_H__625E6F81_8391_48B7_9477_77E2EDC9C73E__INCLUDED_

class WaterMapSurround  
{
public:
	WaterMapSurround(int width, int widthMult);
	virtual ~WaterMapSurround();

	void draw();

protected:
	int width_, widthMult_;
	int triangles_;

	void drawPoint(Vector &start, Vector &diff, 
		int numberX, int maxX,
		int numberY, int maxY);
	void generateList();
};

#endif // !defined(AFX_WATERMAPSURROUND_H__625E6F81_8391_48B7_9477_77E2EDC9C73E__INCLUDED_)
