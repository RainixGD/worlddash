#pragma once

using namespace geode::prelude;

class EatingMonster : public CCMenu {
	virtual bool init(bool isSleeping, bool hideGlow);
	AnimatedGameObject* animatedGameObject;
	void hideGlow();
public:
	static EatingMonster* create(bool isSleeping, bool hideGlow);
};