#include "./MyLevelTools.h"

using namespace geode::prelude;
namespace fs = std::filesystem;

MyLevelTools* MyLevelTools::getInstance() {
	if (!instance) {
		instance = new MyLevelTools();
	}
	return instance;
};

json::Value MyLevelTools::level2Json(GJGameLevel* lvl) {
	auto json = json::Value();
	json.set("id", json::Value(lvl->m_levelID.value()));
	json.set("levelName", json::Value(lvl->m_levelName.c_str()));
	json.set("levelString", json::Value(lvl->m_levelString.c_str()));
	json.set("length", json::Value(lvl->m_levelLength));
	if (lvl->m_songID) {
		json.set("isLocalSong", json::Value(0));
		json.set("songId", json::Value(lvl->m_songID));
	}
	else {
		json.set("isLocalSong", json::Value(1));
		json.set("songId", json::Value(lvl->m_audioTrack));
	}
	switch (lvl->m_levelType) {
	case GJLevelType::Local:
		json.set("type", json::Value(1));
		break;
	case GJLevelType::Editor:
		json.set("type", json::Value(2));
		break;
	case GJLevelType::Saved:
		json.set("type", json::Value(3));
		break;
	default:
		json.set("type", json::Value(1));
		break;
	}
	return json;
}

GJGameLevel* MyLevelTools::json2Level(json::Value json) {
	auto lvl = GJGameLevel::create();
	lvl->m_levelID = json["id"].as_int();
	lvl->m_levelName = json["levelName"].as_string();
	lvl->m_levelString = json["levelString"].as_string();
	lvl->m_levelLength = json["length"].as_int();
	if (json["isLocalSong"].as_int())
		lvl->m_audioTrack = json["songId"].as_int();
	else
		lvl->m_songID = json["songId"].as_int();
	switch (json["type"].as_int()) {
	case 1:
		lvl->m_levelType = GJLevelType::Local;
		break;
	case 2:
		lvl->m_levelType = GJLevelType::Editor;
		break;
	case 3:
		lvl->m_levelType = GJLevelType::Saved;
		break;
	default:
		lvl->m_levelType = GJLevelType::Local;
		break;
	}
	return lvl;
}

void MyLevelTools::saveLocalLevelAsFile(json::Value level) {
	fs::path filePath = fs::path(std::string("./Resources/levels/") + std::to_string(level["id"].as_int()) + std::string(".gdlevel"));
	if (!fs::exists("./Resources/levels/") || !fs::is_directory("./Resources/levels/"))
		fs::create_directory("./Resources/levels/");

	std::ofstream file(filePath.string());
	if (file.is_open()) {
		file << level.dump(json::NO_INDENTATION);
		file.close();
	}
	else {
		log::error("Can't save level with id {}", level["id"].as_int());
	}
}

void MyLevelTools::deleteLocalLevelAsFile(int localId) {
	fs::path filePath = fs::path(std::string("./Resources/levels/") + std::to_string(localId) + std::string(".gdlevel"));
	if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
		if (!fs::remove(filePath))
			log::error("Can't delete the level with id {}", localId);
	}
	else
		log::error("No local level with id {}", localId);
}

MyLevelTools::MyLevelTools() {}
MyLevelTools* MyLevelTools::instance = nullptr;