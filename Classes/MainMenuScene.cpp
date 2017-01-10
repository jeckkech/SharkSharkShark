#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "MainGameScene.h"
#include "OptionsScene.h"
#include <iomanip>

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* MainMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = MainMenu::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/exit.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/select.wav");
	UserDefault *def = UserDefault::getInstance();

	auto node = CSLoader::createNode("scenes/MainMenu.csb");
	auto panel = node->getChildByName("mainMenu")->getChildByName("mainMenuPanel");

	auto startButton = static_cast<Button*>(panel->getChildByName("startBtn"));
	auto infiniteButton = static_cast<Button*>(panel->getChildByName("infiniteBtn"));
	auto galleryButton = static_cast<Button*>(panel->getChildByName("galleryBtn"));
	auto exitButton = static_cast<Button*>(panel->getChildByName("exitBtn"));

	auto muteCheckbox = static_cast<CheckBox*>(panel->getChildByName("muteCheckbox"));
	auto hiScoreLabel = static_cast<Text*>(panel->getChildByName("hiScore"));
	if (def->getBoolForKey("silentMode")) {
		muteCheckbox->setSelected(true);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(2.5f);
	}
	if (def->getBoolForKey("infiniteModeUnlocked")) {
		infiniteButton->setVisible(true);
		panel->getChildByName("lockDark")->setVisible(false);
		panel->getChildByName("infiniteBtnLocked")->setVisible(false);
	}
	else {
		static_cast<Sprite*>(panel->getChildByName("lockDark"))->getTexture()->setAliasTexParameters();
		static_cast<Sprite*>(panel->getChildByName("infiniteBtnLocked"))->getTexture()->setAliasTexParameters();
	}
	CCSprite::create(infiniteButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	CCSprite::create(startButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	CCSprite::create(galleryButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();

	static_cast<Sprite*>(panel->getChildByName("background"))->getTexture()->setAliasTexParameters();

	startButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::startCallback, this));
	infiniteButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::startInfiniteCallback, this));
	galleryButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::galleryCallback, this));
	exitButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::exitCallback, this));

	muteCheckbox->addEventListener(CC_CALLBACK_2(MainMenu::muteCallback, this));
	
	int hiScore = UserDefault::getInstance()->getIntegerForKey("hiScore");
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(8) << hiScore;
	hiScoreLabel->setString(ss.str());
	ss.clear();
	ss.str(std::string());

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setContentSize(visibleSize);

	ui::Helper::doLayout(node);
	this->addChild(node, 10);
	return true;
}

unsigned getNumberOfDigits(unsigned i)
{
	return i > 0 ? (int)log10((double)i) + 1 : 1;
}

void MainMenu::exitCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/select.wav");
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}

void MainMenu::startCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN) {
		auto scene = MainGame::createScene();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/exit.wav");
		Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0, scene));
	}
}

void MainMenu::startInfiniteCallback(Ref* pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/exit.wav");
		auto scene = MainGame::createSceneInfinite();
		Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0, scene));
	}
}
void MainMenu::galleryCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/exit.wav");
		auto scene = Options::createScene();
		Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5, scene));
	}
}

void MainMenu::muteCallback(Ref* pSender, CheckBox::EventType type)
{
	UserDefault *def = UserDefault::getInstance();
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		def->setBoolForKey("silentMode", true);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
		break;

	case CheckBox::EventType::UNSELECTED:
		def->setBoolForKey("silentMode", false);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(2.5f);
		break;

	default:
		break;
	}
}