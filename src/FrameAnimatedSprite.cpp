#include "FrameAnimatedSprite.h"

using namespace geode::prelude;

bool FrameAnimatedSprite::init(std::string textureNameWithoutFrameCount, int frameCount, float delayBetweenFrames, float delayBetweenRepeats, bool isDoubleTexture) {
	if (!CCSprite::init()) return false;
	auto framesArray = CCArray::create();
	for (int i = 1; i <= frameCount; i++) {
		std::string spriteNameMonster = i < 10 ? std::string(textureNameWithoutFrameCount + "_00" + std::to_string(i) + ".png") : std::string(textureNameWithoutFrameCount + "_0" + std::to_string(i) + ".png");
		auto nextFrameMonster = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteNameMonster.c_str());
		framesArray->addObject(nextFrameMonster);
	}
	auto animation = CCAnimation::createWithSpriteFrames(framesArray, delayBetweenFrames);
	auto sequence = CCSequence::createWithTwoActions(CCAnimate::create(animation), CCFadeTo::create(0, 0));
	auto sequence2 = CCSequence::createWithTwoActions(sequence, CCDelayTime::create(delayBetweenRepeats));
	auto sequence3 = CCSequence::createWithTwoActions(sequence2, CCFadeTo::create(0, 255));
	this->setContentSize( static_cast<CCSpriteFrame*>(framesArray->objectAtIndex(0))->getOriginalSize());
	this->runAction(CCRepeatForever::create(sequence3));

	if (isDoubleTexture) {
		secondLayer = CCSprite::create();
		auto framesArray = CCArray::create();
		for (int i = 1; i <= frameCount; i++) {
			std::string spriteNameMonster = i < 10 ? std::string(textureNameWithoutFrameCount + "_color_00" + std::to_string(i) + ".png") : std::string(textureNameWithoutFrameCount + "_color_0" + std::to_string(i) + ".png");
			auto nextFrameMonster = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteNameMonster.c_str());
			framesArray->addObject(nextFrameMonster);
		}
		auto animation = CCAnimation::createWithSpriteFrames(framesArray, delayBetweenFrames);
		auto sequence = CCSequence::createWithTwoActions(CCAnimate::create(animation), CCFadeTo::create(0, 0));
		auto sequence2 = CCSequence::createWithTwoActions(sequence, CCDelayTime::create(delayBetweenRepeats));
		auto sequence3 = CCSequence::createWithTwoActions(sequence2, CCFadeTo::create(0, 255));
		secondLayer->runAction(CCRepeatForever::create(sequence3));
		this->addChild(secondLayer);
		secondLayer->setPosition(this->getContentSize() / 2);
	}

	return true;
}

void FrameAnimatedSprite::setSecondColor(ccColor3B color) {
	if (secondLayer != nullptr)
		secondLayer->setColor(color);
}


FrameAnimatedSprite* FrameAnimatedSprite::create(std::string textureNameWithoutFrameCount, int frameCount, float delayBetweenFrames, float delayBetweenRepeats, bool isDoubleTexture) {
	auto ret = new FrameAnimatedSprite();
	if (ret && ret->init(textureNameWithoutFrameCount, frameCount, delayBetweenFrames, delayBetweenRepeats, isDoubleTexture)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}