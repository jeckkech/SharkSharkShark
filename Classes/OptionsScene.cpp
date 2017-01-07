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

	auto node = CSLoader::createNode("scenes/OptionsScene.csb");

	auto panel = node->getChildByName("optionsPage");

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	auto backButton = static_cast<cocos2d::ui::Button*>(node->getChildByName("backBtn"));
	auto background = static_cast<Sprite*>(node->getChildByName("background"));

	backButton->setScale(visibleSize.width / 5 / backButton->getContentSize().width);
	background->setScale(visibleSize.height / background->getContentSize().height);
	background->getTexture()->setAliasTexParameters();
	CCSprite::create(backButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	
	backButton->addTouchEventListener(CC_CALLBACK_1(Options::backCallback, this));

	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setContentSize(visibleSize);
	node->setName("OptionsPage");

	ui::Helper::doLayout(node);
	this->addChild(node, 2);
	unlockElements();

	return true;
}

void Options::backCallback(Ref* pSender)
{
	static_cast<cocos2d::ui::PageView*>(this->getChildByName("OptionsPage")->getChildByName("optionsPage"))->setClippingEnabled(false);
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5, scene));
}

void Options::unlockElements()
{
	auto panel0 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel0")->getChildByName("galleryPanel");
	auto panel1 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel1")->getChildByName("galleryPanel");
	auto panel2 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel2")->getChildByName("galleryPanel");
	auto panel3 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel3")->getChildByName("galleryPanel");
	auto panel4 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel4")->getChildByName("galleryPanel");
	auto panel5 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel5")->getChildByName("galleryPanel");
	auto panel6 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel6")->getChildByName("galleryPanel");
	auto panel7 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel7")->getChildByName("galleryPanel");
	auto panel8 = this->getChildByName("OptionsPage")->getChildByName("optionsPage")->getChildByName("panel8")->getChildByName("galleryPanel");


	for (int o = 1; o <= 5; o++) {
		if (UserDefault::getInstance()->getIntegerForKey("playerStage") >= o) {
			char lockName[10] = { 0 };
			sprintf(lockName, "lock%i", o);

			char imgName[10] = { 0 };
			sprintf(imgName, "img%i", o);

			panel0->getChildByName(lockName)->setVisible(false);
			static_cast<Sprite*>(panel0->getChildByName(imgName))->getTexture()->setAliasTexParameters();
			panel0->getChildByName(imgName)->setColor(Color3B(225, 225, 225));
		}
	}

	if (UserDefault::getInstance()->getIntegerForKey("playerStage") >= 6) {
		if (UserDefault::getInstance()->getIntegerForKey("playerStageFinal") == 1) {
			panel1->getChildByName("lock1")->setVisible(false);
			panel1->getChildByName("img1")->setColor(Color3B(225, 225, 225));
			static_cast<Sprite*>(panel1->getChildByName("img1"))->getTexture()->setAliasTexParameters();
			static_cast<Text*>(panel1->getChildByName("description"))->setString("that's a cool fish\nnamed Arin!");
		}
		else {
			panel2->getChildByName("lock1")->setVisible(false);
			static_cast<Sprite*>(panel2->getChildByName("img1"))->getTexture()->setAliasTexParameters();
			panel2->getChildByName("img1")->setColor(Color3B(225, 225, 225));
			static_cast<Text*>(panel1->getChildByName("description"))->setString("that's a curly-scaled\nDanny-fish!");
		}
	}

	for (int o = 1; o <= 5; o++) {
		if (UserDefault::getInstance()->getIntegerForKey("fishStage") >= o) {
			char lockName[10] = { 0 };
			sprintf(lockName, "lock%i", o);

			char imgName[10] = { 0 };
			sprintf(imgName, "img%i", o);

			panel3->getChildByName(lockName)->setVisible(false);
			static_cast<Sprite*>(panel3->getChildByName(imgName))->getTexture()->setAliasTexParameters();
			panel3->getChildByName(imgName)->setColor(Color3B(225, 225, 225));
		}
	}

	for (int o = 1; o <= 2; o++) {
		if (UserDefault::getInstance()->getIntegerForKey("cancerStage") >= o) {
			char lockName[10] = { 0 };
			sprintf(lockName, "lock%i", o);

			char imgName[10] = { 0 };
			sprintf(imgName, "img%i", o);

			panel4->getChildByName(lockName)->setVisible(false);
			panel4->getChildByName(imgName)->setColor(Color3B(225, 225, 225));
			static_cast<Sprite*>(panel4->getChildByName(imgName))->getTexture()->setAliasTexParameters();
			static_cast<Text*>(panel4->getChildByName("description"))->setString("These guys are a piece \nof a relentless crabbage.\nThey love to hang out on a \nbottom and eventually jump up.");
		}
	}

	if (UserDefault::getInstance()->getIntegerForKey("jellyFishStage") == 1) {
		panel5->getChildByName("lock1")->setVisible(false);
		panel5->getChildByName("img1")->setColor(Color3B(225, 225, 225));
		static_cast<Sprite*>(panel5->getChildByName("img1"))->getTexture()->setAliasTexParameters();
		static_cast<Text*>(panel5->getChildByName("description"))->setString("Silent but violent.\nIt may look small but\nit will straight up\nkill you.");
	}
	if (UserDefault::getInstance()->getIntegerForKey("seaHorseStage") == 1) {
		panel6->getChildByName("lock1")->setVisible(false);
		panel6->getChildByName("img1")->setColor(Color3B(225, 225, 225));
		static_cast<Sprite*>(panel6->getChildByName("img1"))->getTexture()->setAliasTexParameters();
		static_cast<Text*>(panel6->getChildByName("description"))->setString("This guy is\nrare and fast\nbut he's the most\nvaluable one.");
	}
	if (UserDefault::getInstance()->getIntegerForKey("sharkStage") == 1) {
		panel7->getChildByName("lock1")->setVisible(false);
		panel7->getChildByName("img1")->setColor(Color3B(225, 225, 225));
		static_cast<Sprite*>(panel7->getChildByName("img1"))->getTexture()->setAliasTexParameters();
		static_cast<Text*>(panel7->getChildByName("description"))->setString("This guy looks tough\nbut he's got a\nweak spot on\nits tail.\nBut you have to be\ntough to do it.");
	} 
	if (UserDefault::getInstance()->getIntegerForKey("rossStage") == 1) {
		panel8->getChildByName("lock1")->setVisible(false);
		panel8->getChildByName("img1")->setColor(Color3B(225, 225, 225));
		static_cast<Sprite*>(panel8->getChildByName("img1"))->getTexture()->setAliasTexParameters();
		static_cast<Text*>(panel8->getChildByName("description"))->setString("An evil octopus\nnamed ROSS!\nGODDAMIT!\nhe's invincible during\nhis rage mode.");
	}
}