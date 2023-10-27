#pragma once

using namespace geode::prelude;

class FlyingMonster : public CCMenu {
	virtual bool init(bool isSleeping, bool hideGlow);
	AnimatedGameObject* animatedGameObject;
	void hideGlow();
public:
	static FlyingMonster* create(bool isSleeping, bool hideGlow);
};