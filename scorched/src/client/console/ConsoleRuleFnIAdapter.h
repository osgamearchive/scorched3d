////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ConsoleRULEFNIADAPTER_H__A8429FA2_3499_4F4A_95BC_9F94FC58C087__INCLUDED_)
#define AFX_ConsoleRULEFNIADAPTER_H__A8429FA2_3499_4F4A_95BC_9F94FC58C087__INCLUDED_

#include <common/OptionEntry.h>
#include <console/ConsoleRuleFn.h>
#include <string>

class ConsoleRuleFnIBooleanAdapter : 
	public ConsoleRuleFnI
{
public:
	ConsoleRuleFnIBooleanAdapter(Console &console, const std::string &name, bool &param);
	virtual ~ConsoleRuleFnIBooleanAdapter();

	// Inherited from ConsoleRuleFnI
	virtual bool getBoolParam(const std::string &name);
	virtual void setBoolParam(const std::string &name, bool value);

protected:
	Console &console_;
	ConsoleRuleFn *readRule_, *writeRule_;
	std::string name_;
	bool &param_;

};

class ConsoleRuleFnINumberAdapter : 
	public ConsoleRuleFnI
{
public:
	ConsoleRuleFnINumberAdapter(Console &console, const std::string &name, float &param);
	virtual ~ConsoleRuleFnINumberAdapter();

	// Inherited from ConsoleRuleFnI
	virtual float getNumberParam(const std::string &name);
	virtual void  setNumberParam(const std::string &name, float value);

protected:
	Console &console_;
	ConsoleRuleFn *readRule_, *writeRule_;
	std::string name_;
	float &param_;

};

class ConsoleRuleFnIOptionsAdapter :
	public ConsoleRuleFnI
{
public:
	ConsoleRuleFnIOptionsAdapter(Console &console, OptionEntry &entry, bool write = false);
	virtual ~ConsoleRuleFnIOptionsAdapter();

	// Inherited from ConsoleRuleFnI
	virtual bool getBoolParam(const std::string &name);
	virtual void setBoolParam(const std::string &name, bool value);
	virtual float getNumberParam(const std::string &name);
	virtual void  setNumberParam(const std::string &name, float value);
	virtual std::string getStringParam(const std::string &name);
	virtual void setStringParam(const std::string &name, const std::string &value);

protected:
	Console &console_;
	OptionEntry &entry_;
	ConsoleRuleFn *readRule_, *writeRule_;
};

#endif // !defined(AFX_ConsoleRULEFNIADAPTER_H__A8429FA2_3499_4F4A_95BC_9F94FC58C087__INCLUDED_)
