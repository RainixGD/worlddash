#include "TryOpenChestAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"
#include "FallingChestAnimationAlert.h"

using namespace geode::prelude;


bool TryOpenChestAlert::init() {
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
	message.set("chestTag", json::Value(chestTag));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "openChest", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void TryOpenChestAlert::onResponse(std::string response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		FallingChestAnimationAlert::create(chestTag, json["prize"].as_int())->show();
		hideChest();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();
}
void TryOpenChestAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void TryOpenChestAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

TryOpenChestAlert* TryOpenChestAlert::create(int chestTag, std::function<void()> hideChest) {
	auto ret = new TryOpenChestAlert();
	ret->chestTag = chestTag;
	ret->hideChest = hideChest;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
