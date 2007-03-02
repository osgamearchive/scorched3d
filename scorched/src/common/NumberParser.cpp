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


// NumberParser.cpp: implementation of the NumberParser class.
//
//////////////////////////////////////////////////////////////////////

#include <common/RandomGenerator.h>
#include <engine/ActionController.h>
#include <common/NumberParser.h>
#include <string>
#include <list>
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NumberParser::NumberParser()
{

}

NumberParser::NumberParser(const char *expression)
{
        expression_ = expression;
}

NumberParser::NumberParser(float value)
{
        this->setExpression(value);
}

NumberParser::NumberParser(int value)
{
        this->setExpression(value);
}

NumberParser::~NumberParser()
{

}


bool NumberParser::getOperands()
{
	operands_.clear();
        int count = 0;
	int nextPos = 0;
        std::string value;
        int pos = expression_.find('(',0);
	if (pos == std::string::npos)
	{
		//value = expression_.substr(pos + 1, nextPos - pos + 1);
		operands_.push_back(atof(expression_.c_str()));
		return true;
	}

	pos += 1;
        while (pos < expression_.length())
        {
                nextPos = expression_.find_first_of(",)", pos);
                if (nextPos == std::string::npos)
                        nextPos = expression_.length() -1;
                value = expression_.substr(pos, nextPos - pos);
                operands_.push_back(atof(value.c_str()));
                pos = nextPos + 1;
        }
        return true;
}


bool NumberParser::setExpression(const char *expression)
{
        expression_ = expression;
        // test to ensure it's valid! TODO
	this->getOperands();
        return true;
}

bool NumberParser::setExpression(int value)
{
        std::ostringstream convert;
        if (!(convert << value))
                return false;
        expression_ = convert.str();
	this->getOperands();
        return true;
}

bool NumberParser::setExpression(float value)
{
        std::ostringstream convert;
        if (!(convert << value))
                return false;
        expression_ = convert.str();
	this->getOperands();

        return true;
}


float NumberParser::getValue(ScorchedContext &context) //RandomGenerator &generator)
{
        // Examples: 10, RANGE(1,10)
        float value = 0;
        // Constant
        if (operands_.size() == 1)
                return operands_.front();

        step_ = 0;
        std::list<float>::iterator itor;
        itor = operands_.begin();
        RandomGenerator &random = context.actionController->getRandom();

        if (expression_.find("RANGE",0) != std::string::npos)
        {
                min_ = *itor;
                max_ = *(++itor);
                if (operands_.size() >= 3)
                        step_ = *(++itor);

                if (step_ == 0)
                        value = random.getRandFloat() * (max_ - min_) + min_;
                else
                        value = float((int(random.getRandFloat() * (max_ - min_) /  step_ )) + min_) * step_;
                return value;
        }

        else if (expression_.find("DISTRIBUTION",0) != std::string::npos)
        {
                if (operands_.size() < 2)
                        return 0;       // need better error handling
                int operandNo = int(random.getRandFloat() * float(operands_.size()));
                for (int i = 0; i <= operandNo; i++) itor++;
                value = *itor;
                return value;
        }

        // TODO - add error dialog
        dialogExit("NumberParser",
	        formatString("Invalid float expression: \"%s\"",
                expression_.c_str()));
}

unsigned int NumberParser::getUInt(ScorchedContext &context)
{
	unsigned int value = 0;
	// Constant
	if (operands_.size() == 1)
		return (unsigned int) operands_.front();
	
	step_ = 0;
	std::list<float>::iterator itor;
	itor = operands_.begin();

	RandomGenerator &random = context.actionController->getRandom();

	if (expression_.find("RANGE",0) != std::string::npos)
	{
		min_ = *itor; 
		max_ = *(++itor); 
		if (operands_.size() >= 3)
			step_ = *(++itor);
		
		if (step_ == 0)
			value = random.getRandUInt() % (int) (max_ - min_) + (int) min_;
		else
			value = random.getRandUInt() % (int) (max_ - min_) / (int) step_ * (int) step_ + (int) min_;

		return (unsigned int) value;
	}
	else if (expression_.find("DISTRIBUTION",0) != std::string::npos)
	{
		if (operands_.size() < 2)
			return 0;
		int operandNo = random.getRandUInt() % operands_.size();
		for (int i = 0; i <= operandNo; i++) itor++;
		return (unsigned int) *itor;
	}
	dialogExit("NumberParser",
		formatString("Invalid integer expression: \"%s\"",
		expression_.c_str()));

}

int NumberParser::getInt(ScorchedContext &context)
{
	return (int) this->getValue(context);
}

