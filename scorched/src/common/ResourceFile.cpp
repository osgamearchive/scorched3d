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


// ResourceFile.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include <common/ResourceFile.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>
#include <wx/utils.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ResourceFile::ResourceFile() : currentModule_(0)
{

}

ResourceFile::~ResourceFile()
{
}

void ResourceFile::setRandomModule()
{
	int moduleId = (int) (RAND * modules_.size());
	std::map<std::string, ResourceModule *>::iterator itor;
	for (itor = modules_.begin();
		itor != modules_.end();
		itor++)
	{
		if (moduleId-- == 0) 
		{
			currentModule_ = (*itor).second;
			return;
		}
	}
	currentModule_ = (*modules_.begin()).second;
}

bool ResourceFile::setModule(const char *name)
{
	std::map<std::string, ResourceModule *>::iterator findItor =
		modules_.find(name);
	if (findItor == modules_.end()) return false;

	currentModule_ = (*findItor).second;
	return true;
}

const char *ResourceFile::getResource(ResourceModule *module, const char *name)
{
	if (module)
	{
		std::map<std::string, std::string>::iterator itor = 
			module->moduleValues.find(name);
		if (itor != module->moduleValues.end()) 
		{
			return (*itor).second.c_str();
		}
		else
		{
			if (module->inherits)
			{
				return getResource(module->inherits, name);
			}
			else
			{
				dialogMessage("Resource File", 
					"Cannot find resource named \"%s\", in module \"%s\".",
					name, module->moduleName.c_str());
			}
		}
	}
	else
	{
		dialogMessage("Resource File", 
			"Cannot find resource named \"%s\", no current module.",
			name);
	}

	exit(1);
	return 0;	
}

const char *ResourceFile::getStringResource(const char *name)
{
	return getResource(currentModule_, name);
}

bool ResourceFile::initFromFile(const char *xmlFileName)
{
	XMLFile xmlFile;
	if (!xmlFile.readFile(xmlFileName))
	{
		dialogMessage("Resource File", 
			"Cannot open resource file \"%s\"",
			xmlFileName);
		return false;
	}

	XMLNode *rootNode = xmlFile.getRootNode();
	if (!rootNode)
	{
		dialogMessage("Resource File", 
			"Cannot find root node in resource file \"%s\"",
			xmlFileName);
		return false;
	}

	// For each module in the resource file
	std::list<XMLNode *> &moduleNodes = rootNode->getChildren();
	std::list<XMLNode *>::iterator itor;
	for (itor = moduleNodes.begin();
		itor != moduleNodes.end();
		itor++)
	{
		// Get the current module
		XMLNode *moduleNode = (*itor);

		// Check if this resource is enabled
		bool resouceEnabled = true;
		XMLNode *enabled = moduleNode->getNamedParameter("enabled");
		if (enabled)
		{
			if (strcmp(enabled->getContent(), "true"))
			{
				resouceEnabled = false;
			}
		}

		if (resouceEnabled)
		{
			ResourceModule *inheritedNode = 0;
			XMLNode *inherits = moduleNode->getNamedParameter("inherits");
			if (inherits)
			{
				std::map<std::string, ResourceModule *>::iterator findItor =
					modules_.find(inherits->getContent());
				if (findItor == modules_.end())
				{
					dialogMessage("Resource File", 
						"Resource file \"%s\" module \"%s\" inherits unknown module \"%s\"",
						xmlFileName, moduleNode->getName(), inherits->getContent());
					return false;
				}
				else
				{
					inheritedNode = (*findItor).second;
				}
			}

			ResourceModule *module = 
				new ResourceModule(moduleNode->getName(), inheritedNode);
			if (!currentModule_) currentModule_ = module;

			// Add new module type and check it does not already exist
			std::map<std::string, ResourceModule *>::iterator findItor =
				modules_.find(moduleNode->getName());
			if (findItor != modules_.end())
			{
				dialogMessage("Resource File", 
					"Resource file \"%s\" has duplicate module \"%s\"",
					xmlFileName, moduleNode->getName());
				return false;
			}
			modules_[moduleNode->getName()] = module;

			// Add all the values from the module
			std::list<XMLNode *> &valueNodes = moduleNode->getChildren();
			std::list<XMLNode *>::iterator valueItor;
			for (valueItor = valueNodes.begin();
				valueItor != valueNodes.end();
				valueItor++)
			{
				// Get the current value
				XMLNode *valueNode = (*valueItor);

				// Check the value does not already exist and add the value
				std::map<std::string, std::string>::iterator valueFind =
					module->moduleValues.find(valueNode->getName());
				if (valueFind != module->moduleValues.end())
				{
					dialogMessage("Resource File", 
						"Resource file \"%s\" has duplicate value \"%s:%s\"",
						xmlFileName, moduleNode->getName(), valueNode->getName());
					return false;
				}

				// check files exist etc...
				XMLNode *type = valueNode->getNamedParameter("type");
				if (type)
				{
					if (0 == strcmp(type->getContent(), "file"))
					{
						static char string[1024];
						sprintf(string, "%s", valueNode->getContent());

						// Remove front spaces etc...
						char *spaceString = string;
						while (*spaceString && *spaceString < 32) spaceString++;

						// Remove end spaces etc...
						for (int i=strlen(spaceString) - 1; i>=0; i--)
						{
							if (spaceString[i] < 32) spaceString[i] = '\0';
							else break;
						}

						// Add directory component
						static char fileName[1024];
						sprintf(fileName, "%s%s", PKGDIR, spaceString);
	                    
						::wxDos2UnixFilename(fileName);
						if (::wxFileExists(fileName))
						{
							module->moduleValues[valueNode->getName()] = fileName;
						}
						else
						{
							dialogMessage("Resource File", 
								"Resource file \"%s\" value \"%s:%s\"\n"
								"Cannot find file type defines filename \"%s\"",
								xmlFileName, moduleNode->getName(), valueNode->getName(),
								fileName);
							return false;
						}
					}
					else
					{
						dialogMessage("Resource File", 
							"Resource file \"%s\" has unknown type \"%s\" value \"%s:%s\"",
							xmlFileName, type->getContent(), moduleNode->getName(), valueNode->getName());
						return false;
					}
				}
				else
				{
					module->moduleValues[valueNode->getName()] =
						valueNode->getContent();
				}
			}
		}
	}

	return true;
}
