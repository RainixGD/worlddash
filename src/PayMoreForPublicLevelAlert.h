#pragma once
#include "PayMoreForPublicLevelAlert.h"
#include "PublicLevelOptionsPopup.h";

using namespace geode::prelude;

class PayMoreForPublicLevelAlert : public FLAlertLayer {
	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
	PublicLevelOptionsPopup* popup;
public:
	int reqId;
	static PayMoreForPublicLevelAlert* create(PublicLevelOptionsPopup* popup);
};