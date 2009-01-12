////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#if !defined(AFX_PROGRESSCOUNTER_H__4FA8EFA4_46AC_4790_9F55_B47AEF18EFEC__INCLUDED_)
#define AFX_PROGRESSCOUNTER_H__4FA8EFA4_46AC_4790_9F55_B47AEF18EFEC__INCLUDED_

#include <lang/LangString.h>

class ProgressCounterI
{
public:
	virtual ~ProgressCounterI();

	virtual void progressChange(const LangString &op, const float percentage) = 0;
};

class ProgressCounter  
{
public:
	ProgressCounter(ProgressCounterI *user = 0);
	virtual ~ProgressCounter();

	void setUser(ProgressCounterI *user) { user_ = user; }
	void setNewOp(const LangString &op);
	void setNewPercentage(float percentage);

	LangString &getCurrentOp() { return currentOp_; }
	const float getCurrentPercentage() { return currentPercentage_; }

protected:
	LangString currentOp_;
	float currentPercentage_;
	ProgressCounterI *user_;

};

#endif // !defined(AFX_PROGRESSCOUNTER_H__4FA8EFA4_46AC_4790_9F55_B47AEF18EFEC__INCLUDED_)