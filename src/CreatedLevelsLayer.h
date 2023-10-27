#pragma once

using namespace geode::prelude;


class CreatedLevelsLayer : public CCLayer {
	GJListLayer* alert;
	CCSprite* bg;
	CustomListView* list;
	CCArray* levels;
	virtual bool init() override;
	void onNew(CCObject*);
	void onBack(CCObject*);
	void keyBackClicked() override;
public:
	static CreatedLevelsLayer* create();
};