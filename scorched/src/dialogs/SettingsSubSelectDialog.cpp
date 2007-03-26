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

#include <dialogs/SettingsSubSelectDialog.h>
#include <dialogs/SettingsSelectDialog.h>
#include <dialogs/MsgBoxDialog.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWOptionEntry.h>
#include <GLW/GLWTabContainer.h>
#include <GLW/GLWSpacer.h>
#include <client/ClientParams.h>
#include <client/ClientMain.h>

SettingsSubSelectDialog *SettingsSubSelectDialog::instance_ = 0;

SettingsSubSelectDialog *SettingsSubSelectDialog::instance()
{
	if (!instance_)
	{
		instance_ = new SettingsSubSelectDialog;
	}
	return instance_;
}

SettingsSubSelectDialog::SettingsSubSelectDialog() : 
	GLWWindow("", 700.0f, 540.0f, 0, "")
{
	// Create Tabs
	GLWTabContainer *tabPanel = new GLWTabContainer(0.0f, 0.0f, 0.0f, 0.0f);
	mainTab_ = (GLWTab *)
		tabPanel->addWidget(new GLWTab("Main", 10, 40, 520, 315));
	mainTab_->setGridWidth(2);
	mainTab_->setLayout(GLWPanel::LayoutGrid);
	ecoTab_ = (GLWTab *)
		tabPanel->addWidget(new GLWTab("Eco", 10, 40, 520, 315));
	ecoTab_->setGridWidth(2);
	ecoTab_->setLayout(GLWPanel::LayoutGrid);
	scoreTab_ = (GLWTab *)
		tabPanel->addWidget(new GLWTab("Score", 10, 40, 520, 315));
	scoreTab_->setGridWidth(2);
	scoreTab_->setLayout(GLWPanel::LayoutGrid);
	envTab_ = (GLWTab *)
		tabPanel->addWidget(new GLWTab("Env", 10, 40, 520, 315));
	envTab_->setGridWidth(2);
	envTab_->setLayout(GLWPanel::LayoutGrid);
	landTab_ = (GLWTab *)
		tabPanel->addWidget(new GLWTab("Land", 10, 40, 520, 315));
	landTab_->setGridWidth(2);
	landTab_->setLayout(GLWPanel::LayoutGrid);
	addWidget(tabPanel, 0, SpaceAll, 10.0f);

	OptionsGame &optionsGame = SettingsSelectDialog::instance()->getOptions();

	// Add Options
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getTeamBallanceEntry());
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getNoRoundsEntry());
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getNoMaxRoundTurnsEntry());
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getStartTimeEntry());	
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getShotTimeEntry());	
	GLWOptionEntry::createEntry(
		controls_, mainTab_, optionsGame.getBuyingTimeEntry());	
	mainTab_->addWidget(new GLWSpacer(0.0f, 20.0f));

	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getBuyOnRoundEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getStartMoneyEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getInterestEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyPerRoundEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getEconomyEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonPerAssistPointEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonPerKillPointEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonPerMultiKillPointEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonPerHitPointEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonForRoundEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyWonForLivesEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getMoneyPerHealthPointEntry());
	GLWOptionEntry::createEntry(
		controls_, ecoTab_, optionsGame.getGiveAllWeaponsEntry());
	ecoTab_->addWidget(new GLWSpacer(0.0f, 10.0f));

	GLWOptionEntry::createEntry(
		controls_, scoreTab_, optionsGame.getScoreWonForRoundEntry());
	GLWOptionEntry::createEntry(
		controls_, scoreTab_, optionsGame.getScoreWonForLivesEntry());
	GLWOptionEntry::createEntry(
		controls_, scoreTab_, optionsGame.getScorePerKillEntry());
	GLWOptionEntry::createEntry(
		controls_, scoreTab_, optionsGame.getScorePerAssistEntry());
	GLWOptionEntry::createEntry(
		controls_, scoreTab_, optionsGame.getScorePerMoneyEntry());
	scoreTab_->addWidget(new GLWSpacer(0.0f, 20.0f));

	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getWindForceEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getWindTypeEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getWallTypeEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getWeapScaleEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getStartArmsLevelEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getEndArmsLevelEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getMinFallingDistanceEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getMaxClimbingDistanceEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getResignModeEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getMovementRestrictionEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getPlayerLivesEntry());
	GLWOptionEntry::createEntry(
		controls_, envTab_, optionsGame.getDelayedDefenseActivationEntry());
	envTab_->addWidget(new GLWSpacer(0.0f, 10.0f));
	// End Add Options

	// Create buttons
	GLWPanel *buttonPanel = new GLWPanel(0.0f, 0.0f, 0.0f, 0.0f, false, false);
	GLWButton *cancelButton = new GLWTextButton("Cancel", 0.0f, 0.0f, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX);
	cancelId_ = cancelButton->getId();
	buttonPanel->addWidget(cancelButton, 0, SpaceRight, 10.0f);
	GLWButton *okButton = new GLWTextButton("Ok", 0.0f, 0.0f, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX);
	okId_ = okButton->getId();
	buttonPanel->addWidget(okButton);
	buttonPanel->setLayout(GLWPanel::LayoutHorizontal);
	addWidget(buttonPanel, 0, AlignRight | SpaceLeft | SpaceRight | SpaceBottom, 10.0f);

	setLayout(GLWPanel::LayoutVerticle);
	layout();
}

SettingsSubSelectDialog::~SettingsSubSelectDialog()
{
}

void SettingsSubSelectDialog::display()
{
	GLWOptionEntry::updateControls(controls_);
	mainTab_->setDepressed();
}

void SettingsSubSelectDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		GLWOptionEntry::updateEntries(controls_);

		GLWWindowManager::instance()->hideWindow(id_);
	}
	else if (id == cancelId_)
	{
		GLWWindowManager::instance()->hideWindow(id_);
	}
}
