#pragma once

using namespace geode::prelude;

class LoadingAlert : public FLAlertLayer {
	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
public:
	int reqId;
	static LoadingAlert* create();
};