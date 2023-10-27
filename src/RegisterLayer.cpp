#include "RegisterLayer.h"
#include <Geode/utils/web.hpp>
#include "ReqManager.h"
#include <functional>

using namespace geode::prelude;


bool RegisterLayer::init() {
	if (!GJDropDownLayer::init("register", 240)) {
		return false;
	}

	this->m_listLayer->setColor(_ccColor3B(153, 85, 51));
	this->m_listLayer->setOpacity(255);

	auto buttons = CCMenu::create();
	buttons->setPosition(0, 0);
	buttons->setContentSize(this->m_listLayer->getContentSize());
	this->m_listLayer->addChild(buttons);

	loginInput = InputNode::create(250, "username");
	loginInput->getInput()->setDelegate(this);
	loginInput->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 + 75);
	buttons->addChild(loginInput);

	passwordInput = InputNode::create(250, "password");
	passwordInput->getInput()->setDelegate(this);
	passwordInput->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 + 25);
	buttons->addChild(passwordInput);

	emailInput = InputNode::create(250, "email");
	emailInput->getInput()->setDelegate(this);
	emailInput->getInput()->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-+.@!");
	emailInput->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 - 25);
	buttons->addChild(emailInput);

	loginBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("register", 250, 1, "bigFont.fnt", "GJ_button_06.png", 40, 1), this, menu_selector(RegisterLayer::onTryRegister));
	loginBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 - 75);
	buttons->addChild(loginBtn);

	loadingCircle = MyLoadingCircle::create();
	loadingCircle->setPosition({ buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 });
	buttons->addChild(loadingCircle);
	loadingCircle->setVisible(0);

	return true;
}


void RegisterLayer::onTryRegister(CCObject*) {
	loadingCircle->setVisible(1);
	loginInput->setVisible(0);
	passwordInput->setVisible(0);
	emailInput->setVisible(0);
	loginBtn->setVisible(0);

	auto message = json::Value();
	message.set("username", json::Value(loginInput->getString()));
	message.set("password", json::Value(passwordInput->getString()));
	message.set("email", json::Value(emailInput->getString()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "register", message,
		[this](std::string response) {
			log::info("ok");
			this->onResponse(response);
		},
		[this](std::string error) {
			log::info("error");
			this->onError(error);
		}
	);
}

void RegisterLayer::onResponse(std::string const& response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		this->exitLayer(nullptr);
		FLAlertLayer::create("GG!", "You are <cg>registered</c>!\n Now check your <cr>email</c> and verifie your account!", "ok")->show();
	}
	else {
		FLAlertLayer::create("Error!", json["message"].as_string(), "ok")->show();
		loadingCircle->setVisible(0);
		loginInput->setVisible(1);
		passwordInput->setVisible(1);
		emailInput->setVisible(1);
		loginBtn->setVisible(1);
	}
}
void RegisterLayer::onError(std::string const& error) {
	loadingCircle->setVisible(0);
	loginInput->setVisible(1);
	passwordInput->setVisible(1);
	emailInput->setVisible(1);
	loginBtn->setVisible(1);
	FLAlertLayer::create("Register Error", error, "ok")->show();
}

void RegisterLayer::textChanged(CCTextInputNode* obj) {
	log::info("{}", obj->getString());
}

void RegisterLayer::exitLayer(CCObject* obj) {
	GJDropDownLayer::exitLayer(obj);
	ReqManager::getInstance()->cancelReqById(reqId);
}


RegisterLayer* RegisterLayer::create() {
	auto ret = new RegisterLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}