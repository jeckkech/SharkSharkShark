#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "OptionsScene.h"
#include "MainMenuScene.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* Options::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = Options::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Options::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto node = CSLoader::createNode("OptionsScene.csb");

	auto panel = node->getChildByName("optionsPage")->getChildByName("optionsPanel");

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	auto backButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("backBtn"));

	auto scrollView = static_cast<cocos2d::ui::ScrollView*>(panel->getChildByName("scrollGallery"));
	


	backButton->addTouchEventListener(this, toucheventselector(Options::backCallback));

	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setContentSize(visibleSize);

	ui::Helper::doLayout(node);
	this->addChild(node, 2);
	return true;
}

void Options::backCallback(Ref* pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5, scene));
}