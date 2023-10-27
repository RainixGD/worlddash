#pragma once
#include "Button.h"

using namespace geode::prelude;

class FallingChestAnimationAlert : public FLAlertLayer {
	virtual bool init();
	int prize;
	int chestTag;

	CCSprite* fullChest;
	CCSprite* halfOpenedChest;
	CCSprite* chestBack;
	CCSprite* chestGlow;
	CCSprite* openedChestPart1;
	CCSprite* openedChestPart2;
	CCMenu* prizeMenu;
	Button* backBtn;

	void switchToClosed();
	void switchToHalfOpen();
	void switchToOpen();
	void showPrize();
	void showBack();
	void onBack(CCObject*);
public:
	static FallingChestAnimationAlert* create(int chestTag, int prize);
};