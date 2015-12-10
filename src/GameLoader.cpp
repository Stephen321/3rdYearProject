#include "GameLoader.h"
#include <iostream> //testing
GameLoader::GameLoader(std::string const & filePath, FMOD::System * soundSystem) :
m_filePath(filePath){
	loadData(soundSystem);
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

void GameLoader::loadData(FMOD::System * soundSystem){
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_JSONData.clear();
	loadJSONDATA(m_filePath + "data.json"); 
	m_document.Parse<0>(m_JSONData.c_str());	

	Value::ConstMemberIterator it = m_document.MemberBegin();
	std::string animationPath = it->value.GetString();
	++it;
	std::string soundPath = it->value.GetString();
	loadAnimations(animationPath, loadJSONFileNames(animationPath)); //animations
	++it;
	//sounds
	Value::ConstMemberIterator soundsIT = it->value.MemberBegin();
	std::cout << 
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->birdTweet1);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->birdTweet2);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->birdTweet3);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->birdTweet4);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->birdTweet5);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->crowSound);
	++soundsIT;
	soundSystem->createSound((m_filePath + soundPath + soundsIT->value.GetString()).c_str(), FMOD_3D, 0, &ptr->windAmbience);
	++it;
	//textures
	Value::ConstMemberIterator texturesIT = it->value.MemberBegin();
	ptr->rockTexture.loadFromFile(m_filePath + texturesIT->value.GetString());
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