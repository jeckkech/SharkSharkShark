#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"
#include "MainGameScene.h"
#include "OptionsScene.h"

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
	UserDefault *def = UserDefault::getInstance();

	auto node = CSLoader::createNode("MainMenu.csb");
	auto panel = node->getChildByName("mainMenu")->getChildByName("mainMenuPanel");

	auto startButton = static_cast<Button*>(panel->getChildByName("startBtn"));
	auto galleryButton = static_cast<Button*>(panel->getChildByName("galleryBtn"));
	auto exitButton = static_cast<Button*>(panel->getChildByName("exitBtn"));

	auto muteCheckbox = static_cast<CheckBox*>(panel->getChildByName("muteCheckbox"));
	
	if (def->getBoolForKey("silentMode")) {
		muteCheckbox->setSelected(true);
	}
	CCSprite::create(startButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	CCSprite::create(galleryButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();

	static_cast<Sprite*>(panel->getChildByName("background"))->getTexture()->setAliasTexParameters();

	startButton->addTouchEventListener(this, toucheventselector(MainMenu::startCallback));
	galleryButton->addTouchEventListener(this, toucheventselector(MainMenu::galleryCallback));
	exitButton->addTouchEventListener(this, toucheventselector(MainMenu::exitCallback));

	muteCheckbox->addEventListener(CC_CALLBACK_2(MainMenu::muteCallback, this));
	
	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setContentSize(visibleSize);

	ui::Helper::doLayout(node);
	this->addChild(node, 10);
	return true;
}

void MainMenu::exitCallback(Ref* pSender)
{
	Director::getInstance()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}

void MainMenu::startCallback(Ref* pSender)
{
	auto scene = MainGame::createScene();
	Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0, scene));
}

void MainMenu::galleryCallback(Ref* pSender)
{
	auto scene = Options::createScene();
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5, scene));
}

void MainMenu::muteCallback(Ref* pSender, CheckBox::EventType type)
{
	UserDefault *def = UserDefault::getInstance();
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		def->setBoolForKey("silentMode", true);
		break;

	case CheckBox::EventType::UNSELECTED:
		def->setBoolForKey("silentMode", false);
		break;

	default:
		break;
	}
}