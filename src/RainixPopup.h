#pragma once
#include "./Button.h"

using namespace geode::prelude;

class RainixPopup : public geode::Popup<> {
protected:
	bool setup() override;
	static std::vector<std::string> strings;
public:
	static RainixPopup* create();
};