#include "./MyInput.h"

using namespace geode::prelude;

bool MyInput::init(int width, std::string placeholder, std::string value, std::function<void(std::string const&)> changeCallback) {
	if (!CCMenu::init()) return false;
	callback = changeCallback;

	input = InputNode::create(width, placeholder.c_str());
	input->setString(value);
	input->getInput()->setDelegate(this);
	this->addChild(input);

	return 1;
}

void MyInput::textChanged(CCTextInputNode* obj) {
	TextInputDelegate::textChanged(obj);
	callback(obj->getString());
}

InputNode* MyInput::getInput() {
	return input;
}

MyInput* MyInput::create(int width, std::string placeholder, std::string value, std::function<void(std::string const&)> changeCallback) {
	auto ret = new MyInput();
	if (ret && ret->init(width, placeholder, value, changeCallback)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}