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

#include <server/ServerWebSettingsHandler.h>
#include <server/ServerLog.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <server/ServerState.h>
#include <server/ServerParams.h>
#include <landscapedef/LandscapeDefinitionsBase.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <engine/ModDirs.h>
#include <engine/ModFiles.h>
#include <tankai/TankAINames.h>
#include <XML/XMLParser.h>
#include <vector>
#include <algorithm>

static const char *getField(std::map<std::string, std::string> &fields, const char *field)
{
	std::map<std::string, std::string>::iterator itor = 
		fields.find(field);
	if (itor != fields.end())
	{
		return (*itor).second.c_str();
	}
	return 0;
}

static void setValues(std::map<std::string, std::string> &fields)
{
	std::list<OptionEntry *>::iterator itor;
	{
		std::list<OptionEntry *> &options = 
			ScorchedServer::instance()->getOptionsGame().
				getChangedOptions().getOptions();
		for (itor = options.begin();
			itor != options.end();
			itor++)
		{
			OptionEntry *entry = (*itor);
			std::map<std::string, std::string>::iterator findItor =
				fields.find(entry->getName());
			if (findItor != fields.end())
			{
				const char *value = (*findItor).second.c_str();
				entry->setValueFromString(value);
			}
		}	
	}
	{
		std::list<OptionEntry *> &options = 
			ScorchedServer::instance()->getOptionsGame().
				getChangedOptions().getPlayerTypeOptions();
		for (itor = options.begin();
			itor != options.end();
			itor++)
		{
			OptionEntry *entry = (*itor);
			std::map<std::string, std::string>::iterator findItor =
				fields.find(entry->getName());
			if (findItor != fields.end())
			{
				const char *value = (*findItor).second.c_str();
				entry->setValueFromString(value);
			}
		}	
	}
}

void ServerWebSettingsHandler::generateSettingValue(OptionEntry *entry, std::string &value)
{
	if (entry->getEntryType() == OptionEntry::OptionEntryTextType)
	{
		value = formatString("<textarea name='%s' cols=20 rows=5>%s</textarea>",
			entry->getName(),
			entry->getValueAsString());
	}
	else if (entry->getEntryType() == OptionEntry::OptionEntryBoolType)
	{
		OptionEntryBool *boolEntry = (OptionEntryBool *) entry;
		
		value = formatString(
			"<input type='radio' name='%s' %s value='on'>On</input>"
			"<input type='radio' name='%s' %s value='off'>Off</input>",
			entry->getName(), (boolEntry->getValue()?"checked":""),
			entry->getName(), (!boolEntry->getValue()?"checked":""));
	}
	else if (entry->getEntryType() == OptionEntry::OptionEntryBoundedIntType)
	{
		OptionEntryBoundedInt *intEntry = (OptionEntryBoundedInt *) entry;
		
		value = formatString("<select name='%s'>", entry->getName());
		bool found = false;
		for (int i=intEntry->getMinValue(); 
			i<=intEntry->getMaxValue();
			i+=intEntry->getStepValue())
		{
			if (intEntry->getValue() < i && !found)
			{
				found = true;
				value.append(formatString("<option %s>%i</option>",
					"selected", 
					intEntry->getValue()));
			}
			else if (intEntry->getValue() == i)
			{
				found = true;
			}

			value.append(formatString("<option %s>%i</option>",
				(intEntry->getValue() == i?"selected":""), 
				i));
		}
		value.append("</select>");
	}
	else if (entry->getEntryType() == OptionEntry::OptionEntryEnumType)
	{
		OptionEntryEnum *enumEntry = (OptionEntryEnum *) entry;

		value = formatString("<select name='%s'>", entry->getName());
		OptionEntryEnum::EnumEntry *enums = enumEntry->getEnums();
		for (OptionEntryEnum::EnumEntry *current = enums; current->description[0]; current++)
		{
			value.append(formatString("<option %s>%s</option>",
				(enumEntry->getValue() == current->value?"selected":""), 
				current->description));		
		}
		value.append("</select>");
	}
	else if (entry->getEntryType() == OptionEntry::OptionEntryStringEnumType)
	{
		OptionEntryStringEnum *enumEntry = (OptionEntryStringEnum *) entry;
		
		value = formatString("<select name='%s'>", entry->getName());
		OptionEntryStringEnum::EnumEntry *enums = enumEntry->getEnums();
		for (OptionEntryStringEnum::EnumEntry *current = enums; current->value[0]; current++)
		{
			value.append(formatString("<option %s>%s</option>",
				(0 == strcmp(enumEntry->getValue(), current->value)?"selected":""), 
				current->value));		
		}
		value.append("</select>");
	}
	else
	{
		value = formatString("<input type='text' name='%s' value='%s'>",
			entry->getName(),
			entry->getValueAsString());
	}
}

bool ServerWebSettingsHandler::SettingsPlayersHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) ServerParams::instance()->getServerFile());
	}
	else
	{
		setValues(fields);
	}

	{
		TankAINames tankAIStore;
		tankAIStore.loadAIs();
		
		std::string players;
		std::list<OptionEntry *>::iterator itor;
		std::list<OptionEntry *> &options = 
			ScorchedServer::instance()->getOptionsGame().
				getChangedOptions().getPlayerTypeOptions();
		for (itor = options.begin();
			itor != options.end();
			itor++)
		{
			OptionEntry *entry = (*itor);
			
			std::string value = "";

			value.append("<tr><td>");
			value.append(entry->getName());
			value.append("</td><td>");
			value.append(formatString("<select name='%s'>", entry->getName()));
			std::list<std::string> &ais = tankAIStore.getAis();
			std::list<std::string>::iterator aiitor;
			for (aiitor = ais.begin(); aiitor != ais.end(); aiitor++)
			{
				std::string &ai = (*aiitor);
				value.append(formatString("<option %s>%s</option>",
					(0 == strcmp(entry->getValueAsString(), ai.c_str())?"selected":""), 
					ai.c_str()));		
			}
			value.append("</select>");
			value.append("</td></tr>\n");
			
			players.append(value);
		}
		
		fields["PLAYERS"] = players;
	}	

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) ServerParams::instance()->getServerFile());
	}

	return ServerWebServer::getHtmlTemplate("settingsplayers.html", fields, text);
}

bool ServerWebSettingsHandler::SettingsLandscapeHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) ServerParams::instance()->getServerFile());
	}

	LandscapeDefinitionsBase landscapeDefinitions;
	landscapeDefinitions.readLandscapeDefinitions();
	std::list<LandscapeDefinitionsEntry> &defns =
		landscapeDefinitions.getAllLandscapes();
	std::list<LandscapeDefinitionsEntry>::iterator itor;

	OptionsGame &optionsGame = 
		ScorchedServer::instance()->getOptionsGame().getChangedOptions();

	// Set new options if any 
	if (action && (
		0 == strcmp(action, "Save") || 
		0 == strcmp(action, "Set") || 
		0 == strcmp(action, "Select All") || 
		0 == strcmp(action, "Select None")
		))
	{
		std::string landscapesString = "";
		if (0 != strcmp(action, "Select None"))
		{
			for (itor = defns.begin();
				itor != defns.end();
				itor++)
			{
				LandscapeDefinitionsEntry &dfn = *itor;
		
				const char *setting = getField(fields, dfn.name.c_str());
				if (0 == strcmp(action, "Select All") || 
					(setting && 0 == strcmp(setting, "on")))
				{
					if (!landscapesString.empty()) landscapesString.append(":");
					landscapesString.append(dfn.name.c_str());
				}
			}
		}
		else
		{
			landscapesString = " ";
		}
		optionsGame.getLandscapesEntry().setValue(landscapesString.c_str());
	}

	// Read the current options
	std::string landscapes;
	for (itor = defns.begin();
		itor != defns.end();
		itor++)
	{
		LandscapeDefinitionsEntry &dfn = *itor;

		bool enabled = landscapeDefinitions.landscapeEnabled(
			optionsGame, dfn.name.c_str());

		std::string value = formatString(
			"<input type='radio' name='%s' %s value='on'>On</input>"
			"<input type='radio' name='%s' %s value='off'>Off</input>",
			dfn.name.c_str(), (enabled?"checked":""),
			dfn.name.c_str(), (!enabled?"checked":""));

		landscapes.append(formatString("<tr><td>%s</td><td>%s</td><td>%s</td></tr>\n",
			dfn.name.c_str(), 
			(dfn.description.c_str()[0]?dfn.description.c_str():"-"), 
			value.c_str()));
	}
	fields["LANDSCAPES"] = landscapes;

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) ServerParams::instance()->getServerFile());
	}

	return ServerWebServer::getHtmlTemplate("settingslandscape.html", fields, text);
}

bool ServerWebSettingsHandler::SettingsAllHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::list<OptionEntry *>::iterator itor;
	std::list<OptionEntry *> &options = 
		ScorchedServer::instance()->getOptionsGame().
			getChangedOptions().getOptions();

	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) ServerParams::instance()->getServerFile());
	}
	else
	{
		// Check if any changes have been made
		setValues(fields);
	}

	// Show the current settings
	std::string settings;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		if (!(entry->getData() & OptionEntry::DataDepricated))
		{
			std::string value;
			generateSettingValue(entry, value);
			
			bool different = (0 != strcmp(entry->getValueAsString(), 
				entry->getDefaultValueAsString()));
			std::string desc = entry->getDescription();
			desc.append("<br>Default value : ");
			if (different) desc.append("<b>");
			desc.append(entry->getDefaultValueAsString());
			if (different) desc.append("</b>");

			settings += formatString("<tr><td>%s</td>"
				"<td><font size=-1>%s</font></td>"
				"<td>%s</td></tr>\n",
				entry->getName(),
				desc.c_str(),
				value.c_str());
		}
	}

	fields["SETTINGS"] = settings;

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) ServerParams::instance()->getServerFile());
	}

	return ServerWebServer::getHtmlTemplate("settingsall.html", fields, text);
}

bool ServerWebSettingsHandler::SettingsMainHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::list<OptionEntry *>::iterator itor;
	std::list<OptionEntry *> &options = 
		ScorchedServer::instance()->getOptionsGame().
			getChangedOptions().getOptions();

	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) ServerParams::instance()->getServerFile());
	}
	else
	{
		// Check if any changes have been made
		setValues(fields);
	}

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) ServerParams::instance()->getServerFile());
	}

	return ServerWebServer::getHtmlTemplate("settingsmain.html", fields, text);
}

bool ServerWebSettingsHandler::SettingsModHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::list<OptionEntry *>::iterator itor;
	std::list<OptionEntry *> &options = 
		ScorchedServer::instance()->getOptionsGame().
			getChangedOptions().getOptions();

	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) ServerParams::instance()->getServerFile());
	}
	else
	{
		// Check if any changes have been made
		setValues(fields);
	}

	// Import/upload a mod (if specified)
	if (!parts.empty())
	{
		std::map<std::string, NetMessage *>::iterator modupload =
			parts.find("modupload");
		if (modupload != parts.end())
		{
			NetMessage *message = (*modupload).second;
			ModFiles files;
			const char *mod = 0;
			if (files.importModFiles(&mod, message->getBuffer()))
			{
				if (files.writeModFiles(mod))
				{
					return ServerWebServer::getHtmlMessage(
						"Mod Upload", 
						formatString("Successfuly uploaded and imported mod %s",
						(mod?mod:"Unknown")), fields, text);
				}
				else
				{
					return ServerWebServer::getHtmlMessage(
						"Mod Upload", 
						"Failed to write mod files to disk", fields, text);
				}
			}
			else
			{
				return ServerWebServer::getHtmlMessage(
					"Mod Upload", 
					"Failed to load mod files from network", fields, text);
			}
		}
	}

	// Display the list of mods, and the displayed the currently selected mod
	ModDirs modDirs;
	if (modDirs.loadModDirs())
	{
		OptionEntryString &modOption = 
			ScorchedServer::instance()->getOptionsGame().getChangedOptions().getModEntry();

		std::string mods;
		std::list<ModInfo>::iterator itor;
		for (itor = modDirs.getDirs().begin();
			itor != modDirs.getDirs().end();
			itor++)
		{
			ModInfo &modInfo = (*itor);

			mods.append(formatString("<tr><td>%s</td><td>%s</td><td><input type='radio' name='%s' %s value='%s'></input></td>\n",
				modInfo.getName(),
				modInfo.getDescription(),
				modOption.getName(),
				((0 == strcmp(modOption.getValueAsString(), modInfo.getName()))?"checked":""),
				modInfo.getName()));
		}

		fields["MODS"] = mods;
	}

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) ServerParams::instance()->getServerFile());
	}

	return ServerWebServer::getHtmlTemplate("settingsmod.html", fields, text);
}
