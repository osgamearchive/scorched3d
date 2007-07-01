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

#include <dialogs/NetworkSelectDialog.h>
#include <dialogs/MsgBoxDialog.h>
#include <GLW/GLWLabel.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWTranslate.h>
#include <graph/TextureStore.h>
#include <scorchedc/ServerBrowser.h>
#include <client/ClientParams.h>
#include <client/ClientMain.h>
#include <common/Defines.h>

static struct ColumnInfo
{
	GLWIconTable::Column col;
	const char *dataName;
}
cols[] = 
{
	"", 60 , "",
	"Server Name", 250, "servername",
	"Plyrs", 55, "noplayers",
	"Round", 50, "round",
	"Mod", 70, "mod",
	"Game Type", 160, "gametype"
};

NetworkSelectDialog *NetworkSelectDialog::instance_ = 0;

NetworkSelectDialog *NetworkSelectDialog::instance()
{
	if (!instance_)
	{
		instance_ = new NetworkSelectDialog;
	}
	return instance_;
}

NetworkSelectDialog::NetworkSelectDialog() : 
	GLWWindow("", 700.0f, 540.0f, 0, ""),
	totalTime_(0.0f), invalidateId_(0),
	okTex_(0), questionTex_(0),
	warningTex_(0), noentryTex_(0),
	tankTex_(0)
{
	std::list<GLWIconTable::Column> columns;
	for (int i=0; i<sizeof(cols)/sizeof(ColumnInfo); i++)
	{
		columns.push_back(cols[i].col);
	}

	iconTable_ = new GLWIconTable(10.0f, 70.0f, 680.0f, 400.0f, &columns, 20.0f);
	addWidget(iconTable_);
	iconTable_->setHandler(this);

	ok_ = (GLWTextButton *) addWidget(
		new GLWTextButton("Join Game", 560, 10, 130, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX));
	cancelId_ = addWidget(
		new GLWTextButton("Cancel", 450, 10, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
	refresh_ = (GLWTextButton *) addWidget(
		new GLWTextButton("Refresh", 595, 40, 95, this, 
		GLWButton::ButtonFlagCenterX));
	favourites_ = (GLWTextButton *) addWidget(
		new GLWTextButton("Add Favourite", 435, 40, 155, this, 
		GLWButton::ButtonFlagCenterX));

	ipaddress_ = (GLWTextBox *) addWidget(
		new GLWTextBox(170.0f, 510.0f, 300.0));
	addWidget(new GLWLabel(35.0f, 510.0f, "Connect To :"));
	ipaddress_->setHandler(this);

	password_ = (GLWTextBox *) addWidget(
		new GLWTextBox(170.0f, 480.0f, 300.0, 
		"", GLWTextBox::eFlagPassword));
	addWidget(new GLWLabel(50.0f, 480.0f, "Password :"));

	refreshType_ = (GLWDropDownText *) addWidget(
		new GLWDropDownText(490.0f, 480.0f, 150.0f));
	refreshType_->addText("Internet");
	refreshType_->addText("LAN");
	refreshType_->addText("Favourites");
	refreshType_->setCurrentText("Internet");
	refreshType_->setHandler(this);

	ipaddress_->setCurrent();
}

NetworkSelectDialog::~NetworkSelectDialog()
{

}

void NetworkSelectDialog::simulate(float frameTime)
{
	GLWWindow::simulate(frameTime);

	totalTime_ += frameTime;
	if (totalTime_ > 1.0f)
	{
		updateTable();
	}
}

void NetworkSelectDialog::drawIcon(GLTexture *tex, float &x, float y, const char *message)
{
	GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON);
	glColor3f(1.0f, 1.0f, 1.0f);
	tex->draw();

	float w = 18.0f;
	float h = 18.0f;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x + w, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x + w, y + h);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y + h);
	glEnd();

	if (GLWToolTip::instance()->addToolTip(&colToolTip_, 
		GLWTranslate::getPosX() + x, 
		GLWTranslate::getPosY() + y, 
		w, h))
	{
		colToolTip_.setText("Status Icon", message);
	}

	x += w;
}

GLTexture *NetworkSelectDialog::getTexture(int row, const char *&message)
{
	std::string pversion =
		ServerBrowser::instance()->getServerList().
			getEntryValue(row, "protocolversion");
	std::string version =
		ServerBrowser::instance()->getServerList().
			getEntryValue(row, "version");
	if (!serverCompatable(pversion, version))
	{
		message = "Incompatible version.";
		return noentryTex_;
	}

	std::string clients = 
		ServerBrowser::instance()->getServerList().
			getEntryValue(row, "noplayers");
	std::string maxclients = 
		ServerBrowser::instance()->getServerList().
			getEntryValue(row, "maxplayers");
	if (clients.size() > 0 &&
		0 == strcmp(clients.c_str(), maxclients.c_str()))
	{
		message = "Server is full.";
		return exclaimTex_;
	}
	
	std::string state = 
		ServerBrowser::instance()->getServerList().
			getEntryValue(row, "state");
	if (0 == strcmp(state.c_str(), "Waiting"))
	{
		message = "Game has not started.";
		return warningTex_;
	}
	if (0 == strcmp(state.c_str(), "Started"))
	{
		message = "Game in progress and spaces on server.";
		return okTex_;
	}

	message = "Cannot contact server.";
	return questionTex_;
}

void NetworkSelectDialog::drawColumn(unsigned int id, int row, int col, 
	float x, float y, float w)
{
	if (row < 0 || row >= ServerBrowser::instance()->getServerList().getNoEntries())
	{
		return;
	}

	const char *value = 0, *tipValue = 0;
	if (col == 0)
	{
		if (!okTex_)
		{
			okTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/ok.bmp")),
				formatString("%s", getDataFile("data/windows/mask.bmp")));
			questionTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/question.bmp")),
				formatString("%s", getDataFile("data/windows/mask.bmp")));
			warningTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/warn.bmp")),
				formatString("%s", getDataFile("data/windows/mask.bmp")));
			noentryTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/noentry.bmp")),
				formatString("%s", getDataFile("data/windows/mask.bmp")));
			exclaimTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/exclaim.bmp")),
				formatString("%s", getDataFile("data/windows/mask.bmp")));
			keyTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/key.bmp")),
				formatString("%s", getDataFile("data/windows/keya.bmp")),
				true);
			cogTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/cog.bmp")),
				formatString("%s", getDataFile("data/windows/coga.bmp")),
				true);
			tankTex_ = TextureStore::instance()->loadTexture(
				formatString("%s", getDataFile("data/windows/tank2s.bmp")));
		}

		const char *message = "None";
		GLTexture *tex = getTexture(row, message);
		drawIcon(tex, x, y, message);

		std::string key = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(row, "password");
		if (0 == strcmp(key.c_str(), "On"))
		{
			drawIcon(keyTex_, x, y, "Password protected.");
		}

		std::string officialStr = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(row, "type");
		if (officialStr == "official")
		{
			drawIcon(tankTex_, x, y, "An offical server.");
		}
		else if (officialStr == "mod")
		{
			drawIcon(cogTex_, x, y, "Home of mod server.");
		}
	}
	else if (col == 2)
	{
		std::string clients = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(row, "noplayers");
		std::string maxclients = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(row, "maxplayers");
		std::string compplayers = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(row, "compplayers");

		std::string name;
		if (compplayers.c_str()[0])
		{
			name = formatString("%s/%s (%i)", clients.c_str(), maxclients.c_str(),
				(atoi(clients.c_str()) - atoi(compplayers.c_str())));
		}
		else
		{
			name = formatString("%s/%s (?)", clients.c_str(), maxclients.c_str());
		}

		std::string message = name;
		int noplayers = atoi(clients.c_str());
		for (int i=0; i<noplayers; i++)
		{
			std::string pn = 
				ServerBrowser::instance()->getServerList().getEntryValue(
					row, formatString("pn%i", i));
			std::string pa = 
				ServerBrowser::instance()->getServerList().getEntryValue(
					row, formatString("pa%i", i));

			message.append(formatString("\n%i: %s: %s", 
				i, pa.c_str(), pn.c_str()));
		}

		value = formatString("%s", name.c_str());
		tipValue = formatString("%s", message.c_str());
	}
	else
	{
		tipValue = value = ServerBrowser::instance()->getServerList().getEntryValue(row, cols[col].dataName);
		if (value[0] == '\0' && col == 1)
		{
			tipValue = value = ServerBrowser::instance()->getServerList().getEntryValue(row, "address");
		}
	}

	if (value)
	{
		Vector color(0.3f, 0.3f, 0.3f);
		GLWFont::instance()->getLargePtFont()->drawWidth(w, 
			color, 
			10.0f, x + 3.0f, y + 5.0f, 0.0f, value);

		if (tipValue)
		{
			if (GLWToolTip::instance()->addToolTip(&colToolTip_, 
				GLWTranslate::getPosX() + x, 
				GLWTranslate::getPosY() + y, 
				w, 20.0f))
			{
				colToolTip_.setText(cols[col].col.name.c_str(), tipValue);
			}
		}
	}
}

bool NetworkSelectDialog::serverCompatable(std::string pversion, std::string version)
{
	if (pversion.size() > 0 && 
		0 != strcmp(pversion.c_str(), ScorchedProtocolVersion))
	{
		return false;
	}

	return true;
}

void NetworkSelectDialog::rowSelected(unsigned int id, int row)
{
	if (row < 0 || row >= ServerBrowser::instance()->getServerList().getNoEntries())
	{
		return;
	}

	std::string ipaddress = ServerBrowser::instance()->
		getServerList().getEntryValue(row, "address");
	if (ipaddress.empty()) return;

	std::string protocolVersion = 
		ServerBrowser::instance()->getServerList().getEntryValue(row, "protocolversion");
	std::string version =
		ServerBrowser::instance()->getServerList().getEntryValue(row, "version");
	if (!serverCompatable(protocolVersion, version))
	{
		MsgBoxDialog::instance()->show(
			formatString(
			"Warning: This server is running a incompatable version of Scorched3D.\n"
			"You cannot connect to this server.\n\n"
			"This server is running Scorched build %s (%s).\n"
			"You are running Scorched build %s (%s).\n\n"					
			"The latest version of Scorched3D can be downloaded from \n"
			"http://www.scorched3d.co.uk\n",
			version.c_str(), protocolVersion.c_str(),
			ScorchedVersion, ScorchedProtocolVersion));
	}

	ipaddress_->setText(ipaddress.c_str());
	password_->setText("");
}

void NetworkSelectDialog::rowChosen(unsigned int id, int row)
{
	if (ok_->getEnabled()) buttonDown(ok_->getId());
}

void NetworkSelectDialog::columnSelected(unsigned int id, int col)
{
	ServerBrowser::instance()->getServerList().sortEntries(cols[col].dataName);
}

void NetworkSelectDialog::display()
{
	ipaddress_->setText("");
	refreshType_->setCurrentText("Internet");
}

void NetworkSelectDialog::hide()
{
	stopRefresh();
}

void NetworkSelectDialog::updateTable()
{
	// Check if we are refreshing
	// Set the button accordingly
	if (!ServerBrowser::instance()->getRefreshing())
	{
		refresh_->setText("Refresh");
	}
	else
	{
		refresh_->setText("Stop");
	}

	// Check if we have more items to display
	if (invalidateId_ != ServerBrowser::instance()->
		getServerList().getRefreshId())
	{
		invalidateId_ = ServerBrowser::instance()->
			getServerList().getRefreshId();
		iconTable_->setItemCount(
			ServerBrowser::instance()->getServerList().getNoEntries());
	}
}

void NetworkSelectDialog::select(unsigned int id, const int pos, GLWSelectorEntry value)
{
	if (0 == strcmp(refreshType_->getCurrentText(), "Favourites"))
	{
		favourites_->setText("Del Favourite");
	}
	else
	{
		favourites_->setText("Add Favourite");
	}

	startRefresh();
}

void NetworkSelectDialog::startRefresh()
{
	stopRefresh();

	iconTable_->setItemCount(0);

	ServerBrowser::RefreshType t = ServerBrowser::RefreshNone;
	if (0 == strcmp(refreshType_->getCurrentText(), "LAN")) t = ServerBrowser::RefreshLan;
	else if (0 == strcmp(refreshType_->getCurrentText(), "Internet")) t = ServerBrowser::RefreshNet;
	else if (0 == strcmp(refreshType_->getCurrentText(), "Favourites")) t = ServerBrowser::RefreshFavourites;

	ServerBrowser::instance()->refreshList(t);
	updateTable();
}

void NetworkSelectDialog::stopRefresh()
{
	ServerBrowser::instance()->cancel();
	updateTable();
}

void NetworkSelectDialog::textChanged(unsigned int id, const char *text)
{
	ok_->setEnabled(text[0]!='\0');
	favourites_->setEnabled(text[0]!='\0');
}

void NetworkSelectDialog::buttonDown(unsigned int id)
{
	if (id == refresh_->getId())
	{
		if (ServerBrowser::instance()->getRefreshing())
		{
			stopRefresh();
		}
		else
		{
			startRefresh();
		}
	}
	else if (id == ok_->getId())
	{
		GLWWindowManager::instance()->hideWindow(id_);

		if (ipaddress_->getText()[0])
		{
			ClientParams::instance()->reset();
			ClientParams::instance()->setConnect(ipaddress_->getText().c_str());
			ClientParams::instance()->setPassword(password_->getText().c_str());
			ClientMain::startClient();
		}
	}
	else if (id == favourites_->getId())
	{
		if (!ipaddress_->getText().empty())
		{
			std::set<std::string> favs = 
				ServerBrowser::instance()->getCollect().getFavourites();

			if (0 == strcmp(refreshType_->getCurrentText(), "Favourites"))
			{
				favs.erase(ipaddress_->getText().c_str());
			}
			else 
			{
				favs.insert(ipaddress_->getText().c_str());
			}
			ServerBrowser::instance()->getCollect().setFavourites(favs);

			if (0 == strcmp(refreshType_->getCurrentText(), "Favourites"))
			{
				startRefresh();
			}
		}
	}
	else if (id == cancelId_)
	{
		GLWWindowManager::instance()->hideWindow(id_);
	}
}
