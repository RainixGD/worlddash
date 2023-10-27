#pragma once
#include "./PublicLevelOptionsPopup.h"

using namespace geode::prelude;

class UpdateLevelAlert : public FLAlertLayer {
	PublicLevelOptionsPopup* popup;

	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
public:
	int reqId;
	static UpdateLevelAlert* create(PublicLevelOptionsPopup*);
};
