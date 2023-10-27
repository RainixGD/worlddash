#include "RateDifficultyLevelAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool RateDifficultyLevelAlert::init(int levelId) {
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
	message.set("id", json::Value(levelId));
	message.set("difficulty", json::Value(ratedDifficulty));
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "rateDifficultyLevel", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void RateDifficultyLevelAlert::onResponse(std::string response) {
	auto json = json::parse(response);
	if (json["ok"].as_bool()) {
		log::info("{}", json["message"].as_string());
		FLAlertLayer::create("Ok", "Level is Rated!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
	}
	FLAlertLayer::keyBackClicked();

}
void RateDifficultyLevelAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void RateDifficultyLevelAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

RateDifficultyLevelAlert* RateDifficultyLevelAlert::create(int levelId, int ratedDifficulty) {
	auto ret = new RateDifficultyLevelAlert();
	ret->ratedDifficulty = ratedDifficulty;
	if (ret && ret->init(levelId)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}