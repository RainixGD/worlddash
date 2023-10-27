using namespace geode::prelude;
#include "Button.h"

bool Button::init(CCSprite* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector) {
	if (!CCMenu::init()) return false;
	this->sprite = sprite;
	button = CCMenuItemSpriteExtra::create(sprite, target, selector);
	this->addChild(button);
	return true;
}

Button* Button::create(CCSprite* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector) {
	auto ret = new Button();
	if (ret && ret->init(sprite, target, selector)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}