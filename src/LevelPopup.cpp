#include "./LevelPopup.h"
#include "./ReqManager.h"
#include "./Button.h"
#include "./DataManager.h"
#include "./LikeLevelAlert.h"
#include "./SelectLevelDifficultyAlert.h"
#include "./PublicLevelOptionsPopup.h"
#include <Geode/utils/web.hpp>
using namespace geode::prelude;

const char* getTextureNameByDifficultyId(int id) {
	switch (id) {
	case 0:
		return "difficulty_auto_btn_001.png";
	case 1:
		return "difficulty_01_btn_001.png";
	case 2:
		return "difficulty_02_btn_001.png";
	case 3:
		return "difficulty_03_btn_001.png";
	case 4:
		return "difficulty_04_btn_001.png";
	case 5:
		return "difficulty_05_btn_001.png";
	case 6:
		return "difficulty_07_btn2_001.png";
	case 7:
		return "difficulty_08_btn2_001.png";
	case 8:
		return "difficulty_06_btn2_001.png";
	case 9:
		return "difficulty_09_btn2_001.png";
	case 10:
		return "difficulty_10_btn2_001.png";
	default:
		return "difficulty_00_btn_001.png";
	}
}

bool LevelPopup::setup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	this->setTitle("");

	auto nameLabel = CCLabelBMFont::create(lvlJson["name"].as_string().c_str(), "bigFont.fnt");
	nameLabel->setPositionY(m_size.height / 2.3);
	m_buttonMenu->addChild(nameLabel, 2);
	nameLabel->limitLabelWidth(m_size.width, .7f, .1f);

	auto authorLabel = CCLabelBMFont::create(("by " + lvlJson["creatorName"].as_string()).c_str(), "goldFont.fnt");
	auto authorBtn = CCMenuItemSpriteExtra::create(authorLabel, this, nullptr);
	authorBtn->setPositionY(m_size.height / 2.3 - 20);
	m_buttonMenu->addChild(authorBtn, 2);
	authorLabel->limitLabelWidth(m_size.width - 20.f, .7f, .1f);

	m_closeBtn->setZOrder(5);

	auto corner1 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner1->setPosition({ m_size.width / 2 - corner1->getContentSize().width / 2, m_size.height / 2 - corner1->getContentSize().height / 2 });
	corner1->setRotation(180);
	m_buttonMenu->addChild(corner1);

	auto corner2 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner2->setPosition({ -m_size.width / 2 + corner2->getContentSize().width / 2, m_size.height / 2 - corner2->getContentSize().height / 2 });
	corner2->setRotation(90);
	m_buttonMenu->addChild(corner2);

	auto corner3 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner3->setPosition({ m_size.width / 2 - corner3->getContentSize().width / 2, -m_size.height / 2 + corner3->getContentSize().height / 2 });
	corner3->setRotation(270);
	m_buttonMenu->addChild(corner3);

	auto corner4 = CCSprite::createWithSpriteFrameName("dailyLevelCorner_001.png");
	corner4->setPosition({ -m_size.width / 2 + corner4->getContentSize().width / 2, -m_size.height / 2 + corner4->getContentSize().height / 2 });
	m_buttonMenu->addChild(corner4);


	playBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"), this, menu_selector(LevelPopup::onPlay));
	playBtn->setPositionY(20);
	m_buttonMenu->addChild(playBtn);
	playBtn->setVisible(0);

	levelLoadingCircle = MyLoadingCircle::create();
	levelLoadingCircle->setPositionY(20);
	this->m_buttonMenu->addChild(levelLoadingCircle);

	auto difficultySprite = CCSprite::createWithSpriteFrameName(getTextureNameByDifficultyId(lvlJson["difficulty"].as_int()));
	difficultySprite->setPosition({ -m_size.width / 4, 20 });
	m_buttonMenu->addChild(difficultySprite);
	
	if (!lvlJson["isLocalSong"].as_int()) {
		//MusicDownloadManager::sharedState()->getSongInfo(lvlJson["songId"].as_int(), 0);
		auto songInfoObject = SongInfoObject::create();
		songInfoObject->m_songID = lvlJson["songId"].as_int();
		auto songNode = CustomSongWidget::create(songInfoObject/*MusicDownloadManager::sharedState()->getSongInfoObject(lvlJson["songId"].as_int())*/, LevelSettingsObject::create(), 0, 0, 1, 0, 1);
		songNode->onGetSongInfo(nullptr);
		songNode->setPositionY(-80);
		m_buttonMenu->addChild(songNode);
	}



	auto downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_sDownloadIcon_001.png");
	downloadsIcon->setPosition({ m_size.width / 5, 60 });
	m_buttonMenu->addChild(downloadsIcon);

	auto likesIcon = CCSprite::createWithSpriteFrameName("GJ_sLikeIcon_001.png");
	likesIcon->setPosition({ m_size.width / 5, 40 });
	m_buttonMenu->addChild(likesIcon);
	
	auto downloadsLabel = CCLabelBMFont::create(std::to_string(lvlJson["downloads"].as_int()).c_str(), "bigFont.fnt");
	downloadsLabel->setPosition({ m_size.width / 5 + 10, 60 });
	downloadsLabel->setAnchorPoint({ 0, 0.5 });
	downloadsLabel->limitLabelWidth(80, .5f, .1f);
	m_buttonMenu->addChild(downloadsLabel);

	auto likesLabel = CCLabelBMFont::create(std::to_string(lvlJson["likes"].as_int()).c_str(), "bigFont.fnt");
	likesLabel->setPosition({ m_size.width / 5 + 10, 40 });
	likesLabel->setAnchorPoint({ 0, 0.5 });
	likesLabel->limitLabelWidth(80, .5f, .1f);
	m_buttonMenu->addChild(likesLabel);

	likeBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), this, menu_selector(LevelPopup::onLikeClicked));
	likeBtn->setPosition({ m_size.width / 2, 0 });
	m_buttonMenu->addChild(likeBtn);

	rateDifficultyBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_rateDiffBtn_001.png"), this, menu_selector(LevelPopup::onRateDifficultyClicked));
	rateDifficultyBtn->setPosition({ m_size.width / 2, -60 });
	m_buttonMenu->addChild(rateDifficultyBtn);

	likeBtnCanceled = Button::create(CCSprite::createWithSpriteFrameName("GJ_likeBtn2_001.png"), this, nullptr);
	likeBtnCanceled->setPosition({ m_size.width / 2, 0 });
	m_buttonMenu->addChild(likeBtnCanceled);

	rateDifficultyBtnCanceled = Button::create(CCSprite::createWithSpriteFrameName("GJ_rateDiffBtn2_001.png"), this, nullptr);
	rateDifficultyBtnCanceled->setPosition({ m_size.width / 2, -60 });
	m_buttonMenu->addChild(rateDifficultyBtnCanceled);

	optionsBtn = Button::create(CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png"), this, menu_selector(LevelPopup::onOptionsClicked));
	optionsBtn->setPosition({ -m_size.width / 2, 60 });
	m_buttonMenu->addChild(optionsBtn);

	switch (lvlJson["likeBtn"].as_int()) {
	case 1:
		likeBtn->setVisible(1);
		likeBtnCanceled->setVisible(0);
		break;
	case 0:
		likeBtn->setVisible(0);
		likeBtnCanceled->setVisible(1);
		break;
	default:
		likeBtn->setVisible(0);
		likeBtnCanceled->setVisible(0);
		break;
	}

	switch (lvlJson["difficultyBtn"].as_int()) {
	case 1:
		rateDifficultyBtn->setVisible(1);
		rateDifficultyBtnCanceled->setVisible(0);
		break;
	case 0:
		rateDifficultyBtn->setVisible(0);
		rateDifficultyBtnCanceled->setVisible(1);
		break;
	default:
		rateDifficultyBtn->setVisible(0);
		rateDifficultyBtnCanceled->setVisible(0);
		break;
	}

	switch (lvlJson["optionsBtn"].as_int()) {
	case 1:
		optionsBtn->setVisible(1);
		break;
	default:
		optionsBtn->setVisible(0);
		break;
	}

	loadLevel();
	return true;
}

void LevelPopup::onOptionsClicked(CCObject*) {
	PublicLevelOptionsPopup::create(lvlJson, updateMapAndCoinsCallback, [this]() {this->onClose(nullptr); })->show();
}

void LevelPopup::onLikeClicked(CCObject*) {
	createQuickPopup(
		"Like system",
		"Do you want to rate this level?",
		"no", "yes",
		[this](auto self, bool btn2) {
			if (btn2) {
				LikeLevelAlert::create(lvlJson["id"].as_int())->show();
			}
		}
	);
}

void LevelPopup::onRateDifficultyClicked(CCObject*) {
	SelectLevelDifficultyAlert::create(lvlJson["id"].as_int())->show();
}

void LevelPopup::loadLevel() {
	auto message = json::Value();
	message.set("id", json::Value(lvlJson["id"].as_int()));
	message.set("username", json::Value(DataManager::getInstance()->getUsername()));
	message.set("password", json::Value(DataManager::getInstance()->getPassword()));

	reqId = ReqManager::getInstance()->makeReq(ReqManager::serverLink + "getLevelStringById", message,
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

void LevelPopup::onResponse(std::string const& response) {
	auto str = json::Value::from_str(response);
	auto test = str["levelstring"];
	levelString = test.as_string();

	levelLoadingCircle->setVisible(0);
	playBtn->setVisible(1);
}

void LevelPopup::onError(std::string const& error) {
	levelLoadingCircle->setVisible(0);
	FLAlertLayer::create("Loading Error", error, "ok")->show();
}

void LevelPopup::onPlay(CCObject*) {
	auto level = GJGameLevel::create();
	level->m_levelID = lvlJson["id"].as_int();
	level->m_levelName = lvlJson["name"].as_string();
	level->m_levelString = levelString;
	if (lvlJson["isLocalSong"].as_int())
		level->m_audioTrack = lvlJson["songId"].as_int();
	else
		level->m_songID = lvlJson["songId"].as_int();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.3, PlayLayer::scene(level)));
}

void LevelPopup::onClose(CCObject* obj) {
	ReqManager::getInstance()->cancelReqById(reqId);
	this->setKeyboardEnabled(false);
	this->removeFromParentAndCleanup(true);
}


LevelPopup* LevelPopup::create(json::Value lvlJson, std::function<void()> updateMapAndCoinsCallback) {
	auto ret = new LevelPopup();
	ret->lvlJson = lvlJson;
	ret->updateMapAndCoinsCallback = updateMapAndCoinsCallback;
	if (ret && ret->init(350.f, 270.f)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

LevelPopup::LevelPopup(){}