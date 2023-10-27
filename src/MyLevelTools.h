#pragma once
#include <fstream>
#include <filesystem>
#include <random>

using namespace geode::prelude;

class MyLevelTools {
	friend class DataManager;
	static void saveLocalLevelAsFile(json::Value level);
	static void deleteLocalLevelAsFile(int localId);
	
public:
	static MyLevelTools* getInstance();

	static json::Value level2Json(GJGameLevel* lvl);
	static GJGameLevel* json2Level(json::Value json);

	MyLevelTools();
private:
	static MyLevelTools* instance;
};