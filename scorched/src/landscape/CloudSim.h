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


#include <stdio.h>
#include <stdlib.h>

// THIS CLASS IS NOT USED (YET)

class Cell
{
public:
	Cell() 
		: 
		AHum_(0), AAct_(0), ACld_(0),
		BHum_(0), BAct_(0), BCld_(0) 
		{ 
			
		}

	enum UseSet
	{
		SetA,
		SetB
	};
	void setHum(UseSet calcValue)
	{
		if (calcValue == SetA)
		{
			AHum_ = 1;
			AAct_ = 1;
		}
		else
		{
			BHum_ = 1;
			BAct_ = 1;
		}
	}

	bool getCurrentCld(UseSet calcValue)
		{
			if (calcValue == SetA)
			{
				return (ACld_ & 1);
			}
			else
			{
				return (BCld_ & 1);
			}
		}

	bool getPrevAct(UseSet calcValue)
		{
			if (calcValue == SetA)
			{
				return (BAct_ & 1);
			}
			else
			{
				return (AAct_ & 1);
			}
		}

	void calcNextValue(UseSet calcValue,
					   bool fnAct)
		{
			if (calcValue == SetB)
			{
				BHum_ = (AHum_ & !AAct_) | ((rand() < RAND_MAX / 100)?1:0);
				BCld_ = (ACld_ | AAct_) & ((rand() < RAND_MAX / 2)?1:0);
				//BAct_ = !AAct_ & AHum_ & (fnAct?1:0);
				BAct_ = (!AAct_ & AHum_ & (fnAct?1:0)) | ((rand() < RAND_MAX / 100)?1:0);
			}
			else
			{
				AHum_ = (BHum_ & !BAct_) | ((rand() < RAND_MAX / 100)?1:0);
				ACld_ = (BCld_ | BAct_) & ((rand() < RAND_MAX / 2)?1:0);
				AAct_ = (!BAct_ & BHum_ & (fnAct?1:0)) | ((rand() < RAND_MAX / 100)?1:0);
			}
		}

protected:
	// NOTE: Each value is only 1 bit long
	char AHum_ : 1;
	char AAct_ : 1;
	char ACld_ : 1;
	char BHum_ : 1;
	char BAct_ : 1;
	char BCld_ : 1;
};

class CloudSim
{
public:
	CloudSim();
	~CloudSim();

	void simulate();
	  
protected:
	Cell::UseSet set_;
	Cell *cells_;

};
