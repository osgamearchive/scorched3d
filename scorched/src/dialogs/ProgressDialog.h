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

#if !defined(__INCLUDE_ProgressDialogh_INCLUDE__)
#define __INCLUDE_ProgressDialogh_INCLUDE__

#include <common/ProgressCounter.h>
#include <common/FileLines.h>
#include <GLW/GLWLabel.h>
#include <GLW/GLWProgress.h>
#include <GLW/GLWWindow.h>
#include <GLEXT/GLTexture.h>

class ProgressDialog : 
	public GLWWindow,
	public ProgressCounterI,
	public ProgressCounter
{
public:
	static ProgressDialog *instance();

	virtual void progressChange(const char *op, const float percentage);
	virtual void draw();

	void changeTip();
	void setIcon(const char *iconName);
	void setProgress(const char *op, const float percentage);

protected:
	static ProgressDialog *instance_;
	GLTexture wait_;
	GLTexture icon_;
	GLWLabel *progressLabel_;
	GLWProgress *progress_;
	FileLines tips_;
	std::string tip1_, tip2_;

private:
	ProgressDialog();
	virtual ~ProgressDialog();
};


#endif
