#pragma once

using namespace geode::prelude;

class AccountLayer : public GJDropDownLayer {
	virtual bool init();
	void onLogin(CCObject*);
	void onRegister(CCObject*);
	void onSave(CCObject*);
	void onLoad(CCObject*);
	void onLogout(CCObject*);
public:
	static AccountLayer* create();
};