#pragma once
#include "./MapLayer.h"
#include "./Button.h"
#include "./MyLoadingCircle.h"

using namespace geode::prelude;

class MapUiLayer : public CCLayer {
	Button* publicLevelBtn;
	CCScale9Sprite* coinsAlert;
	CCLabelBMFont* coinsLabel;
	MyLoadingCircle* coinsLoadingCircle;

	bool firstLayerEnter = 1;

	int getCoinsReqId;

	~MapUiLayer();
protected:
	virtual bool init();
	void backToMainScene(CCObject*);
	void keyBackClicked() override;
	void onTryPublic(CCObject*);
	void onUpdateMap(CCObject*);

	void onEnterTransitionDidFinish() override {
		CCLayer::onEnterTransitionDidFinish();
		mapLayer->updateMapAndCoins();
	}

	void onGetCoinsResponse(std::string const&);
	void onGetCoinsError(std::string const&);
public:
	CCParticleSystemQuad* snow;
	void updateCoins();
	MapLayer* mapLayer;
	CCScene* thisScene;
	static MapUiLayer* create(CCScene* thisScene);
};