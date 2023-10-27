#include "PublicLevelAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool PublicLevelAlert::init() {
	if (!FLAlertLayer::init(nullptr, "Loading!", "", "cancel", nullptr, 200, 0, 170)) {
		return false;
	}

	m_buttonMenu->removeAllChildren();

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto loadingCircle = MyLoadingCircle::create();
	loadingCircle->setPosition(size / 2);
	loadingCircle->setZOrder(5);
	m_mainLayer->addChild(loadingCircle);

	auto level = DataManager::getInstance()->getLocalLevelById(popup->selectedLevelId);
	//TODO check if level exists

	auto message = json::Value();
	message.set("x", json::Value(popup->selectedPlusX));
	message.set("y", json::Value(popup->selectedPlusY));
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));
	message.set("rentPeriod", json::Value(popup->days));
	message.set("levelName", json::Value(level["levelName"]));
	message.set("levelString", json::Value(level["levelString"]));
	message.set("songId", json::Value(level["songId"]));
	message.set("isLocalSong", json::Value(level["isLocalSong"]));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "publicLevel", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void PublicLevelAlert::onResponse(std::string response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		popup->updateMapAndCoinsCallback();
		popup->keyBackClicked();
		FLAlertLayer::create("GG!", "Now all the players can see your epic level!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();
}
void PublicLevelAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void PublicLevelAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

PublicLevelAlert* PublicLevelAlert::create(PublicMapLevelPopup* popup) {
	auto ret = new PublicLevelAlert();
	ret->popup = popup;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
