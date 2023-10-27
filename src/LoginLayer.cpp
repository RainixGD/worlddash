#include "LoginLayer.h"
#include <Geode/utils/web.hpp>
#include "DataManager.h"
#include "ReqManager.h"

using namespace geode::prelude;


bool LoginLayer::init() {
	if (!GJDropDownLayer::init("login", 200)) {
		return false;
	}

	this->m_listLayer->setColor(_ccColor3B(153, 85, 51));
	this->m_listLayer->setOpacity(255);

	auto buttons = CCMenu::create();
	buttons->setPosition(0, 0);
	buttons->setContentSize(this->m_listLayer->getContentSize());
	this->m_listLayer->addChild(buttons);

	loginInput = InputNode::create(250, "login");
	loginInput->getInput()->setDelegate(this);
	loginInput->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 + 50);
	buttons->addChild(loginInput);

	passwordInput = InputNode::create(250, "password");
	passwordInput->getInput()->setDelegate(this);
	passwordInput->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2);
	buttons->addChild(passwordInput);

	loginBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("login", 250, 1, "bigFont.fnt", "GJ_button_01.png", 40, 1), this, menu_selector(LoginLayer::onTryLogin));
	loginBtn->setPosition(buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 - 50);
	buttons->addChild(loginBtn);

	loadingCircle = MyLoadingCircle::create();
	loadingCircle->setPosition({ buttons->getContentSize().width / 2, buttons->getContentSize().height / 2 });
	buttons->addChild(loadingCircle);
	loadingCircle->setVisible(0);

	return true;
}

void LoginLayer::onTryLogin(CCObject*) {
	loadingCircle->setVisible(1);
	loginInput->setVisible(0);
	passwordInput->setVisible(0);
	loginBtn->setVisible(0);

	auto req = web::AsyncWebRequest();
	req.postRequest();


	auto message = json::Value();
	message.set("username", json::Value(loginInput->getString()));
	message.set("password", json::Value(passwordInput->getString()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "login", message,
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

void LoginLayer::onResponse(std::string const& response) {
	auto json = json::parse(response);

	if (json["ok"].as_bool()) {
		this->exitLayer(nullptr);
		((GJDropDownLayer*)(this->getParent()))->exitLayer(nullptr);
		FLAlertLayer::create("GG!", "You are <cg>logged in</c>!", "ok")->show();
		DataManager::getInstance()->setUsername(loginInput->getString());
		DataManager::getInstance()->setPassword(passwordInput->getString());
	}
	else {
		loadingCircle->setVisible(0);
		loginInput->setVisible(1);
		passwordInput->setVisible(1);
		loginBtn->setVisible(1);
		FLAlertLayer::create("Error!", "Wrong username or password!", "ok")->show();
	}
}
void LoginLayer::onError(std::string const& error) {
	loadingCircle->setVisible(0);
	loginInput->setVisible(1);
	passwordInput->setVisible(1);
	loginBtn->setVisible(1);
	FLAlertLayer::create("Login Error", error, "ok")->show();
}

void LoginLayer::textChanged(CCTextInputNode* obj) {
	log::info("{}", obj->getString());
}

void LoginLayer::exitLayer(CCObject* obj) {
	GJDropDownLayer::exitLayer(obj);
	ReqManager::getInstance()->cancelReqById(reqId);
}


LoginLayer* LoginLayer::create() {
	auto ret = new LoginLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}