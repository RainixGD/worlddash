#pragma once

using namespace geode::prelude;

class SaveingAlert : public FLAlertLayer {
	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
public:
	int reqId;
	static SaveingAlert* create();
};