using namespace geode::prelude;
#include "MapDecoContainer.h"
#include "FrameAnimatedSprite.h"
#include "JumpingMonster.h"
#include "EatingMonster.h"
#include "FlyingMonster.h"
#include <random>

float getRandomFloat(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

bool MapDecoContainer::init() {
	if (!CCMenu::init()) return false;
	
	for (FrameAnimatedObject frameAnimatedObject : frameAnimatedObjectsList) {
		auto test = FrameAnimatedSprite::create(frameAnimatedObject.textureName, frameAnimatedObject.frameCount, frameAnimatedObject.delayBetweenFrames, getRandomFloat(frameAnimatedObject.minDelay, frameAnimatedObject.maxDelay), frameAnimatedObject.isSecondSprite);
		test->setColor(frameAnimatedObject.firstColor);
		test->setSecondColor(frameAnimatedObject.secondColor);
		test->setPosition(frameAnimatedObject.position);
		this->addChild(test);
	}

	for (MonsterInfoObject monsterInfo : monstersList) {
		CCMenu* monster;
		switch (monsterInfo.type) {
		case 1:
			monster = EatingMonster::create(0, 1);
			break;
		case 2:
			monster = EatingMonster::create(1, 1);
			break;
		case 3:
			monster = JumpingMonster::create();
			break;
		case 4:
			monster = FlyingMonster::create(0, 1);
			break;
		case 5:
			monster = FlyingMonster::create(1, 1);
			break;
		default:
			monster = EatingMonster::create(0, 1);
			break;
		}

		monster->setPosition(monsterInfo.position);
		monster->setScaleX(monsterInfo.scale * (monsterInfo.isFlipX ? -1 : 1 ));
		monster->setScaleY(monsterInfo.scale);
		monster->setRotation(monsterInfo.rotation);
		this->addChild(monster);
	}

	return true;
}

MapDecoContainer* MapDecoContainer::create() {
	auto ret = new MapDecoContainer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}


std::vector<MonsterInfoObject> MapDecoContainer::monstersList = {
	MonsterInfoObject(1, ccp(809, 436), 0.225, -38, 1),
	MonsterInfoObject(1, ccp(395, 146), 0.2, 20, 0),
	MonsterInfoObject(1, ccp(256, 578), 0.2, 0, 1),
	MonsterInfoObject(1, ccp(345, 1096), 0.275, 27, 0),
	MonsterInfoObject(1, ccp(1506, 1239), 0.5, -22, 0),

	MonsterInfoObject(2, ccp(1127, 855), 0.25, 15, 0),
	MonsterInfoObject(2, ccp(1167, 1070), 0.275, 18, 0),
	MonsterInfoObject(2, ccp(1137, 952), 0.25, -16, 1),
	MonsterInfoObject(2, ccp(1227, 898), 0.2, 18, 0),
	MonsterInfoObject(2, ccp(1219, 1046), 0.375, -17, 1),

	MonsterInfoObject(3, ccp(672, 354), 0.3, 0, 1),
	MonsterInfoObject(3, ccp(461, 970), 0.275, 0, 1),
	MonsterInfoObject(3, ccp(988, 570), 0.35, 0, 0),
	MonsterInfoObject(3, ccp(849, 1123), 0.375, 0, 1),
	MonsterInfoObject(3, ccp(1019, 210), 0.325, 0, 0),
	MonsterInfoObject(3, ccp(303, 400), 0.25, 0, 1),
	MonsterInfoObject(3, ccp(1403, 986), 0.375, 0, 0),
	MonsterInfoObject(3, ccp(395, 1235), 0.25, 0, 1),
	MonsterInfoObject(3, ccp(106, 742), 0.2, 0, 0),

	MonsterInfoObject(4, ccp(449, 681), 0.275, 0, 0),
	MonsterInfoObject(4, ccp(362, 670), 0.5, 0, 1),
	MonsterInfoObject(4, ccp(723, 188), 0.425, 0, 0),
	MonsterInfoObject(4, ccp(37, 899), 0.8, 0, 1),
	MonsterInfoObject(4, ccp(703, 715), 0.5, 0, 1),
	MonsterInfoObject(4, ccp(1328, 293), 0.425, 0, 0),

	MonsterInfoObject(5, ccp(567, 804), 0.5, 0, 0),
	MonsterInfoObject(5, ccp(567, 1053), 0.5, 0, 0),
	MonsterInfoObject(5, ccp(920, 559), 0.5, 0, 1),
	MonsterInfoObject(5, ccp(1232, 728), 0.5, 0, 0),
	MonsterInfoObject(5, ccp(858, 773), 0.5, 0, 1),
};


std::vector<FrameAnimatedObject> MapDecoContainer::frameAnimatedObjectsList = {
	FrameAnimatedObject(ccp(382, 349), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(447, 396), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(268, 540), ccc3(129, 191, 246), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(71, 496), ccc3(129, 191, 246), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(524, 139), ccc3(84, 172, 225), ccc3(216, 243, 225), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(260, 1128), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1048, 258), ccc3(84, 172, 225), ccc3(216, 243, 225), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(470, 606), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1302, 516), ccc3(84, 172, 225), ccc3(216, 243, 225), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(756, 477), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(832, 862), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops01", 8, 1, 5, 0.1, 1),

	FrameAnimatedObject(ccp(398, 646), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(191, 1049), ccc3(117, 231, 225), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(256, 921), ccc3(98, 180, 225), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(198, 952), ccc3(16, 112, 182), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(100, 907),  ccc3(16, 112, 182), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(110, 1008), ccc3(84, 172, 225), ccc3(216, 243, 225), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(470, 1089), ccc3(16, 112, 182), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(596, 1180), ccc3(16, 112, 182), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(802, 159), ccc3(98, 180, 225), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1189, 374), ccc3(98, 180, 225), ccc3(255, 255, 255), "gj_bubble01", 15, 1, 5, 0.1, 1),

	FrameAnimatedObject(ccp(1027, 74), ccc3(109, 194, 255), ccc3(220, 252, 255), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1343, 59), ccc3(5, 75, 121), ccc3(50, 121, 134), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1482, 253), ccc3(5, 75, 121), ccc3(50, 121, 134), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1486, 497), ccc3(38, 118, 159), ccc3(128, 180, 240), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(739, 580), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(792, 923), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(961, 1126), ccc3(38, 118, 159), ccc3(128, 180, 240), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1228, 1254), ccc3(5, 75, 121), ccc3(50, 121, 134), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1419, 1219), ccc3(5, 75, 121), ccc3(50, 121, 134), "gj_drops02", 8, 1, 5, 0.1, 1),
	FrameAnimatedObject(ccp(1487, 1132), ccc3(5, 75, 121), ccc3(50, 121, 134), "gj_drops02", 8, 1, 5, 0.1, 1),

	FrameAnimatedObject(ccp(690, 763), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_smoke02", 5, 0, 0, 0.1, 1),
	FrameAnimatedObject(ccp(732, 671), ccc3(169, 211, 238), ccc3(255, 255, 255), "gj_smoke02", 5, 0, 0, 0.1, 1),
};