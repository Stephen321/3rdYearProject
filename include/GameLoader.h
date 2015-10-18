#ifndef GAMELOADER_H
#define GAMELOADER_H

#include "rapidjson\document.h"
#include <memory>
#include <fstream>
#include "SFML/Graphics.hpp" 
#include "GameData.h"

using namespace rapidjson;

class GameLoader {
public:
	GameLoader(std::string const & filePAth);
	void loadAnimations(std::string const & JSONFileName);

private:
	rapidjson::Document m_document;
	std::string m_JSONData;
	void loadJSONDATA(std::string const & filename);
	std::string wStringToString(std::wstring const & wstr);
	std::string m_filePath;
};

#endif 