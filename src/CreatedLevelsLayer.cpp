#include "CreatedLevelsLayer.h"
#include "DataManager.h"
#include "Button.h"
#include "MyLevelTools.h"

using namespace geode::prelude;


bool CreatedLevelsLayer::init(){
	if (!CCLayer::init()) {
		return false;
	}
	auto size = CCDirector::sharedDirector()->getWinSize();

	bg = CCSprite::create("game_bg_14_001.png");
	bg->setColor(ccColor3B(0, 100, 150));
	bg->setPosition({ size.width / 2, size.width / 2 });
	bg->setScale(size.width / bg->getContentSize().width);
	this->addChild(bg);

	alert = GJListLayer::create(nullptr, "my levels", ccColor4B(191, 114, 62, 255), 356, 220);
	alert->setPosition((size - alert->getContentSize()) / 2);
	this->addChild(alert);

	levels = CCArray::create();
	for (json::Value json : DataManager::getInstance()->localLevels) {
		levels->addObject(static_cast<CCObject*>(MyLevelTools::json2Level(json)));
	}

	list = CustomListView::create(levels, BoomListType::Level, 220, 350);
	list->setPosition({ 3, 0 });
	alert->addChild(list);

	auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	auto backBtn = Button::create(backSprite, this, menu_selector(CreatedLevelsLayer::onBack));
	backBtn->setPosition({ 20, size.height - 25 });
	this->addChild(backBtn);


	auto newSprite = CCSprite::createWithSpriteFrameName("GJ_newBtn_001.png");
	auto newBtn = Button::create(newSprite, this, menu_selector(CreatedLevelsLayer::onNew));
	newBtn->setPosition({ size.width - 40, 40 });
	this->addChild(newBtn);

	this->setKeypadEnabled(true);

	return 1;
}

void CreatedLevelsLayer::onNew(CCObject*) {
	auto editLevelLayer = EditLevelLayer::scene(DataManager::getInstance()->createLocalLevel());
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, editLevelLayer));

}

void CreatedLevelsLayer::onBack(CCObject*) {
	keyBackClicked();
}

void CreatedLevelsLayer::keyBackClicked() {
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, MenuLayer::scene(0)));
}

CreatedLevelsLayer* CreatedLevelsLayer::create() {
	auto ret = new CreatedLevelsLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}