#include "JumpingMonster.h"

using namespace geode::prelude;

bool JumpingMonster::init() {
	if (!CCMenu::init()) return false;

	CCMenu* monsterWithoutShadow = CCMenu::create();
	monsterWithoutShadow->setAnchorPoint({ 0.5, 0.5 });
	monsterWithoutShadow->setContentSize({ 0, 0 });
	monsterWithoutShadow->setPosition({ 0, 0 });
	this->addChild(monsterWithoutShadow);
	CCSequence* monsterWithoutShadowAnim = CCSequence::createWithTwoActions(CCEaseSineOut::create(CCMoveBy::create(0.3, { 0, 60 })), CCEaseSineIn::create(CCMoveBy::create(0.3, { 0, -60 })));
	monsterWithoutShadow->runAction(CCRepeatForever::create(monsterWithoutShadowAnim));

	auto monsterFirstPart = CCSprite::createWithSpriteFrameName("GJBeast01_01_001.png");
	monsterFirstPart->setAnchorPoint({ 0.85, 0.35 });
	monsterFirstPart->setPosition({ -9, 0 });
	monsterFirstPart->setColor({ 0, 0, 0 });
	monsterWithoutShadow->addChild(monsterFirstPart);
	CCSequence* monsterFirstPartAnim = CCSequence::createWithTwoActions(CCEaseSineIn::create(CCRotateTo::create(0.36, 0)), CCEaseSineOut::create(CCRotateTo::create(0.24, 35)));
	monsterFirstPart->runAction(CCRepeatForever::create(monsterFirstPartAnim));


	auto monsterSecondPart = CCSprite::createWithSpriteFrameName("GJBeast01_02_001.png");
	monsterSecondPart->setAnchorPoint({ 0.91, 0.85 });
	monsterSecondPart->setPosition({ -5, 0.8 });
	monsterSecondPart->setColor({ 0, 0, 0 });
	monsterWithoutShadow->addChild(monsterSecondPart);
	CCSequence* monsterSecondPartAnim = CCSequence::createWithTwoActions(CCEaseSineOut::create(CCRotateTo::create(0.36, 0)), CCEaseSineIn::create(CCRotateTo::create(0.24, -40)));
	monsterSecondPart->runAction(CCRepeatForever::create(monsterSecondPartAnim));

	auto monsterEye = CCSprite::createWithSpriteFrameName("GJBeast01_03_001.png");
	monsterEye->setPosition({ 33, 18 });
	monsterFirstPart->addChild(monsterEye);

	auto dropshadow = CCSprite::createWithSpriteFrameName("GJBeast01_03_001.png");
	dropshadow->setAnchorPoint({ 0.25, 0.25 });
	dropshadow->setColor({ 0, 0, 0 });
	dropshadow->setScale(0.8);
	dropshadow->setPosition({ -30, -60 });
	dropshadow->setOpacity(100);
	CCSequence* dropshadowAnim = CCSequence::createWithTwoActions(CCEaseSineOut::create(CCScaleBy::create(0.3, 4, 2)), CCEaseSineIn::create(CCScaleBy::create(0.3, 0.25, 0.5)));
	dropshadow->runAction(CCRepeatForever::create(dropshadowAnim));
	this->addChild(dropshadow);

	this->setContentSize({ 0, 0 });

	return true;
}


JumpingMonster* JumpingMonster::create() {
	auto ret = new JumpingMonster();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}