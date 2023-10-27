#include "./RainixPopup.h"
#include "./Button.h"
using namespace geode::prelude;

bool RainixPopup::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("");

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

	float height = 100;
	for (std::string string : strings) {
		auto textarea = TextArea::create(string, "chatFont.fnt", 1, 380, { 0.5, 0.5 }, 10, 0);
		textarea->setPositionY(height);
		this->m_buttonMenu->addChild(textarea);
		height -= 25;
	}

	return true;
}


RainixPopup* RainixPopup::create() {
	auto ret = new RainixPopup();
	if (ret && ret->init(425.f, 270.f, "GJ_square05.png")) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

std::vector<std::string> RainixPopup::strings = {
	"<cy>Hi!</c>",
	"This mod was created in a month for <cg>modtober event</c>.",
	"I spent more than <cr>150 hours</c> on this.",
	"But I think this idea is <co>epic</c> and you will enjoy it!",
	"<cb>Special thanks:</c>",
	"<cr>HJfod</c> - (helped me figure out how to develop using geode)",
	"<cg>OKZ, Arti, Viral, Zemonkamin</c> - (thanks for the beta test)",
	"<cy>RobTopGames</c> - (thank you so much for this legendary game <cr><3</c>)",
	">w<",
};