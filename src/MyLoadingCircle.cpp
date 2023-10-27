#include "./MyLoadingCircle.h"

using namespace geode::prelude;


bool MyLoadingCircle::init() {
	if (!CCSprite::initWithFile("loadingCircle.png")) {
		return false;
	}

	this->runAction(CCRepeatForever::create(CCRotateBy::create(0.5, 360)));
	this->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

	return true;
}

MyLoadingCircle* MyLoadingCircle::create() {
	auto ret = new MyLoadingCircle();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}