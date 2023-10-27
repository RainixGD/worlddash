#pragma once

using namespace geode::prelude;

class PublicLevelOptionsPopup : public geode::Popup<> {
	CCMenuItemSpriteExtra* payMoreForLevelBtn;
	CCMenuItemSpriteExtra* updateLevelBtn;
	CCLabelBMFont* selectedLevel;
	CCLabelBMFont* resultLabel;
	CCSprite* coin;
protected:
	bool setup() override;
	virtual void onClose(CCObject* obj) override;
	std::string unix2DateString(int seconds);
	void onLevelInListClicked(CCObject*);

	void onUpdateLevel(CCObject*);
	void onPayMoreForLevel(CCObject*);

	void onTimeCounterNodeChanged(int);
	void fixCoinPosition();
public:
	std::function<void()> updateMapAndCoinsCallback;
	std::function<void()> closeLevelPopupCallback;
	json::Value lvlJson;
	int selectedLevelId;
	int days = 1;
	int coins;

	const int coinsProDays = 15;

	static PublicLevelOptionsPopup* create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback, std::function<void()> closeLevelPopupCallback);
};