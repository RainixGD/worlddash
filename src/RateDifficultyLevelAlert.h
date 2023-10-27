#pragma once

using namespace geode::prelude;

class RateDifficultyLevelAlert : public FLAlertLayer {
	virtual bool init(int levelId);
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
	int ratedDifficulty;
public:
	int reqId;
	static RateDifficultyLevelAlert* create(int levelId, int ratedDifficulty);
};