#pragma once
#include "./PublicMapLevelPopup.h"

using namespace geode::prelude;

class PublicLevelAlert : public FLAlertLayer {
	PublicMapLevelPopup* popup;

	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
public:
	int reqId;
	static PublicLevelAlert* create(PublicMapLevelPopup*);
};