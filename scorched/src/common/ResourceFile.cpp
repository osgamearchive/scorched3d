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

#include <common/ResourceFile.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>
#include <wx/utils.h>

ResourceFile::ResourceFile() : currentModule_(0)
{

}

ResourceFile::~ResourceFile()
{
}

bool ResourceFile::setModule(const char *name)
{
	std::map<std::string, ResourceModule *>::iterator findItor =
		modules_.find(name);
	if (findItor == modules_.end()) return false;

	currentModule_ = (*findItor).second;
	return true;
}

void *ResourceFile::getResource(ResourceModule *module, const char *name)
{
	if (module)
	{
		std::map<std::string, void *>::iterator itor = 
			module->moduleValues.find(name);
		if (itor != module->moduleValues.end()) 
		{
			return (*itor).second;
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

Vector *ResourceFile::getVectorResource(const char *name)
{
	return (Vector *) getResource(currentModule_, name);
}

const char *ResourceFile::getStringResource(const char *name)
{
	std::string *str = (std::string *) getResource(currentModule_, name);
	if (str) return str->c_str();
	return 0;
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
			std::map<std::string, void *>::iterator valueFind =
				module->moduleValues.find(valueNode->getName());
			if (valueFind != module->moduleValues.end())
			{
				dialogMessage("Resource File", 
					"Resource file \"%s\" has duplicate value \"%s:%s\"",
					xmlFileName, moduleNode->getName(), valueNode->getName());
				return false;
			}

			// Parse the node with respect the specified node type
			XMLNode *type = valueNode->getNamedParameter("type");
			void *result = parseType((type?type->getContent():0), 
				xmlFileName, moduleNode, valueNode);
			if (!result) return false;

			module->moduleValues[valueNode->getName()] = result;
		}
	}

	return true;
}

void *ResourceFile::parseType(const char *type, 
							  const char *xmlFileName,
							  XMLNode *moduleNode,
							  XMLNode *valueNode)
{
	if (type)
	{
		if (0 == strcmp(type, "color"))
		{
			XMLNode *rcolorNode = valueNode->getNamedChild("r");
			XMLNode *gcolorNode = valueNode->getNamedChild("g");
			XMLNode *bcolorNode = valueNode->getNamedChild("b");
			if (!rcolorNode || !gcolorNode || !bcolorNode)
			{
				dialogMessage("Resource File", 
					"Resource file \"%s\" value \"%s:%s\"\n"
					"Cannot find all required color nodes",
					xmlFileName, moduleNode->getName(), valueNode->getName());
			}
			else
			{
				Vector *color = new Vector();
				(*color)[0] = (float) atof(rcolorNode->getContent());
				(*color)[1] = (float) atof(gcolorNode->getContent());
				(*color)[2] = (float) atof(bcolorNode->getContent());
				return color;
			}
		}
		else 
		if (0 == strcmp(type, "file"))
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
				return new std::string(fileName);
			}
			else
			{
				dialogMessage("Resource File", 
					"Resource file \"%s\" value \"%s:%s\"\n"
					"Cannot find file type defines filename \"%s\"",
					xmlFileName, moduleNode->getName(), valueNode->getName(),
					fileName);
			}
		}
		else
		{
			dialogMessage("Resource File", 
				"Resource file \"%s\" has unknown type \"%s\" value \"%s:%s\"",
				xmlFileName, type, moduleNode->getName(), valueNode->getName());
		}
	}
	else
	{
		return new std::string(valueNode->getContent());
	}
	return 0;
}
