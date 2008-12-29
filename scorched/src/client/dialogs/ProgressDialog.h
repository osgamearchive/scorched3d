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

#if !defined(__INCLUDE_ProgressDialogh_INCLUDE__)
#define __INCLUDE_ProgressDialogh_INCLUDE__

#include <common/ProgressCounter.h>
#include <common/FileLines.h>
#include <GLW/GLWWindow.h>
#include <GLEXT/GLTexture.h>

class ProgressDialog : 
	public GLWWindow,
	public ProgressCounterI,
	public ProgressCounter
{
public:
	static ProgressDialog *instance();

	virtual void progressChange(const LangString &op, const float percentage);
	virtual void draw();

	void changeTip();
	void setIcon(const std::string &iconName);

protected:
	static ProgressDialog *instance_;

	GLTexture icon_;
	GLTexture bar1_, bar2_;
	FileLines tips_;
	LangString progressText_;
	float progressPercentage_;

private:
	ProgressDialog();
	virtual ~ProgressDialog();
};

class ProgressDialogSync :
	public ProgressCounterI,
	public ProgressCounter
{
public:
	static ProgressDialogSync *instance();

	virtual void progressChange(const LangString &op, const float percentage);

protected:
	static ProgressDialogSync *instance_;

private:
	ProgressDialogSync();
	virtual ~ProgressDialogSync();
};

#endif
