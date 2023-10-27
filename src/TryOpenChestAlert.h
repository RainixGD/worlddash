#pragma once
#include <functional>

using namespace geode::prelude;

class TryOpenChestAlert: public FLAlertLayer {
	virtual bool init();
	void keyBackClicked() override;
	void onResponse(std::string);
	void onError(std::string);
	int chestTag;
	std::function<void()> hideChest;
public:
	int reqId;
	static TryOpenChestAlert* create(int chestTag, std::function<void()> hideChest);
};