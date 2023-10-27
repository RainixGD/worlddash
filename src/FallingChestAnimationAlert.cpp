#include "FallingChestAnimationAlert.h"

using namespace geode::prelude;

bool FallingChestAnimationAlert::init() {
	if (!FLAlertLayer::init(nullptr, "", "", "", nullptr, 200, 0, 170)) {
		return false;
	}

	m_mainLayer->removeAllChildren();

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto chestMenu = CCMenu::create();
	chestMenu->setPosition({ size.width / 2, size.height + 30 });
	this->m_mainLayer->addChild(chestMenu);

	fullChest = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_02_001.png").c_str());
	fullChest->setVisible(0);
	chestMenu->addChild(fullChest);

	halfOpenedChest = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_03_001.png").c_str());
	halfOpenedChest->setVisible(0);
	chestMenu->addChild(halfOpenedChest);

	chestBack = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_03_back_001.png").c_str());
	chestBack->setPositionY(2);
	chestBack->setVisible(0);
	chestBack->setZOrder(-1);
	chestMenu->addChild(chestBack);

	chestGlow = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_04_glow_001.png").c_str());
	chestGlow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	chestGlow->setVisible(0);
	chestMenu->addChild(chestGlow);

	openedChestPart1 = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_04_001.png").c_str());
	openedChestPart1->setVisible(0);
	openedChestPart1->setZOrder(2);
	chestMenu->addChild(openedChestPart1);

	openedChestPart2 = CCSprite::createWithSpriteFrameName(("chest_0" + std::to_string(chestTag) + "_04_back_001.png").c_str());
	openedChestPart2->setVisible(0);
	openedChestPart2->setZOrder(-1);
	chestMenu->addChild(openedChestPart2);


	prizeMenu = CCMenu::create();
	prizeMenu->setVisible(0);
	prizeMenu->setPosition(-10, -15);
	chestMenu->addChild(prizeMenu);

	auto prizeLabel = CCLabelBMFont::create(std::to_string(prize).c_str(), "bigFont.fnt");
	prizeMenu->addChild(prizeLabel);

	auto susIcon = CCSprite::createWithSpriteFrameName("susCoin.png"_spr);
	susIcon->setPosition({ prizeLabel->getPositionX() + prizeLabel->getContentSize().width / 2 + 10, -2 });
	prizeMenu->addChild(susIcon);

	backBtn = Button::create(ButtonSprite::create("ok"), this, menu_selector(FallingChestAnimationAlert::onBack));
	backBtn->setPosition({ size.width / 2, 30 });
	backBtn->setVisible(0);
	this->m_mainLayer->addChild(backBtn);


	switchToClosed();

	auto fallingAnimation = CCSequence::create( CCEaseBounceOut::create(CCMoveTo::create(1.0, size / 2)), nullptr);
	auto halfOpenCall = CCCallFunc::create(this, callfunc_selector(FallingChestAnimationAlert::switchToHalfOpen));


	auto shakeAnimation = CCSequence::create(
		CCMoveBy::create(0.05, ccp(10, 0)),
		CCMoveBy::create(0.05, ccp(-20, 0)),
		CCMoveBy::create(0.05, ccp(20, 0)),
		CCMoveBy::create(0.05, ccp(-10, 0)),
		CCMoveBy::create(0.05, ccp(10, 0)),
		CCMoveBy::create(0.05, ccp(-20, 0)),
		CCMoveBy::create(0.05, ccp(20, 0)),
		CCMoveBy::create(0.05, ccp(-10, 0)),
		nullptr
	);
	auto openCall = CCCallFunc::create(this, callfunc_selector(FallingChestAnimationAlert::switchToOpen));

	auto showPrizeCall = CCCallFunc::create(this, callfunc_selector(FallingChestAnimationAlert::showPrize));

	auto showBackCall = CCCallFunc::create(this, callfunc_selector(FallingChestAnimationAlert::showBack));

	auto sequence = CCSequence::create(
		fallingAnimation,
		halfOpenCall,
		shakeAnimation,
		openCall,
		showPrizeCall,
		showBackCall,
		nullptr
	);

	chestMenu->runAction(sequence);

	return true;
}

void FallingChestAnimationAlert::onBack(CCObject*) {
	FallingChestAnimationAlert::keyBackClicked();
}

void FallingChestAnimationAlert::switchToClosed() {
	fullChest->setVisible(1);
	halfOpenedChest->setVisible(0);
	chestBack->setVisible(0);
	chestGlow->setVisible(0);
	openedChestPart1->setVisible(0);
	openedChestPart2->setVisible(0);
	prizeMenu->setVisible(0);
}
void FallingChestAnimationAlert::switchToHalfOpen() {
	fullChest->setVisible(0);
	halfOpenedChest->setVisible(1);
	chestBack->setVisible(1);
	chestGlow->setVisible(0);
	openedChestPart1->setVisible(0);
	openedChestPart2->setVisible(0);
	prizeMenu->setVisible(0);
}
void FallingChestAnimationAlert::switchToOpen() {
	fullChest->setVisible(0);
	halfOpenedChest->setVisible(0);
	chestBack->setVisible(0);
	chestGlow->setVisible(1);
	openedChestPart1->setVisible(1);
	openedChestPart2->setVisible(1);
	prizeMenu->setVisible(1);
}
void FallingChestAnimationAlert::showPrize() {
	prizeMenu->runAction(CCEaseBackOut::create(CCMoveTo::create(0.4, ccp(-10, 25))));
}

void FallingChestAnimationAlert::showBack() {
	backBtn->setVisible(1);
}

FallingChestAnimationAlert* FallingChestAnimationAlert::create(int chestTag, int prize) {
	auto ret = new FallingChestAnimationAlert();
	ret->chestTag = chestTag;
	ret->prize = prize;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
