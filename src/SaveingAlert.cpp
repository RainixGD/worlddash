#include "SaveingAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool SaveingAlert::init() {
	if (!FLAlertLayer::init(nullptr, "Saving!", "", "cancel", nullptr, 200, 0, 170)) {
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
	message.set("json", json::Value(DataManager::getInstance()->packAllLocalLevels2Json()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "save", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
	);

	return true;
}

void SaveingAlert::onResponse(std::string response) {
	auto json = json::parse(response);
	if (json["ok"].as_bool())
		FLAlertLayer::create("GG!", json["message"].as_string(), "ok")->show();
	else
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();

	FLAlertLayer::keyBackClicked();
}
void SaveingAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void SaveingAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

SaveingAlert* SaveingAlert::create() {
	auto ret = new SaveingAlert();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
