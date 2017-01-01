#include "SimpleAudioEngine.h"
#include "MainGameScene.h"
#include "MainMenuScene.h"
#include "Fish.h"
#include "SeaHorse.h"
#include "Cancer.h"
#include "JellyFish.h"
#include "PlayerFish.h"
#include "Shark.h"
#include "Skeleton.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;

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

	// return the scene
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
	parallaxBg->setAnchorPoint(Point(0, 0));
	parallaxBg->setPosition(Point(origin.x, origin.y));
	parallaxBg->getTexture()->setAliasTexParameters();

	//float bgScale = visibleSize.height / background->getContentSize().height;
	float bgScale2 = visibleSize.width / parallaxBg2->getContentSize().width;
	//background->setScale(bgScale);
	//this->addChild(background, 1);

	parallaxBg2->setScale(bgScale2);
	this->addChild(parallaxBg2, 2);

	parallaxBg->setScale(bgScale2);
	this->addChild(parallaxBg, 3);

	auto node = CSLoader::createNode("HUDElements.csb");
	
	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	node->setName("HUDElements");


	node->setContentSize(visibleSize);
	auto panel = node->getChildByName("HUDElements")->getChildByName("hudPanel");
	Sprite::create("res/lifebar.png")->getTexture()->setAliasTexParameters();

	auto background = CSLoader::createNode("res/background.csb");
	auto bgNode = background->getChildByName("background");
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(origin.x, origin.y);
	background->setContentSize(visibleSize);
	bgNode->setScaleX(visibleSize.width / bgNode->getContentSize().width);
	bgNode->setScaleY(visibleSize.height / bgNode->getContentSize().height);
	cocostudio::timeline::ActionTimeline* pActionTimeline = CSLoader::createTimeline("res/background.csb");
	pActionTimeline->gotoFrameAndPlay(0);
	bgNode->runAction(pActionTimeline);
	this->addChild(background, 0);

	auto exitButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("hudBlockPanel")->getChildByName("exitBtn"));

	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->setEnabled(true);
	
	exitButton->addTouchEventListener(this, toucheventselector(MainGame::menuCloseCallback));

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	gameOverLabel = static_cast<cocos2d::ui::Text*>(panel->getChildByName("gameOverLabel"));
	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel = static_cast<cocos2d::ui::Text*>(panel->getChildByName("scoreLabel"));
	scoreLabel->setString(tempScore->getCString());

	ui::Helper::doLayout(node);
	this->addChild(node, 5);

	this->scheduleUpdate();
	
	playerFish = PlayerFish::create();
	this->addChild(playerFish, 2);
	playerFish->setColor(Color3B(0, 225, 0));
	playerFish->createFish(1);
	
	playerContactListener = EventListenerPhysicsContact::create();
	playerContactListener->onContactBegin = CC_CALLBACK_1(MainGame::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerContactListener, this);
	return true;
}

void MainGame::lifebarInitialize() {
	auto panel = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel");
	auto bossLifebar = static_cast<Node*>(panel->getChildByName("lifebar"));

	cocostudio::timeline::ActionTimeline* pActionTimeline = (cocostudio::timeline::ActionTimeline*)bossLifebar->getActionByTag(bossLifebar->getTag());
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
		}), pause, CallFunc::create([countdownLabel]() {
		countdownLabel->setString("SET");
		}), pause->clone(),
		CallFuncN::create(std::bind(&MainGame::setUpInitial, this)),
		CallFunc::create([countdownLabel]() {
		countdownLabel->setString("GO!");
		ActionInterval *blink = CCBlink::create(2, 16);
		countdownLabel->runAction(blink);
		}), DelayTime::create(2.0),
		CallFunc::create([this, countdownPanel]() {
		countdownPanel->setVisible(false);
		isInCountdown = false;
		}),nullptr));
}

void MainGame::setUpInitial(){

	Fish* fish1 = Fish::create();
	this->addChild(fish1, 2);
	fish1->drawFish(1);
	//fishList.push_front(fish1);

	Fish* fish2 = Fish::create();
	this->addChild(fish2, 2);
	fish2->drawFish(1);
	//fishList.push_front(fish2);

	Fish* fish3 = Fish::create();
	this->addChild(fish3, 2);
	fish3->drawFish(2);
	//fishList.push_front(fish3);

	Fish* fish5 = Fish::create();
	this->addChild(fish5, 2);
	fish5->drawFish(3);

	SeaHorse* horse = SeaHorse::create();
	this->addChild(horse, 2);
	horse->drawFish(1);

	playerTouchListener = EventListenerTouchOneByOne::create();
	playerTouchListener->setSwallowTouches(true);
	playerTouchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchMove, this);
	playerTouchListener->onTouchMoved = CC_CALLBACK_2(MainGame::onTouchStop, this);
	playerTouchListener->onTouchEnded = CC_CALLBACK_2(MainGame::onTouchStop, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerTouchListener, this);
}
void MainGame::update(float dt) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float playerFishPos = 0;
	
	playerFishPos += playerFish == NULL ? 0 : playerFish->getPositionX();
	
	float bgMargin = visibleSize.width / 2 - playerFishPos + origin.x;
	
	parallaxBg->setPositionX(origin.x + bgMargin/2);
	parallaxBg2->setPositionX(origin.x + bgMargin / 5);

	

	if (totalScore >= 500 && !hwStage1) {
		lifebarInitialize();
		hwStage1 = true;

		Shark* shark = Shark::create();
		this->addChild(shark, 2);
		shark->drawFish();

		Cancer* cancer = Cancer::create();
		this->addChild(cancer, 2);
		cancer->drawFish(2);

		Fish* fish6 = Fish::create();
		this->addChild(fish6, 2);
		fish6->drawFish(3);
	}

	if (totalScore >= 1000 && !stage1) {
		stage1 = true;
		Fish* fish6 = Fish::create();
		this->addChild(fish6, 2);
		fish6->drawFish(4);
		++topStage;
	}

	if(totalScore >= 1500 && !hwStage2){
		hwStage2 = true;
		Fish* fish6 = Fish::create();
		this->addChild(fish6, 2);
		fish6->drawFish(5);
		++topStage;
	
		JellyFish* jf = JellyFish::create();
		this->addChild(jf, 2);
		jf->drawFish(1);
	}

}

bool MainGame::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event) {
	playerFish->refreshPlayerDirection(touch->getLocation().x);
	playerFish->stopActionByTag(1444);
	float distance = playerFish->getPosition().getDistance(touch->getLocation());
	auto action = MoveTo::create(distance/15, touch->getLocation());
	action->setTag(1444);
	playerFish->runAction(action);
}

bool MainGame::onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event) {
	//playerFish->stopActionByTag(1444);
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
	
	if (collisionBitmaskA == 21 || collisionBitmaskB == 21) {
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
					}
					else {
						setScore(100);
						((PlayerFish*)bodyA->getNode())->increaseScore(1.0f);
					}
					
					userCollided = true;
				}
				else if (collisionBitmaskB == 0x09) {
					this->unscheduleUpdate();
					gameOverLabel->setVisible(true);
					this->getEventDispatcher()->removeEventListener(playerTouchListener);
					this->getEventDispatcher()->removeEventListener(playerContactListener);
					userCollided = true;
					gameOver = true;
				}
				if (collisionBitmaskA != 0x09 && bodyA->getCollisionBitmask() != 0x06) {
					node1->increaseScore(1.0);
					node1->increaseScale();
				}

				position = node2->getPosition();

				if (node2->getPositionX() < origin.x || bodyB->getCollisionBitmask() == 0x06 || 
					bodyA->getCollisionBitmask() == 0x06) {
					userCollided = true;
				}
				if (bodyB->getCollisionBitmask() == 0x08) {
					((SeaHorse*)bodyB->getNode())->kill(userCollided);
				}
				else {
					node2->kill(userCollided);
				}
			}
			else if (node1 && node2) {
				if (collisionBitmaskB == 0x09) {
					if (collisionBitmaskA == 0x08) {
						setScore(200);
						((PlayerFish*)bodyB->getNode())->increaseScore(2.0);
					}
					else {
						setScore(100);
						((PlayerFish*)bodyB->getNode())->increaseScore(1.0);
					}
					
					
					userCollided = true;
				}
				else if (collisionBitmaskA == 0x09) {
					this->unscheduleUpdate();
					gameOverLabel->setVisible(true);
					this->getEventDispatcher()->removeEventListener(playerTouchListener);
					this->getEventDispatcher()->removeEventListener(playerContactListener);
					userCollided = true;
					gameOver = true;
				}
				if (collisionBitmaskB != 0x09 && bodyB->getCollisionBitmask() != 0x06) {
					node2->increaseScore(1.0);
					node2->increaseScale();
				}
				position = node1->getPosition();
				if (node1->getPositionX() < origin.x || bodyA->getCollisionBitmask() == 0x06 || 
					bodyB->getCollisionBitmask() == 0x06) {
					userCollided = true;
				}
				if (bodyA->getCollisionBitmask() == 0x08) {
					((SeaHorse*)bodyA->getNode())->kill(userCollided);
				}
				else {
					node1->kill(userCollided);
				}

			}
			int newFishId = cocos2d::RandomHelper::random_int(1, topStage);
			int fishRandomizer = cocos2d::RandomHelper::random_int(static_cast<int>(0), static_cast<int>(10));

			if(fishRandomizer == 1 && newFishId == 1){
				SeaHorse* sh = SeaHorse::create();
				this->addChild(sh, 2);
				sh->drawFish(1);
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
				((Skeleton*)bodyB->getNode())->destroy();
			}
			else if (collisionBitmaskB == 0x09) {
				((PlayerFish*)bodyB->getNode())->increaseScore(0.2f);
				setScore(20);
				((Skeleton*)bodyA->getNode())->destroy();
			}
		}
	}
}

void MainGame::setScore(int points) {
	totalScore = totalScore + points;
	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel->setString(tempScore->getCString());
}

void MainGame::menuCloseCallback(Ref* pSender)
{
	gameOverLabel->setVisible(false);
	this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown")->setVisible(false);
	auto prompt = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("exitPrompt");
	prompt->setVisible(true);
	Director::getInstance()->pause();
	//Director::getInstance()->pause();
	static_cast<Sprite*>(prompt->getChildByName("promptExit"))->getTexture()->setAliasTexParameters();

	auto exitButton = static_cast<cocos2d::ui::Button*>(prompt->getChildByName("promptButtonYes"));
	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	exitButton->addTouchEventListener(this, toucheventselector(MainGame::exit));

	auto resumeButton = static_cast<cocos2d::ui::Button*>(prompt->getChildByName("promptButtonNo"));
	CCSprite::create(resumeButton->getNormalFile().file)->getTexture()->setAliasTexParameters();
	resumeButton->addTouchEventListener(this, toucheventselector(MainGame::resume));
}

void MainGame::resume(Ref* pSender) {
	auto prompt = this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("exitPrompt");
	prompt->setVisible(false);

	Director::getInstance()->resume();
	if (gameOver) {
		gameOverLabel->setVisible(true);
	}
	if (isInCountdown) {
		this->getChildByName("HUDElements")->getChildByName("HUDElements")->getChildByName("hudPanel")->getChildByName("countdown")->setVisible(true);
	}
}

void MainGame::exit(Ref* pSender) {
	auto scene = MainMenu::createScene();
	Director::getInstance()->resume();
	Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, scene));
}