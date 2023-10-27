#include "PayMoreForPublicLevelAlert.h"
#include "PublicLevelOptionsPopup.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool PayMoreForPublicLevelAlert::init() {
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
	message.set("id", json::Value(popup->lvlJson["id"].as_int()));
	message.set("rentPeriod", json::Value(popup->days));
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "payMoreForRent", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void PayMoreForPublicLevelAlert::onResponse(std::string response) {
	auto json = json::parse(response);
	if (json["ok"].as_bool()) {
		popup->updateMapAndCoinsCallback();
		popup->closeLevelPopupCallback();
		popup->keyBackClicked();
		FLAlertLayer::create("Ok", "Payment Completed!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();

}
void PayMoreForPublicLevelAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void PayMoreForPublicLevelAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

PayMoreForPublicLevelAlert* PayMoreForPublicLevelAlert::create(PublicLevelOptionsPopup* popup) {
	auto ret = new PayMoreForPublicLevelAlert();
	ret->popup = popup;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}