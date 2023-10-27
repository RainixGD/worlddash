#include "LoadingAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool LoadingAlert::init() {
	if (!FLAlertLayer::init(nullptr, "Loading!", "", "cancel", nullptr, 200, 0, 170)) {
		return false;
	}

	m_buttonMenu->removeAllChildren();

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto loadingCircle = MyLoadingCircle::create();
	loadingCircle->setPosition(size / 2);
	loadingCircle->setZOrder(5);
	m_mainLayer->addChild(loadingCircle);

	auto message = json::Value();
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "load", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
	);

	return true;
}

void LoadingAlert::onResponse(std::string response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		log::info("{}", json["message"].as_string());
		DataManager::getInstance()->loadBackupFromServer(json::parse(json["message"].as_string()).as_array());
		FLAlertLayer::create("GG!", "Data Loaded!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();
}
void LoadingAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void LoadingAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

LoadingAlert* LoadingAlert::create() {
	auto ret = new LoadingAlert();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
