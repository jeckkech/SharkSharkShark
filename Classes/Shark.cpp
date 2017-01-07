#include "Shark.h"
#include "Ross.h"
#include "MainGameScene.h"

USING_NS_CC;

void Shark::run() {

	if (Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this) == 1) {
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		Size fSpriteSize = this->getChildByName("SHARKSPR")->getContentSize();

		Point swimPoint;
		if (cocos2d::rand_minus1_1() > 0) {
			auto player = this->getParent()->getChildByName("playerNode");
			if (player != NULL) {
				swimPoint = player->getPosition();
			}
		}
		else {
			int positionX = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x - visibleSize.width),
				static_cast<int>(visibleSize.width * 2 + origin.x));
			int positionY = cocos2d::RandomHelper::random_int(static_cast<int>(origin.y), static_cast<int>(visibleSize.height + origin.y));
			//CCLOG("LOGGED UPDATE POSITION CALL x:%i y:%i", positionX, positionY);
			swimPoint = Point(positionX, positionY);
		}

		if (this->getPositionX() > swimPoint.x) {
			this->setScaleX(fScale);
			this->getPhysicsBody()->setPositionOffset(Vec2(-fSpriteSize.width/3, -fSpriteSize.height / 6));
		}
		else {
			this->setScaleX(-fScale);
			this->getPhysicsBody()->setPositionOffset(Vec2(fSpriteSize.width/3, -fSpriteSize.height / 6));
		}
		this->setScaleY(fScale);
		float swimTime = cocos2d::RandomHelper::random_real(0.075, 0.035);
		
		auto action = MoveTo::create(swimTime * visibleSize.height, swimPoint);
		this->runAction(action);
	}
}

void Shark::drawFish() {
	score = 100000;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("shk11.png"));
	fishSprite->setName("SHARKSPR");
	Vector<SpriteFrame*> animFrames(2);

	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, "shk1%i.png", i);
		auto spr = CCSprite::create(str);

		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	fScale = visibleSize.width / 2 / fishSprite->getContentSize().width;

	this->addChild(fishSprite);

	int positionX;
	int positionXNeg = origin.x - visibleSize.width;
	int positionXPos = origin.x + visibleSize.width * 2;
	int dirSwitcher = cocos2d::RandomHelper::random_int(static_cast<int>(0), static_cast<int>(1));
	if (dirSwitcher < 0.5) {
		positionX = positionXNeg;
	}
	else {
		positionX = positionXPos;
	}
	int positionY = origin.y + visibleSize.height / 4 * 3 - fishSprite->getContentSize().height*fScale * 2;
	initialY = positionY;
	this->setAnchorPoint(Point(0, 0));
	this->setScale(fScale);
	this->setPosition(positionX, positionY);
	
	Vec2 pts[8] = { Vec2(0, 13),Vec2(10, 19),Vec2(25, 22),Vec2(29, 27),	Vec2(26, 2),Vec2(18, 6),Vec2(6, 9),	Vec2(0, 13) };
	auto physicsBody = PhysicsBody::createPolygon(pts, 8, PhysicsMaterial(1, 1, 1), Vec2(-17, -13));

	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x14);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setTag(10);
	
	auto shape = PhysicsShapeBox::create(Size(fishSprite->getContentSize().width/2, fishSprite->getContentSize().height / 2), PhysicsMaterial(1, 1, 1), Vec2(fishSprite->getContentSize().width/4*2, 0), 0.0);
	shape->setCollisionBitmask(0x15);
	shape->setCategoryBitmask(0x02);
	shape->setContactTestBitmask(0xFFFFFFFF);

	physicsBody->addShape(shape);
	this->setPhysicsBody(physicsBody);
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));
	this->getTexture()->setAliasTexParameters();

	this->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create(std::bind(&Shark::run, this)),
		nullptr)));
}

void Shark::increaseScale() {
	/*fScale += 0.05;
	float scaleX = this->getScaleX();
	if (scaleX > 0) {
	scaleX += 0.05;
	}
	else {
	scaleX -= 0.05;
	}
	this->setScaleX(scaleX);
	this->setScaleY(fScale);*/

}

Shark* Shark::create(const std::string &path, ssize_t capacity)
{
	Shark *fish = new Shark();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

void Shark::kill() {
	this->stopAllActions();
	this->removeAllChildrenWithCleanup(true);
	this->getPhysicsBody()->setEnabled(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto klSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("kl1.png"));
	Vector<SpriteFrame*> animFrames(8);

	for (int i = 1; i <= 8; i++) {
		char str[100] = { 0 };
		sprintf(str, "kl%i.png", i);
		auto spr = CCSprite::create(str);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);

	this->addChild(klSprite);
	klSprite->runAction(Sequence::create(Animate::create(animation), CallFunc::create([this]() {
		this->removeFromParentAndCleanup(true);
	}), nullptr));
}

Shark* Shark::create() {
	return Shark::create("sprites/sp_main.png", 29L);
}

void Shark::increaseScore(float pts) {
	++score;
}

void Shark::decreaseHealth() {
	--health;
}

int Shark::getHealth() {
	return health;
}

int Shark::getScore() {
	return score;
}