using namespace geode::prelude;
#include "CounterNode.h"
#include <functional>

bool CounterNode::init() {
	if (!CCMenu::init()) return false;
	
	valueBG = CCScale9Sprite::create("square02_small.png", CCRect(0, 0, 40, 40));
	valueBG->setContentSize({ 50, 30 });
	valueBG->setOpacity(50);
	this->addChild(valueBG);

	plusBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("+"), this, menu_selector(CounterNode::onPlus));
	plusBtn->setPosition({ 43, 0 });
	this->addChild(plusBtn);
	minusBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("-"), this, menu_selector(CounterNode::onMinus));
	minusBtn->setPosition({ -43, 0 });
	this->addChild(minusBtn);

	valueLabel = CCLabelBMFont::create(std::to_string(value).c_str(), "goldFont.fnt");
	valueLabel->limitLabelWidth(45, 0.7, 0.1);
	this->addChild(valueLabel);

	return true;
}

void CounterNode::onPlus(CCObject*) {
	if (value + step <= maxValue) {
		value += step;
		updateLabel();
		onChangeCallback(value);
	}
}

void CounterNode::onMinus(CCObject*) {
	if (value - step >= minValue) {
		value -= step;
		updateLabel();
		onChangeCallback(value);
	}
}

void CounterNode::updateLabel() {
	valueLabel->setString(std::to_string(value).c_str());
}

CounterNode* CounterNode::create(int defaultValue, int step, int minValue, int maxValue, std::function<void(int)> onChange) {
	auto ret = new CounterNode();
	ret->value = defaultValue;
	ret->step = step;
	ret->minValue = minValue;
	ret->maxValue = maxValue;
	ret->onChangeCallback = onChange;
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}