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

#if !defined(__INCLUDE_GLWChannelViewh_INCLUDE__)
#define __INCLUDE_GLWChannelViewh_INCLUDE__

#include <GLW/GLWidget.h>
#include <GLW/GLWIconButton.h>
#include <client/ClientChannelManagerI.h>
#include <common/KeyboardKey.h>
#include <common/Vector.h>
#include <list>

class GLWChannelViewEntry
{
public:
	Vector color;
	std::string text;
	float timeRemaining;
};

class GLWChannelViewI
{
public:
	virtual ~GLWChannelViewI();

	virtual void channelsChanged(unsigned int id) = 0;
};

class GLWChannelView : 
	public GLWidget,
	public ClientChannelManagerI,
	public GLWButtonI
{
public:
	class ChannelEntry
	{
	public:
		Vector color;
		std::string channel;
		unsigned int id;
	};

	GLWChannelView();
	virtual ~GLWChannelView();

	bool getParentSized() { return parentSized_; }
	bool initFromXMLInternal(XMLNode *node);

	std::list<ChannelEntry> &getCurrentChannels() { return currentChannels_; }
	std::list<std::string> &getAvailableChannels() { return availableChannels_; }
	ChannelEntry *getChannel(const char *channelName);

	void joinChannel(const char *channelName);
	void leaveChannel(const char *channelName);

	void setHandler(GLWChannelViewI *handler) { handler_ = handler; };

	// GLWidget
	virtual void draw();
	virtual void simulate(float frameTime);
	virtual bool initFromXML(XMLNode *node);
	virtual void mouseDown(int button, float x, float y, bool &skipRest);
	virtual void mouseUp(int button, float x, float y, bool &skipRest);
	virtual void mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);
	virtual void setX(float x);
	virtual void setY(float y);

	// ClientChannelManagerI
	virtual void channelText(ChannelText &text);
	virtual void registeredForChannels(
		std::list<std::string> &registeredChannels,
		std::list<std::string> &availableChannels);

	// ButtonI
	virtual void buttonDown(unsigned int id);

	REGISTER_CLASS_HEADER(GLWChannelView);

protected:
	GLWChannelViewI *handler_;
	GLWIconButton upButton_;
	GLWIconButton downButton_;
	GLWIconButton resetButton_;
	GLTexture upTexture_;
	GLTexture downTexture_;
	GLTexture resetTexture_;

	unsigned int lastChannelId_;
	bool init_;
	bool alignTop_, parentSized_;
	bool splitLargeLines_, allowScroll_;
	int lineDepth_;
	int scrollPosition_;
	float displayTime_;
	float fontSize_, outlineFontSize_;
	int visibleLines_, totalLines_;
	int currentVisible_;
	std::list<GLWChannelViewEntry> textLines_;
	KeyboardKey *scrollUpKey_;
	KeyboardKey *scrollDownKey_;
	KeyboardKey *scrollResetKey_;
	std::map<std::string, Vector> channelColors_;
	std::list<std::string> startupChannels_;
	std::list<ChannelEntry> currentChannels_;
	std::list<std::string> availableChannels_;
	std::string textSound_;

	void addInfo(const char *channel, const char *text);
	void formCurrentChannelList(std::list<std::string> &result);
	int splitLine(const char *message, std::string &result);
};

#endif // __INCLUDE_GLWChannelViewh_INCLUDE__