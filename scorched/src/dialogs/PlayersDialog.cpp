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

#include <GLEXT/GLState.h>
#include <GLW/GLWLabel.h>
#include <GLW/GLWTextButton.h>
#include <tank/TankContainer.h>
#include <tank/TankColorGenerator.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIHuman.h>
#include <tankai/TankAIAdder.h>
#include <tankgraph/TankModelStore.h>
#include <common/OptionsGame.h>
#include <common/WindowManager.h>
#include <common/OptionsTransient.h>
#include <engine/GameState.h>
#include <client/ClientNewGameHandler.h>
#include <client/ClientState.h>
#include <dialogs/PlayersDialog.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlayersDialog* PlayersDialog::instance_ = 0;

PlayersDialog* PlayersDialog::instance()
{
	if (!instance_)
	{
		instance_ = new PlayersDialog;
	}

	return instance_;
}

PlayersDialog::PlayersDialog() : 
	GLWWindow("Players", 10.0f, 10.0f, 460.0f, 280.0f, 0), 
	currentPlayer_(0)
{
	// Add all of the player names that should be used for the tanks
	playerNames_.readFile( PKGDIR "data/playernames.txt");
	DIALOG_ASSERT(playerNames_.getLines().size());

	needCentered_ = true;
	viewer_ = new GLWTankViewer(20.0f, 25.0f, 4, 3);

	// Add buttons
	addWidget(viewer_);
	okId_ = addWidget(new GLWTextButton(" Ok", 380, 10, 55, this, true))->getId();

	// Create player name choice
	playerName_ = (GLWTextBox *) addWidget(new GLWTextBox(80, 240, 240, ""));

	// Create player name choice
	Vector newColor;
	playerFlag_ = (GLWFlag *) addWidget(new GLWFlag(newColor, 10, 245, 60));

	// Create computer type choice
	dropDown_ = (GLWDropDown *) addWidget(new GLWDropDown(330, 240, 110));

	dropDown_->addText("Human");
	std::list<TankAIComputer *>::iterator aiitor;
	for (aiitor = TankAIStore::instance()->getAis().begin();
		aiitor != TankAIStore::instance()->getAis().end();
		aiitor++)
	{
		dropDown_->addText((*aiitor)->getName());
	}
}

PlayersDialog::~PlayersDialog()
{

}

void PlayersDialog::windowInit(const unsigned state)
{
	// Remove any tanks that may be there
	TankContainer::instance()->removeAllTanks();
	TankAIAdder::addTankAIs();

	// Reset stuff to original state
	currentPlayer_ = 0;

	// Load this player
	nextPlayer();
}

void PlayersDialog::nextPlayer()
{
	playerName_->getText() = playerNames_.getLines()
		[currentPlayer_ % playerNames_.getLines().size()].c_str();
	Vector newColor = TankColorGenerator::instance()->getNextColor();
	playerFlag_->setColor(newColor);
}

void PlayersDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		if (!playerName_->getText().empty() &&
			viewer_->getModelName())
		{
			TankModel *tankModel = 
				TankModelStore::instance()->getModelByName(viewer_->getModelName());

			// Create the tank and add it the the containers
			TankModelId modelId(tankModel->getId().getModelName());
			static unsigned int idAllocator = 1;
			Tank *aTank = new Tank(
				idAllocator++,
				playerName_->getText().c_str(), 
				playerFlag_->getColor(), 
				modelId);
			TankContainer::instance()->addTank(aTank);

			// Create the tank ai and add it to the tank
			const char *aiName = dropDown_->getText();
			TankAI *ai = 0;
			if (stricmp(aiName, "Human") == 0)
			{
				ai = new TankAIHuman(aTank);
			}
			else
			{
				TankAIComputer *copyAi = 
					TankAIStore::instance()->getAIByName(aiName);
				ai = copyAi->getCopy(aTank);
			}
			aTank->setTankAI(ai);

			// Check if we have finished creating all of the tanks
			if (++currentPlayer_ < OptionsGame::instance()->getNoMaxPlayers())
			{
				nextPlayer();
			}
			else
			{
				// Ensure we have enough players for a game
				DIALOG_ASSERT(TankContainer::instance()->getNoOfTanks() > 1);

				WindowManager::instance()->hideWindow(getId());
				OptionsTransient::instance()->reset();
				// Yes create the level and move into the next state
				ClientNewGameHandler::instance()->clientNewGame();
			}
		}
	}
}
