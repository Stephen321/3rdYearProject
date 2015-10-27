#include "GameLoader.h"
#include <iostream> //test
GameLoader::GameLoader(std::string const & filePAth) : m_filePath(filePAth){
}

void GameLoader::loadJSONDATA(std::string const & filename){
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()){
		while (std::getline(file, line)){
			m_JSONData.append(line);
		}
	}
}

void GameLoader::loadAnimations(std::string const & JSONFileName) {
	//if (m_document.Size() > 0)
	//	m_document.Clear();
	m_JSONData.clear();
	loadJSONDATA(m_filePath + JSONFileName);
	m_document.Parse<0>(m_JSONData.c_str());

	std::shared_ptr<GameData> ptr = GameData::getInstance();
	
	//Value const & v = m_document["name"];
	Value::ConstMemberIterator it = m_document.MemberBegin(); //iterator for entire object
	std::cout << it->value.GetString() << std::endl;
	std::string characterName = it->value.GetString();
	it++;
	std::vector<std::shared_ptr<sf::Texture>>* textures;
	std::unordered_map<std::string, Animation>* animations;

	if (characterName == "player"){
		textures = &ptr->playerTextures;
		animations = &ptr->playerAnims;
	}
	else{
		return;
		//textures = 0;
		//animations = 0;
	}

	for (Value::ConstValueIterator itA = it->value.Begin(); itA < it->value.End(); itA++){//iterator for animations array loop for each animation object
		Value::ConstMemberIterator itAO = itA->MemberBegin();//iterator for object in animations array
		Animation a;
		std::string name = itAO->value.GetString();
		itAO++;//now has name of spritesheet
		std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>(sf::Texture());
		tex->loadFromFile(m_filePath + itAO->value.GetString());
		itAO++;//now has frames array
		sf::IntRect r;
		for (Value::ConstValueIterator itFrames = itAO->value.Begin(); itFrames < itAO->value.End(); itFrames++){//iterator for frames loop for each frame object
			Value const & frameObject = (*itFrames)["frame"]; //get the frame object from this object in frames. the frame object contains pos and size info
			r.left = frameObject["x"].GetInt();
			r.top = frameObject["y"].GetInt();
			r.width = frameObject["w"].GetInt();
			r.height = frameObject["h"].GetInt();
			a.addFrame(r);
		}
		textures->push_back(tex);
		a.setSpriteSheet(*textures->back());
		(*animations)[name] = a;
	}
}