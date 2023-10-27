#pragma once

using namespace geode::prelude;

class PublicMapLevelPopup : public geode::Popup<> {
	CCMenuItemSpriteExtra* publicBtn;
	CCLabelBMFont* selectedLevel;
	CCLabelBMFont* resultLabel;
	CCSprite* coin;
protected:
	bool setup() override;
	virtual void onClose(CCObject* obj) override;
	void publicLevel(CCObject*);
	void onLevelInListClicked(CCObject*);

	void onTimeCounterNodeChanged(int);
	void fixCoinPosition();
public:
	std::function<void()> updateMapAndCoinsCallback;
	int selectedPlusX;
	int selectedPlusY;
	int selectedLevelId = 0;
	int days = 1;
	int coins;

	const int coinsProDays = 15;

	static PublicMapLevelPopup* create(int x, int y, std::function<void()> updateMapAndCoinsCallback);
};