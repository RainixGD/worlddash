#include "./DailyChestPopup.h"
#include "./Button.h"
#include "./TryOpenChestAlert.h"
#include "./FallingChestAnimationAlert.h"
#include <functional>
using namespace geode::prelude;


bool DailyChestPopup::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("Daily Chests");

	m_closeBtn->setZOrder(5);

	auto corner1 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner1->setPosition({ m_size.width / 2 - corner1->getContentSize().width / 2, m_size.height / 2 - corner1->getContentSize().height / 2 });
	corner1->setRotation(180);
	m_buttonMenu->addChild(corner1);

	auto corner2 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner2->setPosition({ -m_size.width / 2 + corner2->getContentSize().width / 2, m_size.height / 2 - corner2->getContentSize().height / 2 });
	corner2->setRotation(90);
	m_buttonMenu->addChild(corner2);

	auto corner3 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner3->setPosition({ m_size.width / 2 - corner3->getContentSize().width / 2, -m_size.height / 2 + corner3->getContentSize().height / 2 });
	corner3->setRotation(270);
	m_buttonMenu->addChild(corner3);

	auto corner4 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner4->setPosition({ -m_size.width / 2 + corner4->getContentSize().width / 2, -m_size.height / 2 + corner4->getContentSize().height / 2 });
	m_buttonMenu->addChild(corner4);

	
	chestSprite1 = CCSprite::createWithSpriteFrameName("chest_01_02_001.png");
	if (time1 > 0) {
		chestSprite1->setColor(ccc3(100, 100, 100));
		auto chestLabel1 = CCLabelBMFont::create(unix2DateString(time1).c_str(), "goldFont.fnt");
		chestLabel1->limitLabelWidth(100, 0.6, 0.05);
		chestLabel1->setPosition({ -150, -50 });
		this->m_buttonMenu->addChild(chestLabel1);
	}
	auto chest1 = Button::create(chestSprite1, this, (time1 > 0) ? nullptr : menu_selector(DailyChestPopup::onChest));
	chest1->button->setTag(1);
	chest1->setPosition({ -150, 0 });
	this->m_buttonMenu->addChild(chest1);


	chestSprite2 = CCSprite::createWithSpriteFrameName("chest_02_02_001.png");
	chestSprite2->setScale(0.8);
	if (time2 > 0) {
		chestSprite2->setColor(ccc3(100, 100, 100));
		auto chestLabel2 = CCLabelBMFont::create(unix2DateString(time2).c_str(), "goldFont.fnt");
		chestLabel2->limitLabelWidth(100, 0.6, 0.05);
		chestLabel2->setPosition({ -50, -50 });
		this->m_buttonMenu->addChild(chestLabel2);
	}
	auto chest2 = Button::create(chestSprite2, this, (time2 > 0) ? nullptr : menu_selector(DailyChestPopup::onChest));
	chest2->button->setTag(2);
	chest2->setPosition({ -50, 0 });
	this->m_buttonMenu->addChild(chest2);


	chestSprite3 = CCSprite::createWithSpriteFrameName("chest_03_02_001.png");
	chestSprite3->setScale(0.8);
	if (time3 > 0) {
		chestSprite3->setColor(ccc3(100, 100, 100));
		auto chestLabel3 = CCLabelBMFont::create(unix2DateString(time3).c_str(), "goldFont.fnt");
		chestLabel3->limitLabelWidth(100, 0.6, 0.05);
		chestLabel3->setPosition({ 50, -50 });
		this->m_buttonMenu->addChild(chestLabel3);
	}
	auto chest3 = Button::create(chestSprite3, this, (time3 > 0) ? nullptr : menu_selector(DailyChestPopup::onChest));
	chest3->button->setTag(3);
	chest3->setPosition({ 50, 0 });
	this->m_buttonMenu->addChild(chest3);


	chestSprite4 = CCSprite::createWithSpriteFrameName("chest_04_02_001.png");
	chestSprite4->setScale(0.8);
	if (time4 > 0) {
		chestSprite4->setColor(ccc3(100, 100, 100));
		auto chestLabel4 = CCLabelBMFont::create(unix2DateString(time4).c_str(), "goldFont.fnt");
		chestLabel4->limitLabelWidth(100, 0.6, 0.05);
		chestLabel4->setPosition({ 150, -50 });
		this->m_buttonMenu->addChild(chestLabel4);
	}
	auto chest4 = Button::create(chestSprite4, this, (time4 > 0) ? nullptr : menu_selector(DailyChestPopup::onChest));
	chest4->button->setTag(4);
	chest4->setPosition({ 150, 0 });
	this->m_buttonMenu->addChild(chest4);

	return true;
}

void DailyChestPopup::onChest(CCObject* obj) {
	auto tag = obj->getTag();
	TryOpenChestAlert::create(tag, [this, tag]() { this->hideChestByTag(tag); })->show();
}

void DailyChestPopup::hideChestByTag(int tag) {
	switch (tag) {
	case 1:
		chestSprite1->setColor({ 100, 100, 100 });
		break;
	case 2:
		chestSprite2->setColor({ 100, 100, 100 });
		break;
	case 3:
		chestSprite3->setColor({ 100, 100, 100 });
		break;
	case 4:
		chestSprite4->setColor({ 100, 100, 100 });
		break;
	default:
		break;
	}
}

std::string DailyChestPopup::unix2DateString(int seconds) {
	std::string endString = "";
	if (seconds >= 60 * 60 * 24) {
		auto days = seconds / (60 * 60 * 24);
		seconds -= days * (60 * 60 * 24);
		endString += (std::to_string(days) + " days ");
	}
	if (seconds >= 60 * 60) {
		auto hours = seconds / (60 * 60);
		seconds -= hours * (60 * 60);
		endString += (std::to_string(hours) + " hours ");
	}
	if (seconds >= 60) {
		auto minutes = seconds / (60);
		seconds -= minutes * (60);
		endString += (std::to_string(minutes) + " minutes ");
	}
	if (endString == "") {
		endString = "< 1 minute";
	}
	return endString;
}


DailyChestPopup* DailyChestPopup::create(int time1, int time2, int time3, int time4) {
	auto ret = new DailyChestPopup();
	ret->time1 = time1;
	ret->time2 = time2;
	ret->time3 = time3;
	ret->time4 = time4;
	if (ret && ret->init(420.f, 200.f)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}