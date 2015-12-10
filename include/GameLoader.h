#ifndef GAMELOADER_H
#define GAMELOADER_H

#include "rapidjson\document.h"
#include <memory>
#include <fstream>
#include "SFML/Graphics.hpp" 
#include "GameData.h"
#include <dirent.h>

using namespace rapidjson;

class GameLoader {
public:
	GameLoader(std::string const & filePath, std::string const & animationFilePath);
private:
	rapidjson::Document m_document;
	std::string m_JSONData;
	void loadJSONDATA(std::string const & filename);
	std::string wStringToString(std::wstring const & wstr);
	int loadFileNames();
	void loadAnimations();
	void loadData();
	const std::string m_filePath;
	const std::string m_animationFilePath;
	std::vector<std::string> m_jsonAnimFileNames;
};

#endif 