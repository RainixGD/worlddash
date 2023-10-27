#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/OptionsLayer.hpp>

#include <Geode/utils/web.hpp>
#include "./Button.h"
#include "./MyLoadingCircle.h"
#include "./MapUiLayer.h"
#include "./LevelPopup.h"
#include "./LevelObj.h"
#include "./MapLayer.h"
#include "./LoginLayer.h"
#include "./AccountLayer.h"
#include "./RegisterLayer.h"
#include "./DataManager.h"
#include "./CreatedLevelsLayer.h"
#include "./MyLevelTools.h"
#include "./DailyLoadingAlert.h"
#include "./MyInput.h"
#include "./RainixPopup.h"

using namespace geode::prelude;

#define STRINGIFY(...) #__VA_ARGS__
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#define GLEW_STATIC

inline void patch(void* loc, std::vector<std::uint8_t> bytes) {
	auto size = bytes.size();
	DWORD old_prot;
	VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &old_prot);
	memcpy(loc, bytes.data(), size);
	VirtualProtect(loc, size, old_prot, &old_prot);
}

class $modify(AppDelegate) {
	void trySaveGame() {
		DataManager::getInstance()->saveLocalAccountFile();
		AppDelegate::trySaveGame();
	}
};

class $modify(GameStatsManager) {
	bool isItemUnlocked(UnlockType type, int id) {
		return true;
	}
};

class $modify(PlayLayer) {
	void onExit() {
		if (DataManager::isMapOpened) {
			CCDirector::sharedDirector()->popSceneWithTransition(0.3, kPopTransitionFade);
			GameSoundManager::sharedManager()->playBackgroundMusic("mapSong.mp3"_spr, 1, 0);
		}
		else {
			PlayLayer::onExit();
			GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
		}
	}
	void onQuit() {
		if (DataManager::isMapOpened) {
			CCDirector::sharedDirector()->popSceneWithTransition(0.3, kPopTransitionFade);
			GameSoundManager::sharedManager()->playBackgroundMusic("mapSong.mp3"_spr, 1, 0);
		}
		else {
			PlayLayer::onQuit();
			GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
		}
	}
};

class $modify(ModPauseLayer, PauseLayer) {
	void keyBackClicked() {
		ModPauseLayer::onQuit(nullptr);
	}
	void onQuit(cocos2d::CCObject * p0) {
		if (DataManager::isMapOpened) {
			CCDirector::sharedDirector()->popSceneWithTransition(0.3, kPopTransitionFade);
			GameSoundManager::sharedManager()->playBackgroundMusic("mapSong.mp3"_spr, 1, 0);
		}
		else {
			PauseLayer::onQuit(p0);
			GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
		}
	}
};

class $modify(ModEndLevelLayer, EndLevelLayer) {
	void keyBackClicked() {
		ModEndLevelLayer::onMenu(nullptr);
	}
	void onMenu(cocos2d::CCObject * sender) {
		if (DataManager::isMapOpened) {
			CCDirector::sharedDirector()->popSceneWithTransition(0.3, kPopTransitionFade);
			GameSoundManager::sharedManager()->playBackgroundMusic("mapSong.mp3"_spr, 1, 0);
		}
		else {
			EndLevelLayer::onMenu(sender);
			GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
		}
	}
};


class $modify(ModEditorPauseLayer, EditorPauseLayer) {
	void saveLevel() {
		this->m_editorLayer->m_level->m_levelString = this->m_editorLayer->getLevelString();
		DataManager::getInstance()->updateLocalLevel(this->m_editorLayer->m_level);
	}
	void onExitEditor(cocos2d::CCObject* sender) {
		EditorPauseLayer::onExitEditor(sender);
		GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
	}
};


class $modify(ModEditLevelLayer, EditLevelLayer) {
	CCSprite* bg;
	CCMenuItemSpriteExtra* confirmLevelNameBtn;
	MyInput* input;
	std::string levelName;
	bool init(GJGameLevel* level) {
		m_fields->levelName = level->m_levelName;
		if (!EditLevelLayer::init(level)) {
			return false;
		}

		auto size = CCDirector::sharedDirector()->getWinSize();

		this->removeAllChildren();

		m_fields->bg = CCSprite::create("game_bg_14_001.png");
		m_fields->bg->setColor(ccColor3B(0, 100, 150));
		m_fields->bg->setPosition({ size.width / 2, size.width / 2 });
		m_fields->bg->setScale(size.width / m_fields->bg->getContentSize().width);
		this->addChild(m_fields->bg);

		auto backBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(ModEditLevelLayer::onBack));
		backBtn->setPosition({ 20, size.height - 25 });
		this->addChild(backBtn);

		auto playBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"), this, menu_selector(EditLevelLayer::onPlay));
		playBtn->setPosition({ size.width / 2 + 100, size.height / 2 });
		this->addChild(playBtn);

		auto editBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png"), this, menu_selector(EditLevelLayer::onEdit));
		editBtn->setPosition({ size.width / 2 - 100, size.height / 2 });
		this->addChild(editBtn);

		m_fields->input = MyInput::create(250, "level name", m_fields->levelName, [this](std::string const& newValue) { this->onNameChanged(newValue); });
		m_fields->input->getInput()->getInput()->m_maxLabelLength = 20;
		m_fields->input->setPosition({ size.width / 2, size.height * 0.8f });
		this->addChild(m_fields->input);

		CCMenu* confirmLevelMenu = CCMenu::create();
		m_fields->confirmLevelNameBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_rewardBtn_001.png"), this, menu_selector(ModEditLevelLayer::applyNameChange));
		confirmLevelMenu->setPosition({ size.width / 2 + 160, size.height * 0.8f });
		this->addChild(confirmLevelMenu);
		confirmLevelMenu->addChild(m_fields->confirmLevelNameBtn);
		m_fields->confirmLevelNameBtn->setVisible(0);

		auto deleteBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png"), this, menu_selector(ModEditLevelLayer::onDelete));
		deleteBtn->setPosition({ size.width - 25, size.height - 25 });
		this->addChild(deleteBtn);

		return true;
	}

	void onDelete(CCObject* obj) {
		createQuickPopup(
			"!!!",
			"Do you really want to delete this level?",
			"no", "yes",
			[this](auto self, bool btn2) {
				if (btn2) {
					DataManager::getInstance()->deleteLocalLevel(this->m_level->m_levelID);
					EditLevelLayer::keyBackClicked();
				}
			}
		);

		
	}

	void onNameChanged(std::string newValue) {
		if ((newValue != "") && (newValue != m_fields->levelName)) {
			m_fields->confirmLevelNameBtn->setVisible(1);
		}
		else {
			m_fields->confirmLevelNameBtn->setVisible(0);
		}
	}

	void applyNameChange(CCObject*) {
		if (m_fields->input->getInput()->getString() != "") {
			this->m_level->m_levelName = m_fields->input->getInput()->getString();
			m_fields->levelName = m_fields->input->getInput()->getString();
			DataManager::getInstance()->updateLocalLevel(this->m_level);//TODO change only level name
			m_fields->confirmLevelNameBtn->setVisible(0);
		}
	}

	void onBack(CCObject*) {
		EditLevelLayer::keyBackClicked();
	}

	void keyBackClicked(void) {
		CreatedLevelsLayer* createdLevelsLayer = CreatedLevelsLayer::create();
		cocos2d::CCScene* scene = cocos2d::CCScene::create();
		scene->addChild(createdLevelsLayer);
		cocos2d::CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, scene));
	}
};


class WavyNode : public CCNode {
	CCRenderTexture* rt;
	CCSprite* s;
	GLuint m_uniformResolution;

	std::string fragmentShader = STRINGIFY(
		uniform vec2 resolution;
	float time = CC_Time.y;

	uniform sampler2D sprite0;

	void main() {
		vec2 uv = gl_FragCoord / resolution.xy;

		float X = uv.x * 25. + time;
		float Y = uv.y * 25. + time;
		uv.y += cos(X + Y) * 0.005 * cos(Y);
		uv.x += sin(X - Y) * 0.005 * sin(Y);

		gl_FragColor = texture2D(sprite0, uv) * vec4(1., 1., 1., 1.);
	}
	);

	std::string vertexShader = STRINGIFY(
		attribute vec4 a_position;
	attribute vec2 a_texCoord;

	varying vec2 v_texCoord;

	void main() {
		gl_Position = CC_MVPMatrix * a_position;
		v_texCoord = a_texCoord;
	}
	);

	CCNode* orig;
	virtual bool init(CCNode* orig) {
		if (!CCNode::init()) return false;
		this->addChild(orig);

		rt = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
		rt->getSprite()->getTexture()->setAntiAliasTexParameters();
		s = rt->getSprite();
		rt->setScale(3);
		this->addChild(rt, 2);

		auto shader = new CCGLProgram;
		if (!shader->initWithVertexShaderByteArray(vertexShader.c_str(), fragmentShader.c_str()))
			MessageBoxA(NULL, "error", "shaders are not compiled", 0);

		shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		shader->link();
		shader->updateUniforms();

		m_uniformResolution = glGetUniformLocation(shader->getProgram(), "resolution");
		auto uniform_tex = glGetUniformLocation(shader->getProgram(), "sprite0");
		glUniform1i(uniform_tex, 0);
		ccGLBindTexture2D(s->getTexture()->getName());

		s->setShaderProgram(shader);
		s->getShaderProgram()->use();
		s->getShaderProgram()->setUniformsForBuiltins();

		auto glv = CCDirector::sharedDirector()->getOpenGLView();
		auto frSize = glv->getFrameSize();
		s->getShaderProgram()->setUniformLocationWith2f(m_uniformResolution, frSize.width, frSize.height);

		shader->release();

		return 1;
	}

	void visit() override {
		CCNode::visit();

		rt->beginWithClear(0, 0, 0, 0);
		orig->visit();
		rt->end();
	}
public:
	static WavyNode* create(CCNode* orig) {
		auto ret = new WavyNode();
		ret->orig = orig;
		if (ret && ret->init(orig)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	void setPosition(CCPoint pos) {
		orig->setPosition(pos);
	}
	void setScale(float scale) {
		orig->setScale(scale);
	}
	void setScaleX(float scale) {
		orig->setScaleX(scale);
	}
	void setScaleY(float scale) {
		orig->setScaleY(scale);
	}
	void runAction(CCAction* action) {
		orig->runAction(action);
	}
	CCSize getContentSize() {
		return orig->getContentSize();
	}
};


class $modify(ModMenuLayer, MenuLayer) {
	CCSprite* bg;
	float bgScaleX;
	float bgScaleY;

	void onOptionsInstant() {
		//nothing
	}

	bool init() {
		DataManager::getInstance(); //нужно для первоначальной загрузки данных
		GameSoundManager::sharedManager()->playBackgroundMusic("mainSong.mp3"_spr, 1, 0);
		auto size = CCDirector::sharedDirector()->getWinSize();
		/*
		m_fields->bg = WavyNode::create(CCSprite::create("bg.png"_spr));
		m_fields->bgScaleX = size.width / m_fields->bg->getContentSize().width;
		m_fields->bgScaleY = size.height / m_fields->bg->getContentSize().height;
		m_fields->bg->setScaleX(m_fields->bgScaleX);
		m_fields->bg->setScaleY(m_fields->bgScaleY);
		m_fields->bg->setPosition(size / 2);
		this->addChild(m_fields->bg);
*/
		m_fields->bg = CCSprite::create("bg.png"_spr);
		m_fields->bgScaleX = size.width / m_fields->bg->getContentSize().width;
		m_fields->bgScaleY = size.height / m_fields->bg->getContentSize().height;
		m_fields->bg->setScaleX(m_fields->bgScaleX);
		m_fields->bg->setScaleY(m_fields->bgScaleY);
		m_fields->bg->setPosition(size / 2);
		this->addChild(m_fields->bg);

		auto playBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png"), this, menu_selector(ModMenuLayer::onMyPlayBtn));
		playBtn->setPosition(size / 2);
		this->addChild(playBtn);

		auto garageBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png"), this, menu_selector(ModMenuLayer::onMyGarage));
		garageBtn->setPosition(size.width / 2 - 120, size.height / 2);
		this->addChild(garageBtn);

		auto createBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png"), this, menu_selector(ModMenuLayer::onMySelectCreatedLevel));
		createBtn->setPosition(size.width / 2 + 120, size.height / 2);
		this->addChild(createBtn);

		auto accountBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png"), this, menu_selector(ModMenuLayer::onMyAccount));
		accountBtn->setPosition(50, 50);
		this->addChild(accountBtn);

		auto dailyChestBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png"), this, menu_selector(ModMenuLayer::onMyDailyChest));
		dailyChestBtn->setPosition(size.width - 30, size.height / 2);
		this->addChild(dailyChestBtn);

		auto graphixBtn = Button::create(CCSprite::createWithSpriteFrameName("graphixBtn.png"_spr), this, menu_selector(ModMenuLayer::onGraphix));
		graphixBtn->setPosition(size.width / 2 + 75, 50);
		this->addChild(graphixBtn);

		auto rainixBtn = Button::create(CCSprite::createWithSpriteFrameName("rainixBtn.png"_spr), this, menu_selector(ModMenuLayer::onRainix));
		rainixBtn->setPosition(size.width / 2, 50);
		this->addChild(rainixBtn);

		auto optionsBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"), this, menu_selector(ModMenuLayer::onMyOptions));
		optionsBtn->setPosition(size.width / 2 - 75, 50);
		this->addChild(optionsBtn);

		auto closeBtnSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
		closeBtnSprite->setScale(0.6);
		auto closeBtn = Button::create(closeBtnSprite, this, menu_selector(MenuLayer::onQuit));
		closeBtn->setPosition(20, size.height - 20);
		this->addChild(closeBtn);


		GameSoundManager::sharedManager()->enableMetering();
		this->schedule(schedule_selector(ModMenuLayer::pulseLoop), 0.05);

		return 1;
	}
	
	void onRainix(CCObject*) {
		RainixPopup::create()->show();
	}

	void onMyOptions(CCObject*) {
		MoreOptionsLayer::create()->show();
	}

	void onGraphix(CCObject*) {
		VideoOptionsLayer::create()->show();
	}

	void pulseLoop(float dt) {
		auto volumeNow = (FMODAudioEngine::sharedEngine()->m_pulse1 + FMODAudioEngine::sharedEngine()->m_pulse2 + FMODAudioEngine::sharedEngine()->m_pulse3) / 3;
		volumeNow = volumeNow * volumeNow * volumeNow * 2;
		m_fields->bg->runAction(CCScaleTo::create(0.05, m_fields->bgScaleX + volumeNow / 6));
	}

	void onMyPlayBtn(CCObject*) {
		if (DataManager::getInstance()->getUsername() == "") {
			createQuickPopup(
				"!!!",
				"Do you want to continue without <cg>account</c>? You will <cr>NOT</c> be able to public or rate levels without an <cg>account</c>!",
				"no", "yes",
				[this](auto self, bool btn2) {
					if (btn2) {
						CCScene* scene = CCScene::create();
						MapUiLayer* mapUiLayer = MapUiLayer::create(scene);
						scene->addChild(mapUiLayer);
						CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, scene));
					}
				}
			);
		}
		else {
			CCScene* scene = CCScene::create();
			MapUiLayer* mapUiLayer = MapUiLayer::create(scene);
			scene->addChild(mapUiLayer);
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, scene));
		}
	}

	void onMyGarage(CCObject* obj) {
		MenuLayer::onGarage(obj);
	}

	void onMySelectCreatedLevel(CCObject* obj) {
		CreatedLevelsLayer* createdLevelsLayer = CreatedLevelsLayer::create();
		cocos2d::CCScene* scene = cocos2d::CCScene::create();
		scene->addChild(createdLevelsLayer);
		cocos2d::CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, scene));
	}

	void onMyAccount(CCObject* obj) {
		auto accountLayer = AccountLayer::create();
		this->addChild(accountLayer);
		accountLayer->showLayer(0);
	}

	void onMyDailyChest(CCObject*) {
		if (DataManager::getInstance()->getUsername() != "" && DataManager::getInstance()->getPassword() != "")
			DailyLoadingAlert::create()->show();
		else
			FLAlertLayer::create("!!!", "You need account for it!", "ok")->show();
	}
};
