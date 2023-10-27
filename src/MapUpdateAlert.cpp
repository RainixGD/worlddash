#include "MapUpdateAlert.h"
#include "MyLoadingCircle.h"
#include "DataManager.h"
#include "ReqManager.h"
#include "MapUiLayer.h"

using namespace geode::prelude;


bool MapUpdateAlert::init() {
	if (!FLAlertLayer::init(nullptr, "Updating Map!", "", "cancel", nullptr, 200, 0, 170)) {
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

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "getMapLevels", message,
		[this](std::string response) {
			this->onResponse(response);
		},
		[this](std::string error) {
			this->onError(error);
		}
		);

	return true;
}

void MapUpdateAlert::onResponse(std::string response) {
	auto json = json::parse(response);
	mapLayer->updateMap(json["levels"].as_array());
	FLAlertLayer::keyBackClicked();
}
void MapUpdateAlert::onError(std::string error) {
	FLAlertLayer::create("Error!", error, "ok")->show();
	FLAlertLayer::keyBackClicked();
}

void MapUpdateAlert::keyBackClicked() {
	ReqManager::getInstance()->cancelReqById(reqId);
	FLAlertLayer::keyBackClicked();
}

MapUpdateAlert* MapUpdateAlert::create(MapLayer* mapLayer) {
	auto ret = new MapUpdateAlert();
	ret->mapLayer = mapLayer;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
