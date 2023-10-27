#pragma once

using namespace geode::prelude;

class LikeLevelAlert : public FLAlertLayer {
	virtual bool init(int levelId);
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
public:
	int reqId;
	static LikeLevelAlert* create(int levelId);
};