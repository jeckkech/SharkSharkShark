#include "SimpleAudioEngine.h"
#include "MainGameScene.h"
#include "Fish.h"
#include "PlayerFish.h"
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

	auto background = CCSprite::create("bg1.png");
	background->setAnchorPoint(Point(0, 0));
	background->setPosition(Point(origin.x, origin.y));
	background->getTexture()->setAliasTexParameters();

	parallaxBg2 = CCSprite::create("bg2.png");
	parallaxBg2->setAnchorPoint(Point(0, 0));
	parallaxBg2->setPosition(Point(origin.x, origin.y));
	parallaxBg2->getTexture()->setAliasTexParameters();

	parallaxBg = CCSprite::create("bg3.png");
	parallaxBg->setAnchorPoint(Point(0, 0));
	parallaxBg->setPosition(Point(origin.x, origin.y));
	parallaxBg->getTexture()->setAliasTexParameters();

	float bgScale = visibleSize.height / background->getContentSize().height;
	float bgScale2 = visibleSize.width / parallaxBg2->getContentSize().width;
	background->setScale(bgScale);
	this->addChild(background, 1);

	parallaxBg2->setScale(bgScale2);
	this->addChild(parallaxBg2, 2);

	parallaxBg->setScale(bgScale2);
	this->addChild(parallaxBg, 3);

	auto node = CSLoader::createNode("HUDElements.csb");
	
	node->setAnchorPoint(Point(0, 0));
	node->setPosition(origin.x, origin.y);
	
	float scaleSizeX = visibleSize.width / node->getContentSize().width;
	float scaleSizeY = visibleSize.height / node->getContentSize().height;

	node->setContentSize(visibleSize);
	auto panel = node->getChildByName("HUDElements")->getChildByName("Panel");

	auto bossLifebar = static_cast<CCSprite*>(panel->getChildByName("hb11_12"));
	bossLifebar->getTexture()->setAliasTexParameters();

	auto exitButton = static_cast<cocos2d::ui::Button*>(panel->getChildByName("exitBtn"));

	CCSprite::create(exitButton->getNormalFile().file)->getTexture()->setAliasTexParameters();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->setEnabled(true);
	
	exitButton->addTouchEventListener(this, toucheventselector(MainGame::menuCloseCallback));

	panel->setScaleX(visibleSize.width / panel->getContentSize().width);
	panel->setScaleY(visibleSize.height / panel->getContentSize().height);

	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel = static_cast<cocos2d::ui::Text*>(panel->getChildByName("scoreLabel"));
	scoreLabel->setString(tempScore->getCString());

	ui::Helper::doLayout(node);
	this->addChild(node, 5);

	Fish* fish1 = Fish::create();
	this->addChild(fish1, 2);
	fish1->drawFish(1);
	//fishList.push_front(fish1);

	Fish* fish2 = Fish::create();
	this->addChild(fish2, 2);
	fish2->drawFish(2);
	//fishList.push_front(fish2);

	Fish* fish3 = Fish::create();
	this->addChild(fish3, 2);
	fish3->drawFish(3);
	//fishList.push_front(fish3);

	Fish* fish4 = Fish::create();
	this->addChild(fish4, 2);
	fish4->drawFish(4);
	//fishList.push_front(fish4);

	Fish* fish5 = Fish::create();
	this->addChild(fish5, 2);
	fish5->drawFish(5);

	Fish* fish6 = Fish::create();
	this->addChild(fish6, 2);
	fish6->drawFish(1);

	Fish* fish7 = Fish::create();
	this->addChild(fish7, 2);
	fish7->drawFish(3);

	Fish* fish8 = Fish::create();
	this->addChild(fish8, 2);
	fish8->drawFish(1);
	//fishList.push_front(fish5);
	
	this->scheduleUpdate();
	
	playerFish = PlayerFish::create();
	this->addChild(playerFish, 2);
	playerFish->setColor(Color3B(0, 225, 0));
	playerFish->createFish(1);
	
	playerTouchListener = EventListenerTouchOneByOne::create();
	playerTouchListener->setSwallowTouches(true);
	playerTouchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchMove, this);
	playerTouchListener->onTouchMoved = CC_CALLBACK_2(MainGame::onTouchMove, this);
	playerTouchListener->onTouchEnded = CC_CALLBACK_2(MainGame::onTouchStop, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerTouchListener, this);

	playerContactListener = EventListenerPhysicsContact::create();
	playerContactListener->onContactBegin = CC_CALLBACK_1(MainGame::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerContactListener, this);
	return true;
}

void MainGame::update(float dt) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float playerFishPos = 0;
	
	playerFishPos += playerFish == NULL ? 0 : playerFish->getPositionX();
	
	float bgMargin = visibleSize.width / 2 - playerFishPos + origin.x;
	
	parallaxBg->setPositionX(origin.x + bgMargin/2);
	parallaxBg2->setPositionX(origin.x + bgMargin / 5);
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
	bool userCollided = false;
	if(contact.getShapeA() && contact.getShapeB() && 
		contact.getShapeA()->getBody()->getCollisionBitmask() != 0x04 && contact.getShapeB()->getCollisionBitmask() != 0x04){
		Fish* node1 = (Fish*)contact.getShapeA()->getBody()->getNode();
		Fish* node2 = (Fish*)contact.getShapeB()->getBody()->getNode();

		if (node1 && node2 && (node1->getScore() == node2->getScore() || node1->getScore() > node2->getScore())) {
			if (contact.getShapeA()->getCollisionBitmask() == 0x09) {
				setScore(100);
				((PlayerFish*)contact.getShapeA()->getBody()->getNode())->increaseScore(1.0f);
				userCollided = true;
			}
			else if (contact.getShapeB()->getCollisionBitmask() == 0x09) {
				this->unscheduleUpdate();
				scoreLabel->setString("GAME OVER");
				this->getEventDispatcher()->removeEventListener(playerTouchListener);
				this->getEventDispatcher()->removeEventListener(playerContactListener);
				userCollided = true;
			}
			if (contact.getShapeA()->getCollisionBitmask() != 0x09) {
				node1->increaseScore(1.0);
				node1->increaseScale();
			}

			position = node2->getPosition();
			node2->kill(userCollided);

		}
		else if(node1 && node2){
			if (contact.getShapeB()->getCollisionBitmask() == 0x09) {
				setScore(100);
				((PlayerFish*)contact.getShapeB()->getBody()->getNode())->increaseScore(1.0);
				userCollided = true;
			}
			else if (contact.getShapeA()->getCollisionBitmask() == 0x09) {
				this->unscheduleUpdate();
				scoreLabel->setString("GAME OVER");
				this->getEventDispatcher()->removeEventListener(playerTouchListener);
				this->getEventDispatcher()->removeEventListener(playerContactListener);
				userCollided = true;
			}
			if (contact.getShapeB()->getCollisionBitmask() != 0x09) {
				node2->increaseScore(1.0);
				node2->increaseScale();
			}
			position = node1->getPosition();
			node1->kill(userCollided);
		}
		Fish* fish5 = Fish::create();
		this->addChild(fish5, 2);
		fish5->drawFish(cocos2d::RandomHelper::random_int(1, 5));

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
	} else if (contact.getShapeA() && contact.getShapeB()) {
		if (contact.getShapeA()->getBody()->getCollisionBitmask() == 0x09) {
			((PlayerFish*)contact.getShapeA()->getBody()->getNode())->increaseScore(0.2f);
			setScore(20);
			((Skeleton*)contact.getShapeB()->getBody()->getNode())->destroy();
		}
		else if (contact.getShapeB()->getBody()->getCollisionBitmask() == 0x09) {
			((PlayerFish*)contact.getShapeB()->getBody()->getNode())->increaseScore(0.2f);
			setScore(20);
			((Skeleton*)contact.getShapeA()->getBody()->getNode())->destroy();
			//contact.getShapeA()->getBody()->getNode()->removeFromParentAndCleanup(true);
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
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
