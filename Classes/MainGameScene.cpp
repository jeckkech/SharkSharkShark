#include "SimpleAudioEngine.h"
#include "MainGameScene.h"
#include "Fish.h"
#include "PlayerFish.h"
#include "Skeleton.h"

USING_NS_CC;

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
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainGame::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	

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
	
	__String *tempScore = __String::createWithFormat("%i", totalScore);
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Gamegirl.ttf", visibleSize.height * 0.05);
	scoreLabel->setColor(Color3B(225, 0, 0));
	scoreLabel->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y - scoreLabel->getContentSize().height);
	this->addChild(scoreLabel, 3);

	cocos2d::EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchMove, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MainGame::onTouchMove, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainGame::onTouchStop, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MainGame::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
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
				this->getEventDispatcher()->removeAllEventListeners();
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
				this->getEventDispatcher()->removeAllEventListeners();
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
