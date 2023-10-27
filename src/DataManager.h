#pragma once
#include <fstream>
#include <filesystem>
#include <random>

using namespace geode::prelude;
namespace fs = std::filesystem;

class DataManager {
public:

	static DataManager* getInstance();

	void setUsername(std::string username);
	void setPassword(std::string password);
	std::string getUsername();
	std::string getPassword();

	void loadLocalAccountData();
	void loadDefaultLocalOptions();
	void parseLocalAccountFile(std::string content);
	void saveLocalAccountFile();
	
	void loadLocalLevelsFromFiles();
	void loadBackupFromServer(json::Array levelsFromServer);
	json::Array packAllLocalLevels2Json();
	void deleteLevelFolder();


	GJGameLevel* createLocalLevel();

	void updateLocalLevel(GJGameLevel* lvl);
	void updateLocalLevel(json::Value);

	void deleteLocalLevel(int localId);
	
	json::Value getLocalLevelById(int localId);
	

	DataManager();

    std::vector<json::Value> localLevels;
	static bool isMapOpened;
private:
	int getUniqLocalLevelId();

	std::string username;
	std::string password;

    

	static DataManager* instance;
};