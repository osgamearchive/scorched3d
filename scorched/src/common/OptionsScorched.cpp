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

#include <common/OptionsScorched.h>
#include <common/Logger.h>

OptionsScorched::OptionsScorched()
{
}

OptionsScorched::~OptionsScorched()
{
}

void OptionsScorched::updateChangeSet()
{
	NetBufferDefault::defaultBuffer.reset();
	mainOptions_.writeToBuffer(NetBufferDefault::defaultBuffer, true, true);
	NetBufferReader reader(NetBufferDefault::defaultBuffer);
	changedOptions_.readFromBuffer(reader, true, true);
}

bool OptionsScorched::commitChanges()
{
	bool different = false;

	// Compare buffers
	std::list<OptionEntry *> &options = mainOptions_.getOptions();
	std::list<OptionEntry *> &otheroptions = changedOptions_.getOptions();
	std::list<OptionEntry *>::iterator itor;
	std::list<OptionEntry *>::iterator otheritor;
	for (itor=options.begin(), otheritor=otheroptions.begin();
		itor!=options.end() && otheritor!=otheroptions.end();
		itor++, otheritor++)
	{
		OptionEntry *entry = *itor;
		OptionEntry *otherentry = *otheritor;

		DIALOG_ASSERT(0 == strcmp(entry->getName(), otherentry->getName()));

		std::string str = entry->getValueAsString();
		std::string otherstr = otherentry->getValueAsString();
		if (str != otherstr)
		{
			if (!(entry->getData() & OptionEntry::DataProtected) &&
				!(otherentry->getData() & OptionEntry::DataProtected))
			{
				if (strlen(str.c_str()) < 20 && strlen(otherstr.c_str()) < 20)
				{
					Logger::log(formatString("Option %s has been changed from %s to %s",
						entry->getName(), str.c_str(), otherstr.c_str()));
				}
				else
				{
					Logger::log(formatString("Option %s has been changed.",
						entry->getName()));
				}
			}

			different = true;
			entry->setValueFromString(otherentry->getValueAsString());
		}
	}

	return different;
}
