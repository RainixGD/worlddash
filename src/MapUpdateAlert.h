#pragma once
#include "MapLayer.h"

using namespace geode::prelude;

class MapUpdateAlert : public FLAlertLayer {
	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
	MapLayer* mapLayer;
public:
	int reqId;
	static MapUpdateAlert* create(MapLayer* mapUiLayer);
};