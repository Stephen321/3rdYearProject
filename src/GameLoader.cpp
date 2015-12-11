#include "GameLoader.h"
#include <iostream> //testing
GameLoader::GameLoader(std::string const & filePath) :
m_filePath(filePath){
	loadData();
}

std::vector<std::string> GameLoader::loadJSONFileNames(const std::string & animationFilePath){
	DIR *dir;
	struct dirent *ent;
	std::vector<std::string> jsonAnimFileNames;
	if ((dir = opendir((m_filePath + animationFilePath).c_str())) != NULL) {
		/* load all files ending in .json into a vector */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_namlen > 5 &&
				ent->d_name[ent->d_namlen - 5] == '.' &&
				ent->d_name[ent->d_namlen - 4] == 'j' &&
				ent->d_name[ent->d_namlen - 3] == 's' &&
				ent->d_name[ent->d_namlen - 2] == 'o' &&
				ent->d_name[ent->d_namlen - 1] == 'n'){
				jsonAnimFileNames.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
	}
	return jsonAnimFileNames;
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

void GameLoader::loadData(){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_JSONData.clear();
	loadJSONDATA(m_filePath + "data.json"); 
	m_document.Parse<0>(m_JSONData.c_str());	

	Value::ConstMemberIterator it = m_document.MemberBegin();
	std::string animationsPath = it->value.GetString();
	++it;
	std::string soundsPath = it->value.GetString();
	++it;
	std::string imagesPath = it->value.GetString();
	++it;
	std::string mapsPath = it->value.GetString();
	++it;

	//maps
	ptr->mapLoaderPath = m_filePath + mapsPath;

	//animations
	loadAnimations(animationsPath, loadJSONFileNames(animationsPath)); 
	
	//sounds
	Value::ConstMemberIterator soundsIT = it->value.MemberBegin();
	Value::ConstMemberIterator soundsITEnd = it->value.MemberEnd();
	int test = 1;
	for (; soundsIT != soundsITEnd; ++soundsIT){
		SoundManager::getInstance()->loadSound(m_filePath + soundsPath, soundsIT->value.GetString());
		std::cout << "sound" << test++ << "loaded" << std::endl;
	}
	++it;

	//textures
	Value::ConstMemberIterator texturesIT = it->value.MemberBegin();
	ptr->rockTexture.loadFromFile(m_filePath + imagesPath + texturesIT->value.GetString());
}

void GameLoader::loadAnimations(const std::string & animationFilePath, const std::vector<std::string> & jsonAnimFileNames) {
	//if (m_document.Size() > 0)
	//	m_document.Clear();
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	std::vector<std::shared_ptr<sf::Texture>>* textures;
	std::unordered_map<std::string, Animation>* animations;
	float* playSpeed;
	float* scale;
	std::string characterName;

	int jsonFileCount = jsonAnimFileNames.size();
	for (int i = 0; i < jsonFileCount; i++)
	{
		m_JSONData.clear();
		loadJSONDATA(m_filePath + animationFilePath + jsonAnimFileNames[i]);
		m_document.Parse<0>(m_JSONData.c_str());

		Value::ConstMemberIterator it = m_document.MemberBegin(); //iterator for entire object
		characterName = it->value.GetString();
		if (characterName == "player"){
			textures = &ptr->playerTextures;
			animations = &ptr->playerAnims;
			playSpeed = &ptr->playerPlaySpeed;
			scale = &ptr->playerSpriteScale;
		}
		else if (characterName == "ai"){
			textures = &ptr->aiTextures;
			animations = &ptr->aiAnims;
			playSpeed = &ptr->aiPlaySpeed;
			scale = &ptr->aiSpriteScale;
		}
		else{
			return;
		}
		it++;//now has play speed
		*playSpeed = 1.f / it->value.GetInt();
		it++; //now has scale
		*scale = it->value.GetDouble();
		it++;//now has the animations array

		for (Value::ConstValueIterator itA = it->value.Begin(); itA < it->value.End(); itA++){//iterator for animations array loop for each animation object
			Value::ConstMemberIterator itAO = itA->MemberBegin();//iterator for object in animations array
			Animation a;
			std::string name = itAO->value.GetString();
			itAO++;//now has name of spritesheet
			std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>(sf::Texture());
			tex->loadFromFile(m_filePath + animationFilePath + itAO->value.GetString());
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
}