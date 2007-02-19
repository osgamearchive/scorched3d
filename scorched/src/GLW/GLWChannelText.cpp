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

#include <GLW/GLWChannelText.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWPanel.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLConsole.h>
#include <GLEXT/GLPng.h>
#include <GLEXT/GLBitmap.h>
#include <XML/XMLParser.h>
#include <client/ScorchedClient.h>
#include <client/ClientChannelManager.h>
#include <tank/TankContainer.h>
#include <tank/TankColorGenerator.h>
#include <common/ToolTip.h>
#include <common/Defines.h>
#include <common/Keyboard.h>

REGISTER_CLASS_SOURCE(GLWChannelText);

GLWChannelText::GLWChannelText() :
	ctime_(0.0f), cursor_(false), maxTextLen_(0), 
	visible_(false),
	button_(x_ + 2.0f, y_ + 4.0f, 12.0f, 12.0f)
{
	view_.setHandler(this);
	button_.setHandler(this);
	button_.setToolTip(new ToolTip("Chat", "Select channels, mute players..."));
}

GLWChannelText::~GLWChannelText()
{
}

void GLWChannelText::simulate(float frameTime)
{
	view_.simulate(frameTime);
	button_.simulate(frameTime);

	if (!visible_) return;

	ctime_ += frameTime;
	if (ctime_ > 0.5f)
	{
		ctime_ = 0.0f;
		cursor_ = !cursor_;
	}
}

void GLWChannelText::draw()
{
	view_.draw();
	if (view_.getParentSized())
	{
		setW(view_.getW());
		setH(view_.getH());
	}

	if (!buttonTexture_.textureValid())
	{
		GLPng buttonImg(getDataFile("data/windows/arrow_r.png"), true);
		buttonTexture_.create(buttonImg, GL_RGBA, false);
		button_.setTexture(&buttonTexture_);
		GLBitmap map(getDataFile("data/windows/white.bmp"));
		colorTexture_.create(map);
	}

	button_.draw();

	if (!visible_) return;

	GLWidget::draw();
	glColor4f(0.4f, 0.6f, 0.8f, 0.6f);

	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x_ + 25.0f, y_ + 5.0f);
			glVertex2f(x_ + 25.0f, y_);
			drawRoundBox(x_ + 15.0f, y_, w_ - 15.0f, h_, 10.0f);
			glVertex2f(x_ + 25.0f, y_);
		glEnd();
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		drawRoundBox(x_ + 15.0f, y_, w_ - 15.0f, h_, 10.0f);
	glEnd();

	const char *text = formatString("%u. [%s] : %s%s", 
		channelEntry_.id, channelEntry_.channel.c_str(),
		text_.c_str(), cursor_?" ":"_");

	GLWFont::instance()->getLargePtFont()->drawWidthRhs(
		w_ - 25.0f,
		channelEntry_.color, 12,
		x_ + 20.0f, y_ + 5.0f, 0.0f, 
		text);
}

void GLWChannelText::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	view_.keyDown(buffer, keyState, history, hisCount, skipRest);
	if (skipRest) return;

	if (visible_) skipRest = true;
	for (int i=0; i<hisCount; i++)
	{
		char c = history[i].representedKey;
		unsigned int dik = history[i].sdlKey;

		if (!visible_)
		{
			processNotVisibleKey(c, dik, skipRest);
		}
		else
		{
			processVisibleKey(c, dik);
		}
	}
	if (visible_) skipRest = true;
}

void GLWChannelText::processNotVisibleKey(char c, unsigned int dik, bool &skipRest)
{
	// Check all of the key entries to see if the channel needs to be displayed
	std::map<KeyboardKey *, std::string>::iterator keyItor;
	for (keyItor = keys_.begin();
		keyItor != keys_.end();
		keyItor++)
	{
		KeyboardKey *key = keyItor->first;
		std::string &channel = keyItor->second;

		// Check in the list of keys to see if we need to display 
		std::vector<KeyboardKey::KeyEntry> &talkKeys = key->getKeys();
		std::vector<KeyboardKey::KeyEntry>::iterator itor;
		for (itor = talkKeys.begin();
			itor != talkKeys.end();
			itor++)
		{
			KeyboardKey::KeyEntry &entry = *itor;
			if (dik == entry.key)
			{
				setVisible(true);
				skipRest = true;

				text_ = "";
				if (!channel.empty())
				{
					GLWChannelView::ChannelEntry *channelEntry = view_.getChannel(
						channel.c_str());
					if (channelEntry) channelEntry_ = *channelEntry;
				}

				if (entry.key == SDLK_SLASH) text_ = "/";
				else if (entry.key == SDLK_BACKSLASH) text_ = "\\";

				break;
			}
		}
	}

	if (visible_)
	{
		if (!checkCurrentChannel()) setVisible(false);
	}
}

void GLWChannelText::processVisibleKey(char c, unsigned int dik)
{
	if (dik == SDLK_BACKSPACE || dik == SDLK_DELETE)
	{
		if (!text_.empty())
		{
			text_ = text_.substr(0, text_.length() - 1);
		}
	}
	else if (dik == SDLK_ESCAPE)
	{
		if (!text_.empty())
		{
			text_ = "";
		}
		else
		{
			setVisible(false);
		}
	}
	else if (dik == SDLK_RETURN)
	{
		if (!text_.empty())
		{
			char *text = (char *) text_.c_str();
			if (text_[0] == '\\' || text_[0] == '/')
			{
				processSpecialText();
			}
			else
			{
				processNormalText();
			}
		}
		setVisible(false);
		text_ = "";
	}
	else if (c >= ' ')
	{
		if ((maxTextLen_==0) || ((int) text_.size() < maxTextLen_))
		{
			if ((text_[0] == '\\' || text_[0] == '/') && c == ' ')
			{
				processSpecialText();
			}
			else
			{
				text_ += c;
			}
		}
	}
}

void GLWChannelText::setVisible(bool visible)
{
	visible_ = visible;

	if (0 == strcmp("GLWWindow", getParent()->getClassName()))
	{
		GLWWindow *window = (GLWWindow *) getParent();
		window->setWindowLevel(visible_?40000:200000);
		GLWWindowManager::instance()->sortWindowLevels();
	}
}

void GLWChannelText::processNormalText()
{
	ChannelText text(channelEntry_.channel.c_str(), text_.c_str());
	ClientChannelManager::instance()->sendText(text);
}

void GLWChannelText::processSpecialText()
{
	GLWChannelView::ChannelEntry *channelEntry = 
		view_.getChannel(&text_[1]);
	if (channelEntry)
	{
		text_ = "";
		channelEntry_ = *channelEntry;
	}
}

void GLWChannelText::mouseDown(int button, float x, float y, bool &skipRest)
{
	button_.mouseDown(button, x, y, skipRest);
	view_.mouseDown(button, x, y, skipRest);
}

void GLWChannelText::mouseUp(int button, float x, float y, bool &skipRest)
{
	button_.mouseUp(button, x, y, skipRest);
	view_.mouseUp(button, x, y, skipRest);
}

void GLWChannelText::mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest)
{
	button_.mouseDrag(button, mx, my, x, y, skipRest);
	view_.mouseDrag(button, mx, my, x, y, skipRest);
}

void GLWChannelText::setParent(GLWPanel *parent)
{
	GLWidget::setParent(parent);
	button_.setParent(parent);
	view_.setParent(parent);
}

void GLWChannelText::setX(float x)
{
	GLWidget::setX(x);
	button_.setX(x + 2.0f);
	view_.setX(x);
}

void GLWChannelText::setY(float y)
{
	GLWidget::setY(y);
	button_.setY(y + 4.0f);
	view_.setY(y + 20.0f);
}

void GLWChannelText::setW(float w)
{
	GLWidget::setW(w);
	view_.setW(w);
}

void GLWChannelText::setH(float h)
{
	GLWidget::setH(20.0f);
	view_.setH(h - 20.0f);
}

static enum
{
	eMuteSelectorStart = 1000,
	eWhisperSelectorStart = 2000,
	eJoinSelectorStart = 3000,
	eLeaveSelectorStart = 4000,
	eSelectSelectorStart = 5000,
	eColorSelectorStart = 6000
};

void GLWChannelText::buttonDown(unsigned int id)
{
	static ToolTip muteTooltip("Ignore", "Ignore chat from another player (mute)");
	static ToolTip whisperTooltip("Whisper", "Send private chat to another player");
	static ToolTip joinTooltip("Join Channel", "Join another chat channel.\n"
		"You will be able to see messages sent on this channel");
	static ToolTip leaveTooltip("Leave Channel", "Leave a current chat channel.\n"
		"You will stop recieving messages sent on this channel");
	static ToolTip selectTooltip("Select Channel", "Select the current channel.\n"
		"This is the channel you will send messages on.");
	static ToolTip colorTooltip("Channel Color", "Change the color of the current channel.");

	GLWSelectorEntry mute("Ignore", &muteTooltip);
	GLWSelectorEntry whisper("Whisper", &whisperTooltip);
	GLWSelectorEntry bar("---");
	GLWSelectorEntry joinChannel("Join Channel", &joinTooltip);
	GLWSelectorEntry leaveChannel("Leave Channel", &leaveTooltip);
	GLWSelectorEntry selectChannel("Select Channel", &selectTooltip);
	GLWSelectorEntry colorChannel("Channel Color", &colorTooltip);

	int i = 0;
	std::map<unsigned int, Tank *> &tanks = ScorchedClient::instance()->
		getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator tankItor;
	for (tankItor = tanks.begin();
		tankItor != tanks.end();
		tankItor++, i++)
	{
		Tank *tank = (*tankItor).second;
		mute.getPopups().push_back(GLWSelectorEntry(tank->getName(), 
			0, false, 0, (void *) eMuteSelectorStart, tank->getName()));
		whisper.getPopups().push_back(GLWSelectorEntry(tank->getName(),
			0, false, 0, (void *) eWhisperSelectorStart, tank->getName()));
	}
	std::list<GLWChannelView::ChannelEntry> &channels = view_.getCurrentChannels();
	std::list<GLWChannelView::ChannelEntry>::iterator channelItor;
	for (channelItor = channels.begin();
		channelItor != channels.end();
		channelItor++)
	{
		GLWChannelView::ChannelEntry &channel = (*channelItor);
		leaveChannel.getPopups().push_back(GLWSelectorEntry(
			formatString("%u. %s", channel.id, channel.channel.c_str()),
			0, false, 0, 
			(void *) eLeaveSelectorStart, channel.channel.c_str()));
		selectChannel.getPopups().push_back(GLWSelectorEntry(
			formatString("%u. %s", channel.id, channel.channel.c_str()),
			0, (channelEntry_.channel == channel.channel), 0, 
			(void *) eSelectSelectorStart, channel.channel.c_str()));
	}
	std::list<std::string> &available = view_.getAvailableChannels();
	std::list<std::string>::iterator availableItor;
	for (availableItor = available.begin();
		availableItor != available.end();
		availableItor++)
	{
		std::string &channel = *availableItor;
		joinChannel.getPopups().push_back(GLWSelectorEntry(channel.c_str(),
			0, false, 0, (void *) eJoinSelectorStart, channel.c_str()));
	}
	std::vector<Vector *> &colors = TankColorGenerator::instance()->getAllColors();
	std::vector<Vector *>::iterator colorItor;
	for (colorItor = colors.begin();
		colorItor != colors.end();
		colorItor++)
	{
		Vector *color = *colorItor;
		GLWSelectorEntry entry("", 0, false, &colorTexture_, (void *) eColorSelectorStart);
		entry.getColor() = *color;
		entry.getTextureWidth() = 32;
		colorChannel.getPopups().push_back(entry);
	}

	std::list<GLWSelectorEntry> topLevel;
	topLevel.push_back(mute);
	topLevel.push_back(whisper);
	topLevel.push_back(bar);
	topLevel.push_back(joinChannel);
	topLevel.push_back(leaveChannel);
	topLevel.push_back(selectChannel);
	topLevel.push_back(colorChannel);

	float x = (float) ScorchedClient::instance()->getGameState().getMouseX();
	float y = (float) ScorchedClient::instance()->getGameState().getMouseY();
	GLWSelector::instance()->showSelector(this, x, y, topLevel);
}

void GLWChannelText::itemSelected(GLWSelectorEntry *entry, int position)
{
	int type = int(entry->getUserData());
	switch (type)
	{
	case eMuteSelectorStart:

		break;
	case eWhisperSelectorStart:

		break;
	case eJoinSelectorStart:
		view_.joinChannel(entry->getDataText());
		break;
	case eLeaveSelectorStart:
		view_.leaveChannel(entry->getDataText());
		break;
	case eSelectSelectorStart:
		{
			GLWChannelView::ChannelEntry *channel = 
				view_.getChannel(entry->getDataText());
			if (channel) channelEntry_ = *channel;
		}
		break;
	case eColorSelectorStart:
		{
			GLWChannelView::ChannelEntry *channel = 
				view_.getChannel(channelEntry_.channel.c_str());
			if (channel) 
			{
				channelEntry_.color = entry->getColor();
				channel->color = entry->getColor();
			}
		}
		break;
	}
}

void GLWChannelText::channelsChanged(unsigned int id)
{
	checkCurrentChannel();
}

bool GLWChannelText::checkCurrentChannel()
{
	// Just turned visible, check the current channel is valid
	GLWChannelView::ChannelEntry *channelEntry = view_.getChannel(
		channelEntry_.channel.c_str());				
	if (!channelEntry)
	{
		// Its not, try to find a valid channel
		if (!view_.getCurrentChannels().empty())
		{
			// Choose the first channel
			channelEntry_ = view_.getCurrentChannels().front();
		}
		else
		{
			// There is no valid channel
			return false;
		}
	}
	else 
	{
		// The current channel is valid
		channelEntry_ = *channelEntry;
	}	

	return true;
}

bool GLWChannelText::initFromXML(XMLNode *node)
{
	if (!GLWidget::initFromXML(node)) return false;
	if (!view_.initFromXMLInternal(node)) return false;

	XMLNode *keyNode;
	while (node->getNamedChild("talkkey", keyNode, false))
	{
		std::string key, channel;
		if (!keyNode->getNamedChild("key", key)) return false;
		if (!keyNode->getNamedChild("channel", channel)) return false;

		KeyboardKey *actualKey = Keyboard::instance()->getKey(key.c_str());
		if (!actualKey) return keyNode->returnError("Unknown key");

		keys_[actualKey] = channel;
	}

	if (!node->getNamedChild("defaultchannel", channelEntry_.channel)) return false;

	return true;
}
