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

#if !defined(__INCLUDE_GLWToolTiph_INCLUDE__)
#define __INCLUDE_GLWToolTiph_INCLUDE__

#include <engine/GameStateI.h>
#include <string>
#include <list>

class GLWToolTip;
class GLWTipI
{
public:
	virtual ~GLWTipI();

	virtual void populateCalled(unsigned int id) = 0;
};

class GLWTip
{
public:
	friend class GLWToolTip;
	GLWTip(const char *title = "", const char *text = "");
	virtual ~GLWTip();

	// Used to set the title and text of the tooltip
	void setText(const char *title, const char *text);

	// Called just before the tooltip is shown
	// can be used to dynamically populate the title and text fields
	virtual void populate();

	void setHandler(GLWTipI *handler) { handler_ = handler; }

	// Accessors
	const char *getTitle() { return title_.c_str(); }
	std::list<char *> &getTexts() { return texts_; }
	float getTextWidth() { return textWidth_; }
	float getTextHeight() { return textHeight_; }
	virtual float getX();
	virtual float getY();
	virtual float getW();
	virtual float getH();
	unsigned int getId() { return id_; }

protected:
	GLWTipI *handler_;
	float x, y;
	float w, h;

	unsigned int id_;
	static unsigned int nextId_;
	float textWidth_;
	float textHeight_;
	std::string text_;
	std::string title_;
	std::list<char *> texts_;

};

class GLWToolTip : public GameStateI
{
public:
	static GLWToolTip *instance();

	bool addToolTip(GLWTip *tip, float x, float y, float w, float h);

	// Inherited from GameStateI
	virtual void simulate(const unsigned state, float frameTime);
	virtual void draw(const unsigned state);

protected:
	static GLWToolTip *instance_;
	GLWTip *currentTip_;
	GLWTip *lastTip_;
	float timePasted_;

private:
	GLWToolTip();
	virtual ~GLWToolTip();
};

#endif
