#include "UpdateLevelAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool UpdateLevelAlert::init() {
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
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));
	message.set("id", json::Value(popup->lvlJson["id"].as_int()));
	message.set("levelName", json::Value(level["levelName"]));
	message.set("levelString", json::Value(level["levelString"]));
	message.set("songId", json::Value(level["songId"]));
	message.set("isLocalSong", json::Value(level["isLocalSong"]));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "updateLevel", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void UpdateLevelAlert::onResponse(std::string response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		popup->updateMapAndCoinsCallback();
		popup->closeLevelPopupCallback();
		popup->keyBackClicked();
		FLAlertLayer::create("GG!", "Your level is updated!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();
}
void UpdateLevelAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void UpdateLevelAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

UpdateLevelAlert* UpdateLevelAlert::create(PublicLevelOptionsPopup* popup) {
	auto ret = new UpdateLevelAlert();
	ret->popup = popup;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}