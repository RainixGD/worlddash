#pragma once

using namespace geode::prelude;

class DailyChestPopup : public geode::Popup<> {
protected:
	bool setup() override;
	void onChest(CCObject*);
	std::string unix2DateString(int seconds);
	int time1;
	int time2;
	int time3;
	int time4;

	CCSprite* chestSprite1;
	CCSprite* chestSprite2;
	CCSprite* chestSprite3;
	CCSprite* chestSprite4;
public:
	void hideChestByTag(int tag);
	static DailyChestPopup* create(int time1, int time2, int time3, int time4);
};