#pragma once
#include "./MyLoadingCircle.h"
#include "./Button.h"

using namespace geode::prelude;

class LevelPopup : public geode::Popup<>, public MusicDownloadDelegate {
private:
	CCMenuItemSpriteExtra* playBtn;
	MyLoadingCircle* levelLoadingCircle;
	json::Value lvlJson;
	std::function<void()> updateMapAndCoinsCallback;

	Button* likeBtn;
	Button* rateDifficultyBtn;
	Button* likeBtnCanceled;
	Button* rateDifficultyBtnCanceled;
	Button* optionsBtn;
protected:
	bool setup() override;
	void loadLevel();
	void onResponse(std::string const& response);
	void onError(std::string const& error);
	void onPlay(CCObject*);
	virtual void onClose(CCObject* obj) override;

	void onLikeClicked(CCObject*);
	void onRateDifficultyClicked(CCObject*);
	void onOptionsClicked(CCObject*);

public:
	static LevelPopup* create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback);
	LevelPopup();
private:
	std::string levelString;
	int reqId;
};