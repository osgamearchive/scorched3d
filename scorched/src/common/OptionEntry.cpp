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

#include <common/OptionEntry.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <XML/XMLFile.h>
#include <stdio.h>
#include <map>

OptionEntry::OptionEntry(std::list<OptionEntry *> &group,
						 const char *name,
						 const char *description,
						 unsigned int data) :
	name_(name),
	description_(description),
	data_(data)
{
	group.push_back(this);
}

OptionEntry::~OptionEntry()
{

}

bool OptionEntryHelper::addToArgParser(std::list<OptionEntry *> &options,
									   ARGParser &parser)
{
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		if (!(*itor)->addToArgParser(parser)) return false;
	}

	return true;
}

bool OptionEntryHelper::writeToBuffer(std::list<OptionEntry *> &options,
									  NetBuffer &buffer,
									  bool useprotected)
{
	// Write out all of the options
	// We do this as strings this make the message larger than it needs to be but
	// we always know how to read the options at the other end.
	// Even if the other end does not have all the options this end does.
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = *itor;
		if (!(entry->getData() & OptionEntry::DataProtected) || useprotected)
		{
			buffer.addToBuffer(entry->getName());
			buffer.addToBuffer(entry->getValueAsString());
		}
	}
	buffer.addToBuffer("-"); // Marks the end of the entries

	return true;
}

bool OptionEntryHelper::readFromBuffer(std::list<OptionEntry *> &options,
									   NetBufferReader &reader,
									   bool useprotected)
{
	// Create a map from string name to existing options
	// So we can find the named option when reading the buffer
	std::map<std::string, OptionEntry *> entryMap;
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		if (!(entry->getData() & OptionEntry::DataProtected) || useprotected)
		{
			std::string name = entry->getName();
			entryMap[name] = entry;
		}
	}

	// Read the strings from the other end
	for (;;)
	{
		std::string name, value;
		if (!reader.getFromBuffer(name)) return false;
		if (strcmp(name.c_str(), "-") == 0) break; // The end symbol
		if (!reader.getFromBuffer(value)) return false;
		
		std::map<std::string, OptionEntry *>::iterator finditor =
			entryMap.find(name);
		if (finditor == entryMap.end())
		{
			Logger::log( "Warning:Does not support server option \"%s\"",
				name.c_str());
		}
		else
		{
			OptionEntry *entry = (*finditor).second;
			if (!entry->setValueFromString(value.c_str())) return false;
		}
	}

	return true;
}

bool OptionEntryHelper::writeToXML(std::list<OptionEntry *> &options,
	XMLNode *node)
{
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = (*itor);

		if (!(entry->getData() & OptionEntry::DataDepricated))
		{
			// Add the comments for this node
			node->addChild(new XMLNode("", 
				entry->getDescription(), XMLNode::XMLCommentType));
			std::string defaultValue = "(default value : \"";
			defaultValue += entry->getDefaultValueAsString();
			defaultValue += "\")";
			node->addChild(new XMLNode("", 
				defaultValue.c_str(), XMLNode::XMLCommentType));
			
			// Add the name and value
			XMLNode *optionNode = new XMLNode("option");
			node->addChild(optionNode);
			optionNode->addChild(
				new XMLNode("name", entry->getName()));
			optionNode->addChild(
				new XMLNode("value", entry->getValueAsString()));
		}
	}
	return true;
}

bool OptionEntryHelper::writeToFile(std::list<OptionEntry *> &options,
									char *filePath)
{
	XMLNode optionsNode("options");
	optionsNode.addParameter(
		new XMLNode("source", "Scorched3D", XMLNode::XMLParameterType));
	writeToXML(options, &optionsNode);

	if (!optionsNode.writeToFile(filePath)) return false;

	return true;
}

bool OptionEntryHelper::readFromXML(std::list<OptionEntry *> &options,
									 XMLNode *rootnode)
{
	// Create a map from string name to existing options
	// So we can find the named option when parsing the file
	std::map<std::string, OptionEntry *> entryMap;
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		std::string name = entry->getName();
		_strlwr((char *) name.c_str());

		entryMap[name] = entry;
	}

	// Itterate all of the options in the file
	std::list<XMLNode *>::iterator childrenItor;
	for (childrenItor = rootnode->getChildren().begin();
		childrenItor != rootnode->getChildren().end();
		childrenItor++)
	{
		XMLNode *currentNode = (*childrenItor);
		XMLNode *nameNode, *valueNode; 
		if (!currentNode->getNamedChild("name", nameNode)) return false;
		if (!currentNode->getNamedChild("value", valueNode)) return false;

		std::string name = nameNode->getContent();
		std::string value = valueNode->getContent();
		_strlwr((char *) name.c_str());

		std::map<std::string, OptionEntry *>::iterator finditor =
			entryMap.find(name);
		if (finditor == entryMap.end())
		{
			dialogMessage("Scorched3D Options",
				"ERROR: Failed to parse file \"%s\". Cannot find option \"%s\"",
				currentNode->getSource(), name.c_str());
			return false;					
		}

		if (!(*finditor).second->setValueFromString(value.c_str()))
		{
			dialogMessage("Scorched3D Options",
				"ERROR: Failed to parse file \"%s\"\n"
				"Failed to set option \"%s\" with \"%s\"",
				currentNode->getSource(), name.c_str(), value.c_str());
			return false;		
		}	
	}
	return true;
}								 

bool OptionEntryHelper::readFromFile(std::list<OptionEntry *> &options,
									 char *filePath)
{
	// Parse the XML file
	XMLFile file;
	if (!file.readFile(filePath))
	{
		dialogMessage("Scorched3D Options", 
			"ERROR: Failed to parse file \"%s\"\n"
			"%s",
			filePath,
			file.getParserError());		
		return false;
	}

	// return true for an empty file
	if (!file.getRootNode()) return true;

	// Read the options from the XML node
	if (!readFromXML(options, file.getRootNode())) return false;

	return true;
}

OptionEntryInt::OptionEntryInt(std::list<OptionEntry *> &group,
							   const char *name,
							   const char *description,
							   unsigned int data,
							   int value) :
	OptionEntry(group, name, description, data), 
	value_(value), defaultValue_(value)
{
	
}

OptionEntryInt::~OptionEntryInt()
{

}

const char *OptionEntryInt::getValueAsString()
{
	static char value[256];
	sprintf(value, "%i", value_);
	return value;
}

const char *OptionEntryInt::getDefaultValueAsString()
{
	static char value[256];
	sprintf(value, "%i", defaultValue_);
	return value;
}

bool OptionEntryInt::setValueFromString(const char *string)
{
	int val;
	if (sscanf(string, "%i", &val) != 1) return false;
	return setValue(val);
}

int OptionEntryInt::getValue() 
{
	return value_;
}

bool OptionEntryInt::setValue(int value)
{
	value_ = value;
	return true;
}

bool OptionEntryInt::setIntArgument(int value)
{
	return setValue(value);
}

bool OptionEntryInt::addToArgParser(ARGParser &parser)
{
	char name[256], description[1024];
	sprintf(name, "-%s", getName());
	sprintf(description, "%s (Default : %s)",
			(char *) getDescription(), 
			(char *) getDefaultValueAsString());

	parser.addEntry(name , this, description);
	return true;
}

OptionEntryBoundedInt::OptionEntryBoundedInt(std::list<OptionEntry *> &group,
											 const char *name,
											 const char *description,
											 unsigned int data,
											 int value,
											 int minValue, int maxValue) :
	OptionEntryInt(group, name, description, data, value), 
	minValue_(minValue), maxValue_(maxValue)
{
	
}

OptionEntryBoundedInt::~OptionEntryBoundedInt()
{
	
}

bool OptionEntryBoundedInt::setValue(int value)
{
	if (value < minValue_ || value > maxValue_) return false;
	return OptionEntryInt::setValue(value);
}

OptionEntryBool::OptionEntryBool(std::list<OptionEntry *> &group,
								 const char *name, 
								 const char *description, 
								 unsigned int data,
								 bool value) :
	OptionEntry(group, name, description, data), 
	value_(value), defaultValue_(value)
{
	
}

OptionEntryBool::~OptionEntryBool()
{
	
}

const char *OptionEntryBool::getDefaultValueAsString()
{
	return (defaultValue_?"on":"off");
}

const char *OptionEntryBool::getValueAsString()
{
	return (value_?"on":"off");
}

bool OptionEntryBool::setValueFromString(const char *string)
{
	if (stricmp("on", string) == 0)
	{
		value_ = true;
		return true;
	}
	if (stricmp("off", string) == 0)
	{
		value_ = false;
		return true;
	}
	return false;
}

bool OptionEntryBool::setValue(bool value)
{
	value_ = (value?1:0);
	return true;
}

bool OptionEntryBool::getValue()
{
	return ((value_==0)?false:true);
}

bool OptionEntryBool::setBoolArgument(bool value)
{
	return setValue(value);
}

bool OptionEntryBool::addToArgParser(ARGParser &parser)
{
	char name[256], description[1024];
	sprintf(name, "-%s", getName());
	sprintf(description, "%s (Default : %s)",
			(char *) getDescription(), 
			(char *) getDefaultValueAsString());

	parser.addEntry(name , this, description);
	return true;
}

OptionEntryString::OptionEntryString(std::list<OptionEntry *> &group,
									 const char *name, 
									 const char *description, 
									 unsigned int data,
									 const char *value) :
	OptionEntry(group, name, description, data),
	value_(value), defaultValue_(value)
{
	
}

OptionEntryString::~OptionEntryString()
{

}

const char *OptionEntryString::getValueAsString()
{
	return value_.c_str();
}

const char *OptionEntryString::getDefaultValueAsString()
{
	return defaultValue_.c_str();
}

bool OptionEntryString::setValueFromString(const char *string)
{
	return setValue(string);
}

const char *OptionEntryString::getValue() 
{
	return value_.c_str();
}

bool OptionEntryString::setValue(const char *value)
{
	value_ = value;
	return true;
}

bool OptionEntryString::setStringArgument(const char* value)
{
	return setValue(value);
}

bool OptionEntryString::addToArgParser(ARGParser &parser)
{
	char name[256], description[1024];
	sprintf(name, "-%s", getName());
	sprintf(description, "%s (Default : %s)",
			(char *) getDescription(), 
			(char *) getDefaultValueAsString());

	parser.addEntry(name , this, description);
	return true;
}

OptionEntryFloat::OptionEntryFloat(std::list<OptionEntry *> &group,
							   const char *name,
							   const char *description,
							   unsigned int data,
							   float value) :
	OptionEntry(group, name, description, data), 
	value_(value), defaultValue_(value)
{
	
}

OptionEntryFloat::~OptionEntryFloat()
{

}

const char *OptionEntryFloat::getValueAsString()
{
	static char value[256];
	sprintf(value, "%.2f", value_);
	return value;
}

const char *OptionEntryFloat::getDefaultValueAsString()
{
	static char value[256];
	sprintf(value, "%.2f", defaultValue_);
	return value;
}

bool OptionEntryFloat::setValueFromString(const char *string)
{
	float val;
	if (sscanf(string, "%f", &val) != 1) return false;
	return setValue(val);
}

float OptionEntryFloat::getValue() 
{
	return value_;
}

bool OptionEntryFloat::setValue(float value)
{
	value_ = value;
	return true;
}

bool OptionEntryFloat::addToArgParser(ARGParser &parser)
{
	DIALOG_ASSERT(0);
	return false;
}

OptionEntryVector::OptionEntryVector(std::list<OptionEntry *> &group,
							   const char *name,
							   const char *description,
							   unsigned int data,
							   Vector value) :
	OptionEntry(group, name, description, data), 
	value_(value), defaultValue_(value)
{
	
}

OptionEntryVector::~OptionEntryVector()
{

}

const char *OptionEntryVector::getValueAsString()
{
	static char value[256];
	sprintf(value, "%.2f %.2f %.2f", 
		value_[0], value_[1], value_[2]);
	return value;
}

const char *OptionEntryVector::getDefaultValueAsString()
{
	static char value[256];
	sprintf(value, "%.2f %.2f %.2f", 
		defaultValue_[0], defaultValue_[1], defaultValue_[2]);
	return value;
}

bool OptionEntryVector::setValueFromString(const char *string)
{
	Vector vec;
	if (sscanf(string, "%f %f %f", 
		&vec[0], &vec[1], &vec[2]) != 3) return false;
	return setValue(vec);
}

Vector &OptionEntryVector::getValue() 
{
	return value_;
}

bool OptionEntryVector::setValue(Vector value)
{
	value_ = value;
	return true;
}

bool OptionEntryVector::addToArgParser(ARGParser &parser)
{
	DIALOG_ASSERT(0);
	return false;
}
