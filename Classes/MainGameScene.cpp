#include "SimpleAudioEngine.h"
#include "MainGameScene.h"
#include "MainMenuScene.h"
#include "AdBuddizHelper.h"
#include "Fish.h"
#include "SeaHorse.h"
#include "Cancer.h"
#include "JellyFish.h"
#include "PlayerFish.h"
#include "Shark.h"
#include "Ross.h"
#include "Skeleton.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <iomanip>

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;

cocos2d::Vec2 MainGame::playerPosition;
int MainGame::mode;

Scene* MainGame::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setSpeed(0.5);
	scene->getPhysicsWorld()->setGravity(Vect(0, -7));
	// 'layer' is an autorelease object
	auto layer = MainGame::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	scene->addChild(layer);
	MainGame::mode = 0;
	// return the scene
	return scene;
}

Scene* MainGame::createSceneInfinite() {
	auto scene = MainGame::createScene();
	MainGame::mode = 1;
	return scene;
}
// on "init" you need to initialize your instance
bool MainGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Director::getInstance()->setProjection(Director::Projection::_2D);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1.0f, Vec2(origin.x, origin.y));
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	/*auto background = CCSprite::create("bg1.png");
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(origin.x, origin.y));
	background->getTexture()->setAliasTexParameters();
	*/
	parallaxBg2 = CCSprite::create("bg2.png");
	parallaxBg2->setAnchorPoint(Point(0, 0));
	parallaxBg2->setPosition(Point(origin.x, origin.y));
	parallaxBg2->getTexture()->setAliasTexParameters();

	parallaxBg = CCSprite::create("bg3.png");
	parallaxBg->setAnchorPoint(Point(parallaxBg->getAnchorPoint().x, 0));
	parallaxBg->setPosition(Point(visibleSize.width/2 + origin.x, origin.y));
	parallaxBg->getTexture()->setAliasTexParameters();

	parallaxBg2->setScale(visibleSize.width / parallaxBg2->getContentSize().width);
	this->addChild(parallaxBg2, 2);

	parallaxBg->setScale(visibleSize.height / parallaxBg->getContentSize().height);
	this->addChild(parallaxBg, 3);

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/player_death.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/lifebar.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/beep.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/boop.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/swim.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/eat_fish.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/fishes_collide.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/select.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/bite.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/ross_hurt.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/boss_appear.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/exit.wav");

	auto node = CSLoader::createNode("scenes/HUDElements.csb");
	
	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setName("HUDElements");

	node->setContentSize(visibleSize);
	auto panel = node->getChildByName("HUDElements")->getChildByName("hudPanel");

	auto continueButton = static_cast<Button*>(panel->getChildByName("continuePrompt")->getChildByName("promptButtonYes"));
	auto continueButtonDiscart = static_cast<Button*>(panel->getChildByName("continuePrompt")->getChildByName("promptButtonNo"));
	
	continueButton->addTouchEventListener(CC_CALLBACK_2(MainGame::continueCallback, this));
	continueButtonDiscart->addTouchEventListener(CC_CALLBACK_2(MainGame::exit, this));

	auto biteButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("biteActionPanel")->getChildByName("biteButton"));
	auto biteIcon = static_cast<Node*>(panel->getChildByName("biteActionPanel")->getChildByName("biteIconAnimation"));
	auto biteLabel = static_cast<Node*>(panel->getChildByName("biteActionPanel")->getChildByName("biteLabel"));

	biteButton->addTouchEventListener(CC_CALLBACK_2(MainGame::biteCallback, this));

	Sprite::create("res/buttons/bite1.png")->getTexture()->setAliasTexParameters();
	Sprite::create("res/buttons/bite2.png")->getTexture()->setAliasTexParameters();
	Sprite::create("res/buttons/bite3.png")->getTexture()->setAliasTexParameters();

	auto biteButtonTimeline = (cocostudio::timeline::ActionTimeline*)biteIcon->getActionByTag(biteIcon->getTag());
	biteButtonTimeline->setTimeSpeed(0.04f);
	biteButtonTimeline->gotoFrameAndPlay(0);
	biteIcon->runAction(biteButtonTimeline);
	biteLabel->runAction(RepeatForever::create(Blink::create(1.0f, 2)));

	Sprite::create("res/hud/lifebar.png")->getTexture()->setAliasTexParameters();

	auto background = CSLoader::createNode("res/background/background.csb");
	auto bgNode = background->getChildByName("background");
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(origin.x, origin.y);
	background->setContentSize(visibleSize);
	bgNode->setScaleX(visibleSize.width / bgNode->getContentSize().width);
	bgNode->setScaleY(visibleSize.height / bgNode->getContentSize().height);
	cocostudio::timeline::ActionTimeline* pActionTimeline = CSLoader::createTimeline("res/background/background.csb");
	pActionTimeline->gotoFrameAndPlay(0);
	bgNode->runAction(pActionTimeline);
	this->addChild(background, 0);

	auto exitButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("hudBlockPanel")->getChildByName("exitBtn"));

	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->setEnabled(true);
	
	exitButton->addTouchEventListener(CC_CALLBACK_2(MainGame::menuCloseCallback, this));

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	auto repeatButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("gameOverPanel")->getChildByName("repeatBtn"));
	CCSprite::create(repeatButton->getNormalFile().file)->getTexture()->setAliasTexParameters();

	repeatButton->addTouchEventListener(CC_CALLBACK_2(MainGame::repeatCallback, this));

	scoreLabel = static_cast<cocos2d::ui::Text*>(panel->getChildByName("scoreLabel"));


	ui::Helper::doLayout(node);
	this->addChild(node, 5);

	this->scheduleUpdate();
	
	playerFish = PlayerFish::create();
	playerFish->setName("playerNode");
	this->addChild(playerFish, 2);
	playerFish->createFish(1);
	
	
	playerContactListener = EventListenerPhysicsContact::create();
	playerContactListener->onContactBegin = CC_CALLBACK_1(MainGame::onContactBegin, this);
	playerContactListener->onContactSeparate = CC_CALLBACK_1(MainGame::onContactSeparate, this);
	
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerContactListener, this);

	return true;
}

void MainGame::decreaseLifebarPosition() {
	if(pActionTimeline->getCurrentFrame() >= 5){
		auto panel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel");
		auto bossLifebar = static_cast<Node*>(panel->getChildByName("lifebar"));
		int newFrame = pActionTimeline->getCurrentFrame() - 5;

		pActionTimeline->gotoFrameAndPause(newFrame);
		pActionTimeline->setTimeSpeed(0.25f);
		this->runAction(pActionTimeline);
	}
}

void MainGame::gameOver() {
	this->unscheduleUpdate();
	this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("gameOverPanel")->setVisible(true);
	this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(false);
	this->getEventDispatcher()->removeEventListener(playerTouchListener);
	this->getEventDispatcher()->removeEventListener(playerContactListener);
	saveHiScore();
	isGameOver = true;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/player_death.wav");
	isBiteMode = false;
}
void MainGame::lifebarInitialize() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/lifebar.wav");
	auto panel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel");
	auto bossLifebar = static_cast<Node*>(panel->getChildByName("lifebar"));

	pActionTimeline = (cocostudio::timeline::ActionTimeline*)bossLifebar->getActionByTag(bossLifebar->getTag());
	pActionTimeline->retain();
	pActionTimeline->gotoFrameAndPlay(0, false);
	pActionTimeline->setTimeSpeed(0.25f);
	this->runAction(pActionTimeline);
}

void MainGame::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
		//COUNTDOWN
		isInCountdown = true;
		auto countdownPanel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown");
		auto countdownLabel = static_cast<cocos2d::ui::Text*>(countdownPanel->getChildByName("countdownLabel"));
		DelayTime *pause = DelayTime::create(1.0);

		this->runAction(Sequence::create(CallFunc::create([countdownPanel, countdownLabel]() {
		countdownPanel->setVisible(true);
		countdownLabel->setString("READY");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/beep.wav");
		}), pause, CallFunc::create([countdownLabel]() {
		countdownLabel->setFontSize(100);
		countdownLabel->setString("SET");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/beep.wav");
		}), pause->clone(),
		CallFuncN::create(std::bind(&MainGame::setUpInitial, this)),
		CallFunc::create([countdownLabel]() {
		countdownLabel->setString("GO!");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/boop.wav");
		ActionInterval *blink = CCBlink::create(2, 16);
		countdownLabel->runAction(blink);
		}), DelayTime::create(2.0),
		CallFunc::create([this, countdownPanel]() {
		countdownPanel->setVisible(false);
		isInCountdown = false;
		}),nullptr));
}

void MainGame::achievementUnlocked() {
	if(!isAchievementShowing){
		isAchievementShowing = true;
		auto achievementPanel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("achievementPanel");
		achievementPanel->setVisible(true);
		achievementPanel->runAction(Sequence::create(MoveTo::create(1.0f, Vec2(0, 0)),
			DelayTime::create(1.0f), MoveTo::create(1.0f, Vec2(0, -achievementPanel->getContentSize().height)),
			CallFunc::create([achievementPanel, this]() {
			achievementPanel->setVisible(false);
			isAchievementShowing = false;
			}),
			nullptr));
	}
}
void MainGame::blinkNotification(std::string str) {
	auto countdownPanel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown");
	auto countdownLabel = static_cast<cocos2d::ui::Text*>(countdownPanel->getChildByName("countdownLabel"));
	isInCountdown = true;

	this->runAction(Sequence::create(
		CallFunc::create([str, countdownPanel, countdownLabel]() {
		countdownPanel->setVisible(true);
		countdownLabel->setString(str);
		countdownLabel->setFontSize(50);
		ActionInterval *blink = CCBlink::create(2, 16);
		countdownLabel->runAction(blink);
	}), DelayTime::create(2.0),
		CallFunc::create([this, countdownPanel]() {
		countdownPanel->setVisible(false);
		isInCountdown = false;
	}), nullptr));
}

void MainGame::setUpInitial(){
	Fish* fish1 = Fish::create();
	this->addChild(fish1, 2);
	fish1->drawFish(1);

	Fish* fish2 = Fish::create();
	this->addChild(fish2, 2);
	fish2->drawFish(1);

	Fish* fish3 = Fish::create();
	this->addChild(fish3, 2);
	fish3->drawFish(2);

	Fish* fish5 = Fish::create();
	this->addChild(fish5, 2);
	fish5->drawFish(2);

	Fish* fish6 = Fish::create();
	this->addChild(fish6, 2);
	fish6->drawFish(2);

	if (UserDefault::getInstance()->getIntegerForKey("fishStage") < 2) {
		achievementUnlocked();
	}
	UserDefault::getInstance()->setIntegerForKey("fishStage", 2);

	playerTouchListener = EventListenerTouchOneByOne::create();
	playerTouchListener->setSwallowTouches(true);
	playerTouchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchMove, this);
	playerTouchListener->onTouchMoved = CC_CALLBACK_2(MainGame::onTouch, this);
	playerTouchListener->onTouchEnded = CC_CALLBACK_2(MainGame::onTouchStop, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerTouchListener, this);
}
void MainGame::update(float dt) {
	MainGame::playerPosition = Vec2(playerFish->getPositionX(), playerFish->getPositionY());
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float playerFishPos = 0;
	
	playerFishPos += playerFish == NULL ? 0 : playerFish->getPositionX();
	
	float bgMargin = visibleSize.width / 2 - playerFishPos + origin.x;
	
	parallaxBg->setPositionX(origin.x + visibleSize.width/2 + bgMargin/2);
	parallaxBg2->setPositionX(origin.x + bgMargin / 5);

	if(MainGame::mode == 0){
		if (totalScore >= 500 && !hwStage1) {
			hwStage1 = true;
		
			Cancer* cancer = Cancer::create();
			this->addChild(cancer, 2);
			cancer->drawFish(1);

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(2);

			if (UserDefault::getInstance()->getIntegerForKey("cancerStage") < 1) {
				achievementUnlocked();
			}
			UserDefault::getInstance()->setIntegerForKey("cancerStage", 1);
		}
		if (totalScore >= 750 && !hwStage7_5) {
			hwStage7_5 = true;
			blinkNotification("PREPARE FOR\nA SHARK\nATTACK!");
			lifebarInitialize();

			shark = Shark::create();
			shark->setName("sharkNode");
			this->addChild(shark, 2);
			shark->drawFish();
			if (UserDefault::getInstance()->getIntegerForKey("sharkStage") < 1) {
				achievementUnlocked();
			}
			UserDefault::getInstance()->setIntegerForKey("sharkStage", 1);
		}
		if (totalScore >= 1000 && !stage1) {
			stage1 = true;

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(3);
		
			++topStage;
		
			if (UserDefault::getInstance()->getIntegerForKey("fishStage") < topStage) {
				achievementUnlocked();
			}
			UserDefault::getInstance()->setIntegerForKey("fishStage", topStage);
		}

		if(totalScore >= 2000 && !hwStage2){
			hwStage2 = true;
			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(4);
			++topStage;
	
			if (UserDefault::getInstance()->getIntegerForKey("fishStage") < topStage || 
				UserDefault::getInstance()->getIntegerForKey("jellyFishStage") < 1) {
				achievementUnlocked();
			}

			UserDefault::getInstance()->setIntegerForKey("fishStage", topStage);
			UserDefault::getInstance()->setIntegerForKey("jellyFishStage", 1);


			JellyFish* jf = JellyFish::create();
			this->addChild(jf, 2);
			jf->drawFish(1);
		}

		if (totalScore >= 3000 && !hwStage3) {
			hwStage3 = true;

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(5);
			++topStage;

			if (UserDefault::getInstance()->getIntegerForKey("fishStage") < topStage
				|| UserDefault::getInstance()->getIntegerForKey("cancerStage") < 2) {
				achievementUnlocked();
			}
			UserDefault::getInstance()->setIntegerForKey("fishStage", topStage);

			Cancer* cancer2 = Cancer::create();
			this->addChild(cancer2, 2);
			cancer2->drawFish(2);

			UserDefault::getInstance()->setIntegerForKey("cancerStage", 2);
		}

		if (totalScore >= 4000 && !hwStage4) {
			hwStage4 = true;
			achievementUnlocked();
		}

		if (totalScore >= 5000 && !hwStage5) {
			hwStage5 = true;
			if(!UserDefault::getInstance()->getBoolForKey("playerStageFinal") || 
				!UserDefault::getInstance()->getBoolForKey("playerStageFinal_")){
				achievementUnlocked();
			}
		}
	}
	else if (MainGame::mode == 1) {
		topStage = 5;
		if (totalScore >= 500 && !hwStage1) {
			hwStage1 = true;

			Cancer* cancer = Cancer::create();
			this->addChild(cancer, 2);
			cancer->drawFish(1);

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(3);
		}
		if (totalScore >= 1000 && !stage1) {
			stage1 = true;

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(4);
		}

		if (totalScore >= 2000 && !hwStage2) {
			hwStage2 = true;
			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(5);

			JellyFish* jf = JellyFish::create();
			this->addChild(jf, 2);
			jf->drawFish(1);
		}

		if (totalScore >= 3000 && !hwStage3) {
			hwStage3 = true;

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(5);

			Cancer* cancer2 = Cancer::create();
			this->addChild(cancer2, 2);
			cancer2->drawFish(2);
		}

		if (totalScore >= 4000 && !hwStage4) {
			hwStage4 = true;
			JellyFish* jf = JellyFish::create();
			this->addChild(jf, 2);
			jf->drawFish(1);

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(5);
		}

		if (totalScore >= 6000 && !hwStage5) {
			hwStage5 = true;

			Cancer* cancer2 = Cancer::create();
			this->addChild(cancer2, 2);
			cancer2->drawFish(2);
		}

		if (totalScore >= 8000 && !hwStage8) {
			hwStage8 = true;

			JellyFish* jf = JellyFish::create();
			this->addChild(jf, 2);
			jf->drawFish(1);

			Fish* fish6 = Fish::create();
			this->addChild(fish6, 2);
			fish6->drawFish(5);
		}
	}
}

bool MainGame::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	playerFish->refreshPlayerDirection(touch->getLocation().x);
	playerFish->stopActionByTag(1444);
	float distance = playerFish->getPosition().getDistance(touch->getLocation());
	auto action = MoveTo::create(distance/(playerFish->getStage()+15), touch->getLocation());
	action->setTag(1444);
	playerFish->runAction(action);
}

bool MainGame::onTouch(cocos2d::Touch *touch, cocos2d::Event *event) {
	if(!timerUpdated){
		playerFish->stopActionByTag(1444);
		playerFish->refreshPlayerDirection(touch->getLocation().x);
		float distance = playerFish->getPosition().getDistance(touch->getLocation());
		auto action = MoveTo::create(distance / (playerFish->getStage() + 15 + 3), touch->getLocation());
		action->setTag(1444);
		playerFish->runAction(action);
		timerUpdated = true;
		this->scheduleOnce(schedule_selector(MainGame::updateTimer), 0.1f);
	}
}

void MainGame::updateTimer(float dt) {
	timerUpdated = false;
}

bool MainGame::onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/swim.wav");
	playerFish->refreshPlayerDirection(touch->getLocation().x);
	playerFish->stopActionByTag(1444);
	float distance = playerFish->getPosition().getDistance(touch->getLocation());
	auto action = MoveTo::create(distance / (playerFish->getStage() + 15), touch->getLocation());
	action->setTag(1444);
	playerFish->runAction(action);
}

bool MainGame::onContactSeparate(cocos2d::PhysicsContact &contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	int collisionBitmaskA = contact.getShapeA()->getCollisionBitmask();
	int collisionBitmaskB = contact.getShapeB()->getCollisionBitmask();

	if (playerFish->getScore() >= bossLimit && 
		((collisionBitmaskA == 0x15 && collisionBitmaskB == 0x09) || (collisionBitmaskA == 0x09 && collisionBitmaskB == 0x15))) {
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(false);
	}
}
bool MainGame::onContactBegin(cocos2d::PhysicsContact &contact) {
	Vec2 position = Vec2(0, 0);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bool userCollided = false;
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	int collisionBitmaskA = contact.getShapeA()->getCollisionBitmask();
	int collisionBitmaskB = contact.getShapeB()->getCollisionBitmask();
	
	if (playerFish->getScore() >= 50 && 
		((collisionBitmaskA == 0x15 && collisionBitmaskB == 0x09) ||
		(collisionBitmaskB == 0x15 && collisionBitmaskA == 0x09))) {
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(true);
		isBiteMode = true;
	}
	if (playerFish->isBlinking() && (collisionBitmaskA == 0x09 || collisionBitmaskB == 0x09)) {
		return false;
	}
	if ((collisionBitmaskA == 0x16 && collisionBitmaskB == 0x09)) {
		gameOver();
		static_cast<Fish*>(bodyB->getNode())->kill(true);
		return true;
	} else if ((collisionBitmaskA == 0x09 && collisionBitmaskB == 0x16)) {
		gameOver();
		static_cast<Fish*>(bodyA->getNode())->kill(true);
		return true;
	}
	if ((collisionBitmaskA == 0x15 || collisionBitmaskB == 0x15) || (collisionBitmaskA == 0x06 && collisionBitmaskB == 0x06) ||
		(collisionBitmaskA == 0x16 || collisionBitmaskB == 0x16) || (collisionBitmaskA == 0x14 && collisionBitmaskB == 0x06) ||
		(collisionBitmaskA == 0x06 && collisionBitmaskB == 0x14)) {
		return false;
	}

	if (bodyA->getPosition().x > origin.x && bodyB->getPosition().x > origin.x
		&& bodyA->getPosition().x < origin.x + visibleSize.width && bodyB->getPosition().x < origin.x + visibleSize.width
		&& collisionBitmaskA != 0x07 && collisionBitmaskB != 0x07) {
		if (contact.getShapeA() && contact.getShapeB() &&
			collisionBitmaskA != 0x04 && collisionBitmaskB != 0x04) {
			
			Fish* node1 = (Fish*)bodyA->getNode();
			Fish* node2 = (Fish*)bodyB->getNode();

			if (node1 && node2 && (node1->getScore() == node2->getScore() || node1->getScore() > node2->getScore())) {
				CCLOG("SCORE SCORE1: %i", node1->getScore());
				CCLOG("SCORE SCORE2: %i", node2->getScore());
				if (collisionBitmaskA == 0x09) {
					if (collisionBitmaskB == 0x08) {
						setScore(200);
						((PlayerFish*)bodyA->getNode())->increaseScore(2.0f);
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
						createScoreLabel(200, bodyB->getNode()->getPosition());
					}
					else {
						setScore(100);
						((PlayerFish*)bodyA->getNode())->increaseScore(1.0f);
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
						createScoreLabel(100, bodyB->getNode()->getPosition());
					}
					
					userCollided = true;
				}
				else if (collisionBitmaskB == 0x09) {
					gameOver();
					userCollided = true;
				}
				if (collisionBitmaskA != 0x09 && bodyA->getCollisionBitmask() != 0x06) {
					node1->increaseScore(1.0f);
					node1->increaseScale();
				}

				position = node2->getPosition();

				if (node2->getPositionX() < origin.x || collisionBitmaskB == 0x06 || 
					collisionBitmaskA == 0x06 || collisionBitmaskA == 0x14 ||
					collisionBitmaskB == 0x14) {
					userCollided = true;
				}
				if (bodyB->getCollisionBitmask() == 0x08) {
					((SeaHorse*)bodyB->getNode())->kill(userCollided);
				}
				else {
					node2->kill(userCollided);
					if (collisionBitmaskA != 0x09 && collisionBitmaskB != 0x09) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/fishes_collide.wav");
					}
				}
			}
			else if (node1 && node2) {
				if (collisionBitmaskB == 0x09) {
					if (collisionBitmaskA == 0x08) {
						setScore(200);
						((PlayerFish*)bodyB->getNode())->increaseScore(2.0f);
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
						createScoreLabel(200, bodyA->getNode()->getPosition());
					}
					else {
						setScore(100);
						((PlayerFish*)bodyB->getNode())->increaseScore(1.0f);
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
						createScoreLabel(100, bodyA->getNode()->getPosition());
					}
					userCollided = true;
				}
				else if (collisionBitmaskA == 0x09) {
					gameOver();
					userCollided = true;
				}
				if (collisionBitmaskB != 0x09 && bodyB->getCollisionBitmask() != 0x06) {
					node2->increaseScore(1.0f);
					node2->increaseScale();
				}
				position = node1->getPosition();
				if (node1->getPositionX() < origin.x || collisionBitmaskA == 0x06 || 
					collisionBitmaskB == 0x06 || collisionBitmaskA == 0x14 ||
					collisionBitmaskB == 0x14) {
					userCollided = true;
				}
				if (collisionBitmaskA == 0x08) {
					((SeaHorse*)bodyA->getNode())->kill(userCollided);
				}
				else {
					node1->kill(userCollided);
					if (collisionBitmaskA != 0x09 && collisionBitmaskB != 0x09) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/fishes_collide.wav");
					}
				}

			}

			int newFishId = cocos2d::RandomHelper::random_int(1, topStage);
			int fishRandomizer = cocos2d::RandomHelper::random_int(static_cast<int>(0), static_cast<int>(10));

			if(fishRandomizer == 1 && newFishId == 1){
				SeaHorse* sh = SeaHorse::create();
				this->addChild(sh, 2);
				sh->drawFish(1);
				if (UserDefault::getInstance()->getIntegerForKey("seaHorseStage") < 1) {
					achievementUnlocked();
				}
				UserDefault::getInstance()->setIntegerForKey("seaHorseStage", 1);
			}
			else {
				Fish* f = Fish::create();
				this->addChild(f, 2);
				f->drawFish(newFishId);
			}

		}
		else if (contact.getShapeA() && contact.getShapeB()) {
			if (collisionBitmaskA == 0x09) {
				((PlayerFish*)bodyA->getNode())->increaseScore(0.2f);
				setScore(20);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
				createScoreLabel(20, bodyB->getNode()->getPosition());
				((Skeleton*)bodyB->getNode())->destroy();
				
			}
			else if (collisionBitmaskB == 0x09) {
				((PlayerFish*)bodyB->getNode())->increaseScore(0.2f);
				setScore(20);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat_fish.wav");
				createScoreLabel(20, bodyA->getNode()->getPosition());
				((Skeleton*)bodyA->getNode())->destroy();
				
			}
		}
	}
}

void MainGame::saveHiScore() {
	if (UserDefault::getInstance()->getIntegerForKey("hiScore") < totalScore) {
		UserDefault::getInstance()->setIntegerForKey("hiScore", totalScore);
	}
}
void MainGame::createScoreLabel(int points, Vec2 position) {
	auto label = Label::createWithTTF(__String::createWithFormat("%i", points)->getCString(), "fonts/Gamegirl.ttf", 8);
	label->setPosition(position);
	label->enableShadow(Color4B(110, 110, 110, 255), Size(1.0, -1.0), 0);
	label->setColor(Color3B(255, 252, 0));
	label->getFontAtlas()->setAliasTexParameters();
	
	this->addChild(label, 2);
	label->runAction(MoveBy::create(100, Point(position.x, position.y + 500)));
	label->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([label]() {
		label->removeFromParentAndCleanup(true);
	}), nullptr));
}

void MainGame::setScore(int points) {
	totalScore = totalScore + points;

	if (UserDefault::getInstance()->getIntegerForKey("hiScore") > 0 &&
		UserDefault::getInstance()->getIntegerForKey("hiScore") < totalScore &&
		!hiScoreNotified) {
		hiScoreNotified = true;
		blinkNotification("NEW\nHIGHSCORE!");
		UserDefault::getInstance()->setIntegerForKey("hiScore", totalScore);
	}

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(7) << totalScore;
	scoreLabel->setString(ss.str());
	ss.clear();
	ss.str(std::string());
}

void MainGame::menuCloseCallback(Ref* pSender, Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::BEGAN){
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("gameOverPanel")->setVisible(false);
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown")->setVisible(false);
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(false);
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("staticNotificationPanel")->setVisible(false);
		auto prompt = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("exitPrompt");
		prompt->setVisible(true);
		Director::getInstance()->pause();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/select.wav");
		static_cast<Sprite*>(prompt->getChildByName("promptExit"))->getTexture()->setAliasTexParameters();

		auto exitButton = static_cast<cocos2d::ui::Button*>(prompt->getChildByName("promptButtonYes"));
		CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
		exitButton->addTouchEventListener(CC_CALLBACK_2(MainGame::exit, this));

		auto resumeButton = static_cast<cocos2d::ui::Button*>(prompt->getChildByName("promptButtonNo"));
		CCSprite::create(resumeButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
		resumeButton->addTouchEventListener(CC_CALLBACK_2(MainGame::resume, this));
	}
}

void MainGame::biteCallback(Ref* pSender, Widget::TouchEventType type) {
	if(type == Widget::TouchEventType::BEGAN){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/bite.wav");
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(false);
		isBiteMode = false;
		int health;
		if(!hwStage8){
			auto boss = static_cast<Shark*>(this->getChildByName("sharkNode"));
			boss->decreaseHealth();
			if (boss->getHealth() == 0) {
				hwStage8 = true;
				boss->kill();
				endingSequence1();
			}
		}
		else {
			auto boss = static_cast<Ross*>(this->getChildByName("rossNode"));
			boss->decreaseHealth();
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/ross_hurt.wav");
			if (boss->getHealth() == 0) {
				boss->kill();
				this->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([this]() {
					endingSequence2();
				}), nullptr));
			}
		}

		decreaseLifebarPosition();
		createScoreLabel(50, playerFish->getPosition());
		setScore(50);
		playerFish->increaseScore(0.5f);
	}
}

void MainGame::resume(Ref* pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/select.wav");
		auto prompt = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("exitPrompt");
		prompt->setVisible(false);

		Director::getInstance()->resume();
		if (isGameOver) {
			this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("gameOverPanel")->setVisible(true);
		}
		if (isInCountdown) {
			this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown")->setVisible(true);
		}
		if (isBiteMode) {
			this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("biteActionPanel")->setVisible(true);
		}
		if (isInNotification) {
			this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("staticNotificationPanel")->setVisible(true);
		}
	}
}

void MainGame::endingSequence1() {
	playerContactListener->setEnabled(false);
	this->runAction(Sequence::create(DelayTime::create(3.0),
		CallFunc::create([this]() {
			staticNotification("CONGRATULATIONS!");
		}), DelayTime::create(3.0),
			CallFunc::create([this]() {
			staticNotification("YOU'VE DEFEATED\nAN EVIL\nSHARK MONSTER!");
		}), DelayTime::create(3.0),
			CallFunc::create([this]() {
			staticNotification("BUT WAIT...");
		}), DelayTime::create(2.0),
			CallFunc::create([this]() {
			staticNotification("WHAT IS THAT??");
		}), DelayTime::create(2.0),
			CallFunc::create([this]() {
			staticNotification("OH NO!");
		}), DelayTime::create(2.0),
			CallFunc::create([this]() {
			staticNotification("LOOKS LIKE\nTHERE'S ANOTHER\nMONSTER\nTO DEFEAT!");
		}), DelayTime::create(3.0),
			CallFunc::create([this]() {
			staticNotification("THE LEGEND SAYS\nHE'S THE MOST\nVICIOUS CREATURE\nIN THE OCEAN!");
		}), DelayTime::create(3.0),
			CallFunc::create([this]() {
			staticNotification("HIS NAME IS...\nROSS?");
		}), DelayTime::create(3.0),
			CallFunc::create([this]() {
			this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("staticNotificationPanel")->setVisible(false);
			isInNotification = false;
			blinkNotification("GODDAMIT\nROSS!!");
			Director::getInstance()->resume();
			playerContactListener->setEnabled(true);
			auto bossTitle = static_cast<cocos2d::ui::Text*>(this->getChildByName("HUDElements")->
			getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("bossTitle"));
			bossTitle->setString("ROSS");
			bossTitle->runAction(Blink::create(4.0, 30));

			pActionTimeline->gotoFrameAndPlay(0, false);
			this->runAction(pActionTimeline);
			
			Ross* ross = Ross::create();
			ross->setName("rossNode");
			this->addChild(ross, 2);
			ross->drawFish();
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/boss_appear.wav");
			bossLimit = 70;
			if (UserDefault::getInstance()->getIntegerForKey("rossStage") < 1) {
				achievementUnlocked();
			}
			UserDefault::getInstance()->setIntegerForKey("rossStage", 1);
		}), nullptr
		));
}

void MainGame::endingSequence2() {
	playerContactListener->setEnabled(false);
	this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("continuePrompt")->setVisible(true);
	UserDefault::getInstance()->setBoolForKey("infiniteModeUnlocked", true);
}

void MainGame::staticNotification(std::string str) {
	isInNotification = true;
	auto notificationPanel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("staticNotificationPanel");
	auto notificationLabel = static_cast<cocos2d::ui::Text*>(notificationPanel->getChildByName("staticNotification"));
	notificationPanel->setVisible(true);
	notificationLabel->setString(str);
}

void MainGame::repeatCallback(Ref* pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/exit.wav");
		auto scene = MainGame::createScene();
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, scene));
	}
}

void MainGame::continueCallback(Ref* pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::BEGAN) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/select.wav");
		playerFish->removeFromParentAndCleanup(true);
		AdBuddizHelper::showAd();
		playerFish = PlayerFish::create();
		playerFish->setName("playerNode");
		this->addChild(playerFish, 2);
		playerFish->createFish(1);
		playerContactListener->setEnabled(true);
		playerFish->blink();
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("continuePrompt")->setVisible(false);
	}
}

void MainGame::exit(Ref* pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::BEGAN) {
		this->stopAllActions();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/select.wav");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/exit.wav");
		saveHiScore();
		auto scene = MainMenu::createScene();
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, scene));
		AdBuddizHelper::showAd();
	}
}