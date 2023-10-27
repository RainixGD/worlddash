#pragma once
#include "MyLoadingCircle.h"

using namespace geode::prelude;

class LoginLayer : public GJDropDownLayer, public TextInputDelegate {
private:
	InputNode* loginInput;
	InputNode* passwordInput;
	CCMenuItemSpriteExtra* loginBtn;
	MyLoadingCircle* loadingCircle;
	int reqId;
	virtual bool init();

	void onTryLogin(CCObject*);
	void onResponse(std::string const& response);
	void onError(std::string const& error);
	void textChanged(CCTextInputNode* obj) override;
	void exitLayer(CCObject* obj) override;

public:
	static LoginLayer* create();
};