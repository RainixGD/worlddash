#include "FlyingMonster.h"

using namespace geode::prelude;

bool FlyingMonster::init(bool isSleeping, bool hideGlow) {
	if (!CCMenu::init()) return false;

	animatedGameObject = AnimatedGameObject::create(1584);
	animatedGameObject->playAnimation(isSleeping ? 7 : 0);
	animatedGameObject->updateChildSpriteColor(ccc3(0, 0, 0));//glow
	animatedGameObject->setChildColor(ccc3(255, 255, 255));//second color
	animatedGameObject->setObjectColor(ccc3(0, 0, 0)); //first color
	this->addChild(animatedGameObject);
	animatedGameObject->sortAllChildren();

	auto animatedSprite = static_cast<CCNode*>(animatedGameObject->getChildren()->objectAtIndex(0));
	CCObject* obj;
	CCARRAY_FOREACH(animatedSprite->getChildren(), obj) {
		auto node = static_cast<CCSprite*>(obj);
		if (node->getColor() == ccc3(0, 0, 0)) { //!!! we are setting setObjectColor = black in init. And now we are using it to find main object
			CCObject* obj;
			CCARRAY_FOREACH(node->getChildren(), obj) {
				auto node = static_cast<CCSprite*>(obj);
				if (node->getContentSize().width == 6 && node->getContentSize().height == 6)
					node->setColor(ccc3(255, 255, 255));
			}
		}
	}

	this->setContentSize({ 0, 0 });
	if (hideGlow) this->hideGlow();
	return true;
}

void FlyingMonster::hideGlow() {
	auto animatedSprite = static_cast<CCNode*>(animatedGameObject->getChildren()->objectAtIndex(0));
	CCObject* obj;
	CCARRAY_FOREACH(animatedSprite->getChildren(), obj) {
		auto node = static_cast<CCSprite*>(obj);
		if (node->getColor() == ccc3(255, 255, 255)) { //!!! we are setting updateChildSpriteColor = white in init. And now we are using it to find glow object
			node->setVisible(0);
		}
	}
}

FlyingMonster* FlyingMonster::create(bool isSleeping, bool hideGlow) {
	auto ret = new FlyingMonster();
	if (ret && ret->init(isSleeping, hideGlow)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}