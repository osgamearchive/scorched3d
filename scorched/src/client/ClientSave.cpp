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

#include <client/ClientSave.h>
#include <server/ScorchedServer.h>
#include <tankai/TankAIComputer.h>
#include <common/Defines.h>
#include <XML/XMLParser.h>
#include <time.h>

void saveClient()
{
	// Root Node
	XMLNode rootnode("scorchedsave");
	time_t currentTime = time(0);
	rootnode.addParameter(new XMLNode("version", ScorchedVersion, 
		XMLNode::XMLParameterType));
	rootnode.addChild(new XMLNode("date", ctime(&currentTime)));

	// GameState
	XMLNode *gameStateNode = new XMLNode("gamestate");
	ScorchedServer::instance()->getOptionsGame().writeOptionsToXML(
		gameStateNode);
	rootnode.addChild(gameStateNode);
	
	// Transient State
	XMLNode *transientStateNode = new XMLNode("transientstate");
	ScorchedServer::instance()->getOptionsTransient().writeToXML(
		transientStateNode);
	rootnode.addChild(transientStateNode);
	
	// Players
	XMLNode *playersNode = new XMLNode("players");
	rootnode.addChild(playersNode);

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
 	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		XMLNode *playerNode = new XMLNode("player");
		playersNode->addChild(playerNode);

		// Add each tank
		Tank *tank = (*itor).second;
		if (!tank->writeXML(playerNode)) return;
	}

	// Write XML
	//rootnode.writeToFile("/tmp/a.xml");	
}

