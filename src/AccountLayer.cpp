#include "./AccountLayer.h"
#include "./LoginLayer.h"
#include "./RegisterLayer.h"
#include "./DataManager.h"
#include "./SaveingAlert.h"
#include "./LoadingAlert.h"

using namespace geode::prelude;

bool AccountLayer::init() {
	if (!GJDropDownLayer::init("account", 200)) {
		return false;
	}
	auto size = CCDirector::sharedDirector()->getWinSize();


	auto buttons = CCMenu::create();
	buttons->setPosition(0, 0);
	buttons->setContentSize(this->m_listLayer->getContentSize());
	this->m_listLayer->addChild(buttons);

	if (DataManager::getInstance()->getUsername() == "") {
		auto loginBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("login", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(AccountLayer::onLogin));
		loginBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 + 50);
		buttons->addChild(loginBtn);

		auto registerBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("register", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(AccountLayer::onRegister));
		registerBtn->setPosition(buttons->getContentSize() / 2);
		buttons->addChild(registerBtn);
	}
	else {
		auto saveBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("save", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(AccountLayer::onSave));
		saveBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 + 50);
		buttons->addChild(saveBtn);

		auto loadBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("load", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(AccountLayer::onLoad));
		loadBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2);
		buttons->addChild(loadBtn);

		auto logoutBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("logout", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(AccountLayer::onLogout));
		logoutBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 - 50);
		buttons->addChild(logoutBtn);
	}

	return true;
}

void AccountLayer::onLogin(CCObject*) {
	auto loginLayer = LoginLayer::create();
	this->addChild(loginLayer);
	loginLayer->showLayer(0);
}

void AccountLayer::onRegister(CCObject*) {
	auto registerLayer = RegisterLayer::create();
	this->addChild(registerLayer);
	registerLayer->showLayer(0);
}

void AccountLayer::onSave(CCObject*) {
	createQuickPopup(
		"save system",
		"Do you want to save your progress?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				SaveingAlert::create()->show();
			}
		}
	);
}

void AccountLayer::onLoad(CCObject*) {
	createQuickPopup(
		"load system",
		"Do you want to load some data? <cr>Be carefull!</c> It can rewrite your progress on this device.",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				LoadingAlert::create()->show();
			}
		}
	);
}

void AccountLayer::onLogout(CCObject*) {
	createQuickPopup(
		"!!!",
		"Do you really want to quit your <cr>account</c>?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				DataManager::getInstance()->setUsername("");
				DataManager::getInstance()->setPassword("");
				DataManager::getInstance()->deleteLevelFolder();
				this->exitLayer(nullptr);
			}
		}
	);
}

AccountLayer* AccountLayer::create() {
	auto ret = new AccountLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}