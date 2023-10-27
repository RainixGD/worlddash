#pragma once

using namespace geode::prelude;

class JumpingMonster : public CCMenu {
	virtual bool init();
public:
	static JumpingMonster* create();
};