#include "DailyLoadingAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"
#include "DailyChestPopup.h"

using namespace geode::prelude;


bool DailyLoadingAlert::init() {
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

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "getChestsInfo", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void DailyLoadingAlert::onResponse(std::string response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		DailyChestPopup::create(json["time1"].as_int(), json["time2"].as_int(), json["time3"].as_int(), json["time4"].as_int())->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();
}
void DailyLoadingAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void DailyLoadingAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

DailyLoadingAlert* DailyLoadingAlert::create() {
	auto ret = new DailyLoadingAlert();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
