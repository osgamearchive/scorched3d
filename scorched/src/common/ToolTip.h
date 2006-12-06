////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_ToolTiph_INCLUDE__)
#define __INCLUDE_ToolTiph_INCLUDE__

#include <string>

class GLWToolTip;
class ToolTipI
{
public:
	virtual ~ToolTipI();

	virtual void populateCalled(unsigned int id) = 0;
};

class ToolTip
{
public:
	friend class GLWToolTip;
	ToolTip(const char *title = "", const char *text = "");
	virtual ~ToolTip();

	// Used to set the title and text of the tooltip
	void setText(const char *title, const char *text);

	// Called just before the tooltip is shown
	// can be used to dynamically populate the title and text fields
	virtual void populate();

	void setHandler(ToolTipI *handler) { handler_ = handler; }

	unsigned int getId() { return id_; }
	const char *getText() { return text_.c_str(); }
	const char *getTitle() { return title_.c_str(); }

protected:
	ToolTipI *handler_;
	unsigned int id_;
	static unsigned int nextId_;
	std::string text_;
	std::string title_;

};
#endif // __INCLUDE_ToolTiph_INCLUDE__