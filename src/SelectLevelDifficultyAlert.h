#pragma once

using namespace geode::prelude;

class SelectLevelDifficultyAlert : public geode::Popup<> {
protected:
	bool setup() override;
	void onSelect(CCObject*);
	void onRate(CCObject*);
	void updateInterface();
public:
	static SelectLevelDifficultyAlert* create(int levelId);
private:
	CCMenu* interfaceButtons;
	int levelId;
	int selectedDifficulty = 0;
};