////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2011
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

#if !defined(AFX_ThreadCallback_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)
#define AFX_ThreadCallback_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_

#include <engine/ThreadCallbackI.h>
#include <list>

class ThreadCallback
{
public:
	ThreadCallback();
	virtual ~ThreadCallback();

	void addCallback(ThreadCallbackI *callback);
	void addCallbackSync(ThreadCallbackI &callback);

	void processCallbacks();
protected:
	bool callbackOutstanding_;
	boost::mutex callbackMutex_;
	std::list<ThreadCallbackI *> callbacks_;
};

#endif // !defined(AFX_ThreadCallback_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)
