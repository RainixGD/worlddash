#pragma once
#include "MyLoadingCircle.h"
#include <Geode/utils/web.hpp>
using namespace geode::prelude;



class RegisterLayer : public GJDropDownLayer, public TextInputDelegate {
private:
	InputNode* loginInput;
	InputNode* passwordInput;
	InputNode* emailInput;
	CCMenuItemSpriteExtra* loginBtn;
	MyLoadingCircle* loadingCircle;
	int reqId;
	virtual bool init();

	void onTryRegister(CCObject*);
	void onResponse(std::string const& response);
	void onError(std::string const& error);
	void textChanged(CCTextInputNode* obj) override;
	void exitLayer(CCObject* obj) override;
public:
	static RegisterLayer* create();
};