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


#include <tank/TankColorGenerator.h>

#define ADDBYTEVECTOR(x, y, z) \
	availableColors_.push_back(Vector(float(x) / 255.0f, float(y) / 255.0f, float(z) / 255.0f));

TankColorGenerator *TankColorGenerator::instance_ = 0;

TankColorGenerator *TankColorGenerator::instance()
{
	if (!instance_)
	{
		instance_ = new TankColorGenerator;
	}
	return instance_;
}

TankColorGenerator::TankColorGenerator()
{
	ADDBYTEVECTOR(255, 27, 27)
	ADDBYTEVECTOR(102, 253, 27)
	ADDBYTEVECTOR(151, 44, 178)
	ADDBYTEVECTOR(250, 240, 15)
	ADDBYTEVECTOR(15, 249, 246)
	ADDBYTEVECTOR(229, 11, 180)
	ADDBYTEVECTOR(253, 133, 27)
	ADDBYTEVECTOR(76, 153, 38)
	ADDBYTEVECTOR(66,37, 213)
	ADDBYTEVECTOR(27, 255,27)
	ADDBYTEVECTOR(253, 102,27)
	ADDBYTEVECTOR(38, 133, 153)
	ADDBYTEVECTOR(249,15, 246)
	ADDBYTEVECTOR(11, 229, 180)
	ADDBYTEVECTOR(133, 253,27)
	ADDBYTEVECTOR(165,80,41)
	ADDBYTEVECTOR(37,66, 213)
	ADDBYTEVECTOR(27, 253, 102)
	ADDBYTEVECTOR(250,15, 240)
	ADDBYTEVECTOR(229, 180,11)
	ADDBYTEVECTOR(253,27, 133)
	ADDBYTEVECTOR(133, 27, 253)
	ADDBYTEVECTOR(191, 93, 47)
	ADDBYTEVECTOR(66, 213, 37)
	ADDBYTEVECTOR(66, 213, 213)
	ADDBYTEVECTOR(255, 27, 27)
	ADDBYTEVECTOR(102, 253, 27)
	ADDBYTEVECTOR(151, 44, 178)
	ADDBYTEVECTOR(250, 240, 15)
	ADDBYTEVECTOR(15, 249, 246)
	ADDBYTEVECTOR(229, 11, 180)
	ADDBYTEVECTOR(253, 133, 27)
	ADDBYTEVECTOR(76, 153, 38)
	ADDBYTEVECTOR(66,37, 213)
	ADDBYTEVECTOR(27, 255,27)
	ADDBYTEVECTOR(253, 102,27)
	ADDBYTEVECTOR(38, 133, 153)
	ADDBYTEVECTOR(249,15, 246)
	ADDBYTEVECTOR(11, 229, 180)
	ADDBYTEVECTOR(133, 253,27)
	ADDBYTEVECTOR(165,80,41)
	ADDBYTEVECTOR(37,66, 213)
	ADDBYTEVECTOR(27, 253, 102)
	ADDBYTEVECTOR(250,15, 240)
	ADDBYTEVECTOR(229, 180,11)
	ADDBYTEVECTOR(253,27, 133)
	ADDBYTEVECTOR(133, 27, 253)
	ADDBYTEVECTOR(191, 93, 47)
	ADDBYTEVECTOR(66, 213, 37)
	ADDBYTEVECTOR(66, 213, 213)
}

TankColorGenerator::~TankColorGenerator()
{
}

Vector &TankColorGenerator::getNextColor()
{
	static Vector nextColor(0.8f, 0.8f, 0.8f);
	nextColor = Vector();
	if (!availableColors_.empty())
	{
		nextColor = availableColors_.front();
		availableColors_.pop_front();
	}
	return nextColor;
}

void TankColorGenerator::returnColor(Vector &color)
{
	availableColors_.push_back(color);
}
