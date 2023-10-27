#pragma once

using namespace geode::prelude;

class FrameAnimatedSprite : public CCSprite {
	virtual bool init(std::string textureNameWithoutFrameCount, int frameCount, float delayBetweenFrames, float delayBetweenRepeats, bool isDoubleTexture);
	CCSprite* secondLayer = nullptr;
public:
	static FrameAnimatedSprite* create(std::string textureNameWithoutFrameCount, int frameCount, float delayBetweenFrames, float delayBetweenRepeats, bool isDoubleTexture);
	void setSecondColor(ccColor3B color);
};