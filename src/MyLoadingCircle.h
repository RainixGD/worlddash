#pragma once
using namespace geode::prelude;

class MyLoadingCircle : public CCSprite {
	virtual bool init();
public:
	static MyLoadingCircle* create();
};