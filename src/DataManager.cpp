#include "./DataManager.h"
#include "./MyLevelTools.h"

using namespace geode::prelude;


DataManager* DataManager::getInstance() {
	if (!instance) {
		instance = new DataManager();
		instance->loadLocalLevelsFromFiles();
	}
	return instance;
};

void DataManager::setUsername(std::string username) {
	this->username = username;
	//Mod::get()->setSavedValue<std::string>("username", username);
}
void DataManager::setPassword(std::string password) {
	this->password = password;
	//Mod::get()->setSavedValue<std::string>("password", password);
}
std::string DataManager::getUsername() {
	return username;
}
std::string DataManager::getPassword() {
	return password;
}


void DataManager::loadLocalLevelsFromFiles() {
	if (!fs::exists("./Resources/levels/") || !fs::is_directory("./Resources/levels/"))
		fs::create_directory("./Resources/levels/");
	for (const auto& entry : fs::directory_iterator("./Resources/levels")) {
		if (entry.is_regular_file() && entry.path().extension() == ".gdlevel") {
			std::ifstream file(entry.path().string());
			if (file.is_open()) {
				std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();

				localLevels.push_back(json::parse(content));
			}
		}
	}
}
void DataManager::loadBackupFromServer(json::Array levelsFromServer) {
	for (json::Value lvl : levelsFromServer)
		updateLocalLevel(lvl);
}
json::Array DataManager::packAllLocalLevels2Json() {
	auto json = json::Array();
	for (json::Value lvl : localLevels)
		json.push_back(lvl);
	return json;
}

void DataManager::loadLocalAccountData() {
	auto path = std::string("./Resources/localAccountData.json");
	if (fs::exists(path)) {
		std::ifstream file(path);
		if (file.is_open()) {
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
			parseLocalAccountFile(content);
		}
		else loadDefaultLocalOptions();
	}
	else loadDefaultLocalOptions();
}

void DataManager::deleteLevelFolder() {
	fs::path filePath = std::string("./Resources/levels");
	if (fs::exists(filePath))
	fs::remove_all(filePath);
	localLevels.clear();
}


void DataManager::loadDefaultLocalOptions() {
	GameManager::sharedState()->setPlayerFrame(1);
	GameManager::sharedState()->setPlayerShip(1);
	GameManager::sharedState()->setPlayerBall(1);
	GameManager::sharedState()->setPlayerBird(1);
	GameManager::sharedState()->setPlayerDart(1);
	GameManager::sharedState()->setPlayerRobot(1);
	GameManager::sharedState()->setPlayerSpider(1);
	GameManager::sharedState()->setPlayerStreak(1);
	GameManager::sharedState()->setPlayerDeathEffect(1);
	GameManager::sharedState()->setPlayerColor(1);
	GameManager::sharedState()->setPlayerColor2(1);
	GameManager::sharedState()->setPlayerGlow(0);
	username = "";
	password = "";
}
void DataManager::parseLocalAccountFile(std::string content) {
	auto json = json::parse(content);

	GameManager::sharedState()->setPlayerFrame(json["cube"].as_int());
	GameManager::sharedState()->setPlayerShip(json["ship"].as_int());
	GameManager::sharedState()->setPlayerBall(json["ball"].as_int());
	GameManager::sharedState()->setPlayerBird(json["ufo"].as_int());
	GameManager::sharedState()->setPlayerDart(json["wave"].as_int());
	GameManager::sharedState()->setPlayerRobot(json["robot"].as_int());
	GameManager::sharedState()->setPlayerSpider(json["spider"].as_int());
	GameManager::sharedState()->setPlayerStreak(json["trail"].as_int());
	GameManager::sharedState()->setPlayerDeathEffect(json["deathEffect"].as_int());
	GameManager::sharedState()->setPlayerColor(json["color"].as_int());
	GameManager::sharedState()->setPlayerColor2(json["color2"].as_int());
	GameManager::sharedState()->setPlayerGlow(json["glow"].as_bool());
	username = json["username"].as_string();
	password = json["password"].as_string();
}

void DataManager::saveLocalAccountFile() {
	auto json = json::Value();

	json.set("cube", json::Value(GameManager::sharedState()->getPlayerFrame()));
	json.set("ship", json::Value(GameManager::sharedState()->getPlayerShip()));
	json.set("ball", json::Value(GameManager::sharedState()->getPlayerBall()));
	json.set("ufo", json::Value(GameManager::sharedState()->getPlayerBird()));
	json.set("wave", json::Value(GameManager::sharedState()->getPlayerDart()));
	json.set("robot", json::Value(GameManager::sharedState()->getPlayerRobot()));
	json.set("spider", json::Value(GameManager::sharedState()->getPlayerSpider()));
	json.set("trail", json::Value(GameManager::sharedState()->getPlayerStreak()));
	json.set("deathEffect", json::Value(GameManager::sharedState()->getPlayerDeathEffect()));
	json.set("color", json::Value(GameManager::sharedState()->getPlayerColor()));
	json.set("color2", json::Value(GameManager::sharedState()->getPlayerColor2()));
	json.set("glow", json::Value(GameManager::sharedState()->getPlayerGlow()));
	json.set("username", json::Value(DataManager::getInstance()->getUsername()));
	json.set("password", json::Value(DataManager::getInstance()->getPassword()));

	fs::path filePath = std::string("./Resources/localAccountData.json");
	std::ofstream file(filePath.string());
	if (file.is_open()) {
		file << json.dump(json::NO_INDENTATION);
		file.close();
	}
	else {
		log::error("Can't save account data");
	}
}


GJGameLevel* DataManager::createLocalLevel() {
	auto lvl = GJGameLevel::create();
	int randId = getUniqLocalLevelId();
	lvl->m_levelID = randId;
	lvl->m_levelName = "level " + std::to_string(randId);
	lvl->m_levelType = GJLevelType::Editor;
	lvl->m_levelLength = 1;
	lvl->m_levelString = "";
	localLevels.push_back(MyLevelTools::level2Json(lvl));
	MyLevelTools::saveLocalLevelAsFile(MyLevelTools::level2Json(lvl));
	return lvl;
}

void DataManager::updateLocalLevel(GJGameLevel* lvl) {
	int index = 0;
	bool find = 0;
	for (json::Value jsonLvl : localLevels) {
		if (jsonLvl["id"].as_int() == lvl->m_levelID) {
			localLevels[index] = MyLevelTools::level2Json(lvl);
			find = 1;
		}
		index++;
	}
	if (!find)
		localLevels.push_back(MyLevelTools::level2Json(lvl)); //SUS
	MyLevelTools::saveLocalLevelAsFile(MyLevelTools::level2Json(lvl));
}
void DataManager::updateLocalLevel(json::Value lvl) {
	int index = 0;
	bool find = 0;
	for (json::Value jsonLvl : localLevels) {
		if (jsonLvl["id"].as_int() == lvl["id"].as_int()) {
			localLevels[index] = lvl;
			find = 1;
		}
		index++;
	}
	if (!find)
		localLevels.push_back(lvl);
	MyLevelTools::saveLocalLevelAsFile(lvl);
}

void DataManager::deleteLocalLevel(int localId) {
	int index = 0;
	int find = 0;
	for (json::Value lvl : localLevels) {
		if (lvl["id"].as_int() == localId) {
			localLevels.erase(localLevels.begin() + index);
			find = 1;
		}
		index++;
	}
	if (!find) {
		log::error("Can't find local level with id {}", localId);
		return;
	}
	MyLevelTools::deleteLocalLevelAsFile(localId);
}

json::Value DataManager::getLocalLevelById(int localId) {
	for (json::Value lvl : localLevels) {
		if (lvl["id"].as_int() == localId)
			return lvl;
	}
}


int DataManager::getUniqLocalLevelId() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 10000);

	int randomNum;
	bool isUnique;

	do {
		isUnique = true;
		randomNum = distribution(gen);

		for (json::Value item : localLevels) {
			if (item["id"] == randomNum) {
				isUnique = false;
				break;
			}
		}
	} while (!isUnique);
	return randomNum;
}

DataManager::DataManager()
	: username("")
	, password("")
{
	//username = Mod::get()->getSavedValue<std::string>("username");
	//password = Mod::get()->getSavedValue<std::string>("password");
	loadLocalAccountData();
}


DataManager* DataManager::instance = nullptr;
bool DataManager::isMapOpened = 0;