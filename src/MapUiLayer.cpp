#include "./MapUiLayer.h"
#include "./Button.h"
#include "./MapLayer.h"
#include "./DataManager.h"
#include "./ReqManager.h"
#include "./MyLoadingCircle.h"
#include "./MapUpdateAlert.h"

using namespace geode::prelude;


bool MapUiLayer::init() {
	if (!CCLayer::init()) {
		return false;
	}

	mapLayer = MapLayer::create(this);
	thisScene->addChild(mapLayer);

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	auto backBtn = Button::create(backSprite, this, menu_selector(MapUiLayer::backToMainScene));
	backBtn->setPosition({ 20, size.height - 25 });
	this->addChild(backBtn);


	coinsAlert = CCScale9Sprite::create("square02_small.png", CCRect(0, 0, 40, 40));
	coinsAlert->setContentSize({ 100, 40 });
	coinsAlert->setOpacity(100);
	coinsAlert->setPosition({ size.width - coinsAlert->getContentSize().width / 2, size.height - coinsAlert->getContentSize().height / 2 });
	this->addChild(coinsAlert);

	auto coinSprite = CCSprite::createWithSpriteFrameName("susCoin.png"_spr);
	coinSprite->setPosition({ coinSprite->getContentSize().width / 2 + 5, coinsAlert->getContentSize().height / 2 });
	coinsAlert->addChild(coinSprite);

	coinsLabel = CCLabelBMFont::create("", "goldFont.fnt");
	coinsLabel->setScale(0.7);
	coinsLabel->setPosition({ 65, coinsAlert->getContentSize().height / 2 });
	coinsAlert->addChild(coinsLabel);

	coinsLoadingCircle = MyLoadingCircle::create();
	coinsLoadingCircle->setPosition({ 65, coinsAlert->getContentSize().height / 2 });
	coinsLoadingCircle->setZOrder(5);
	coinsLoadingCircle->setScale(0.4);
	coinsAlert->addChild(coinsLoadingCircle);


	auto publicLevelSprite = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
	publicLevelSprite->setScale(0.5);
	publicLevelBtn = Button::create(publicLevelSprite, this, menu_selector(MapUiLayer::onTryPublic));
	publicLevelBtn->setPosition({ size.width - publicLevelSprite->getContentSize().width / 2, size.height - coinsAlert->getContentSize().height / 2 - publicLevelSprite->getContentSize().height / 2 - 10 });
	publicLevelBtn->setVisible(0);
	this->addChild(publicLevelBtn);

	auto updateMapBtnSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	updateMapBtnSprite->setScale(0.7);
	auto updateMapBtn = Button::create(updateMapBtnSprite, this, menu_selector(MapUiLayer::onUpdateMap));
	updateMapBtn->setPosition({ size.width - 20, 20 });
	this->addChild(updateMapBtn);

	snow = CCParticleSystemQuad::create("snow.plist"_spr);
	auto oldColor = snow->getStartColor();
	snow->setStartColor({ oldColor.r, oldColor.g, oldColor.b, 0 });
	snow->setEndColor({ oldColor.r, oldColor.g, oldColor.b, 0 });
	snow->setEndColorVar({ 0, 0, 0, 0 });
	snow->setZOrder(-1);
	snow->setPosition({ size.width / 2, size.height });
	this->addChild(snow);

	this->setKeypadEnabled(true);

	return true;
}

void MapUiLayer::updateCoins() {
	auto message = json::Value();
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));
	
	getCoinsReqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "getCoins", message,
		[this](std::string response) {
			this->onGetCoinsResponse(response);
		},
		[this](std::string error) {
			this->onGetCoinsError(error);
		}
	);
}

void MapUiLayer::onGetCoinsResponse(std::string const& response) {
	auto json = json::parse(response);
	
	if (json["ok"].as_bool()) {
		coinsLoadingCircle->setVisible(0);
		if (!mapLayer->isAddLevelMode)
			publicLevelBtn->setVisible(1);
		coinsAlert->setVisible(1);
		coinsLabel->setString(std::to_string(json["count"].as_int()).c_str());
	}
	else {
		coinsAlert->setVisible(0);
	}
}

void MapUiLayer::onUpdateMap(CCObject*) {
	mapLayer->updateMapAndCoins();
}

void MapUiLayer::onGetCoinsError(std::string const& error) {
	coinsAlert->setVisible(0);
}

void MapUiLayer::backToMainScene(CCObject*) {
	keyBackClicked();
}

MapUiLayer::~MapUiLayer() {
	log::info("destructor");
	DataManager::isMapOpened = 0;
	ReqManager::getInstance()->cancelReqById(getCoinsReqId);
}

void MapUiLayer::keyBackClicked() {
	if (mapLayer->isAddLevelMode) {
		mapLayer->isAddLevelMode = 0;
		mapLayer->mapDecoContainer->setVisible(1);
		mapLayer->showLevels();
		mapLayer->hideAddBtns();
		mapLayer->map->setColor(ccc3(255, 255, 255));
		publicLevelBtn->setVisible(1);
	}
	else {
		ReqManager::getInstance()->cancelReqById(getCoinsReqId);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, MenuLayer::scene(0)));
	}
}

void MapUiLayer::onTryPublic(CCObject*) {
	mapLayer->isAddLevelMode = 1;
	mapLayer->mapDecoContainer->setVisible(0);
	mapLayer->hideLevels();
	mapLayer->showAddBtns();
	mapLayer->map->setColor(ccc3(0, 255, 0));
	publicLevelBtn->setVisible(0);
}


MapUiLayer* MapUiLayer::create(CCScene* thisScene) {
	DataManager::isMapOpened = 1;
	auto ret = new MapUiLayer();
	ret->thisScene = thisScene;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
