#include <XML/XMLFile.h>
#include <common/FileLines.h>

XMLFile::XMLFile()
{
}

XMLFile::~XMLFile()
{
}

bool XMLFile::readFile(const char *fileName)
{
	FileLines lines;
	// Load the file
	// Failing to find the file is not an error
	if (!lines.readFile((char *) fileName)) return true;

	// Parse the file
	std::vector<std::string>::iterator itor;
	for (itor = lines.getLines().begin();
		itor != lines.getLines().end();
		itor++)
	{
		if (!parser_.parse((*itor).c_str(), (*itor).size(), 0)) return false;

		// Hack to get it to count lines correctly :)
		if (!parser_.parse("\n", 1, 0)) return false;
	}
	if (!parser_.parse(0, 0, 1)) return false;

	return true;
}
