#pragma once
#include <Geode/Geode.hpp>
#include <functional>
using namespace geode::prelude;

class CounterNode : public CCMenu {
	CCScale9Sprite* valueBG;
	CCMenuItemSpriteExtra* plusBtn;
	CCMenuItemSpriteExtra* minusBtn;
	CCLabelBMFont* valueLabel;
	int step;
	int minValue;
	int maxValue;

	std::function<void(int)> onChangeCallback;

	virtual bool init();
	void onPlus(CCObject*);
	void onMinus(CCObject*);

public:
	static CounterNode* create(int defaultValue, int step, int minValue, int maxValue, std::function<void(int)> onChange);
	int value;
	void updateLabel();
};