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
#include <tank/Tank.h>
#include <stdlib.h>

#define ADDBYTEVECTOR(x, y, z) \
	availableColors_.push_back(new Vector(float(x) / 255.0f, float(y) / 255.0f, float(z) / 255.0f));

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
	ADDBYTEVECTOR(0x7F, 0xFF, 0xD4)
	ADDBYTEVECTOR(0xFF, 0xE4, 0xC4)
	ADDBYTEVECTOR(0xDE, 0xB8, 0x87)
	ADDBYTEVECTOR(0xD2, 0x69, 0x1E)
	ADDBYTEVECTOR(0x8B, 0x00, 0x8B)
	ADDBYTEVECTOR(0x99, 0x32, 0xCC)
	ADDBYTEVECTOR(0x8D, 0xBC, 0x8F)
	ADDBYTEVECTOR(0x00, 0xDE, 0xD1)
	ADDBYTEVECTOR(0x00, 0xBF, 0xFF)
	ADDBYTEVECTOR(0xB2, 0x22, 0x22)
	ADDBYTEVECTOR(0xFF, 0x00, 0xFF)
	ADDBYTEVECTOR(0xFF, 0xD7, 0x00)
	ADDBYTEVECTOR(0xAD, 0xD8, 0xE6)
	ADDBYTEVECTOR(0xFF, 0xB6, 0xC1)
	ADDBYTEVECTOR(0xFA, 0xFA, 0xD2)
	ADDBYTEVECTOR(0xFF, 0x45, 0x00)
	ADDBYTEVECTOR(0xDA, 0xA5, 0x20)
	ADDBYTEVECTOR(0xFF, 0xFF, 0x20)
	ADDBYTEVECTOR(0xFF, 0x00, 0x20)
	ADDBYTEVECTOR(0x2E, 0x8B, 0x57)
	ADDBYTEVECTOR(0xBD, 0xB7, 0x6B)
	ADDBYTEVECTOR(0xFF, 0x8C, 0x00)
	ADDBYTEVECTOR(0x00, 0x22, 0xFF)
	ADDBYTEVECTOR(0x7F, 0xFF, 0x00)
}

TankColorGenerator::~TankColorGenerator()
{
}

Vector &TankColorGenerator::getNextColor(std::map<unsigned int, Tank *> &tanks)
{
	std::vector<Vector*> leftColors;
	std::list<Vector*>::iterator coloritor;
	for (coloritor = availableColors_.begin();
		coloritor != availableColors_.end();
		coloritor++)
	{
		Vector *color = (*coloritor);
		bool found = false;

		std::map<unsigned int, Tank *>::iterator tankitor;
		for (tankitor = tanks.begin();
			tankitor != tanks.end();
			tankitor++)
		{
			Tank *tank = (*tankitor).second;
			if (tank->getColor() == *color)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			leftColors.push_back(color);
		}
	}

	if (!leftColors.empty())
	{
		Vector *color = leftColors[rand() % leftColors.size()];
		return *color;
	}

	static Vector defaultColor(0.8f, 0.8f, 0.8f);
	return defaultColor;
}
