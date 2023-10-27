#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

struct FrameAnimatedObject {
	CCPoint position;
	ccColor3B firstColor;
	ccColor3B secondColor;
		
	std::string textureName;
	int frameCount;
	float minDelay;
	float maxDelay;
	float delayBetweenFrames;
	bool isSecondSprite;
};

struct MonsterInfoObject {
	int type;
	CCPoint position;
	float scale;
	int rotation;
	bool isFlipX;
};

class MapDecoContainer : public CCMenu {
	virtual bool init();
	
	static std::vector<FrameAnimatedObject> frameAnimatedObjectsList;
	static std::vector<MonsterInfoObject> monstersList;
public:
	static MapDecoContainer* create();
};