#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Button : public CCMenu {
	virtual bool init(CCSprite* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
public:
	CCSprite* sprite;
	CCMenuItemSpriteExtra* button;
	static Button* create(CCSprite* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
};