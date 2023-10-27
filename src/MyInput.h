#pragma once
#include <functional>

using namespace geode::prelude;

class MyInput : public TextInputDelegate, public CCMenu {
private:
	InputNode* input;
	std::function<void(std::string const&)> callback;

	virtual bool init(int width, std::string placeholder, std::string value, std::function<void(std::string const&)> changeCallback);
	void textChanged(CCTextInputNode* obj) override;
public:
	InputNode* getInput();
	static MyInput* create(int width, std::string placeholder, std::string value, std::function<void(std::string const&)> changeCallback);
};