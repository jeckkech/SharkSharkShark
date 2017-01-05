#include "SeaHorse.h"
#include "Skeleton.h"

USING_NS_CC;

void SeaHorse::run() {
	if (Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this) == 1) {
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		int positionX = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x),
			static_cast<int>(visibleSize.width + origin.x));
		int positionY = cocos2d::RandomHelper::random_int(static_cast<int>(origin.y), static_cast<int>(visibleSize.height + origin.y));
		//CCLOG("LOGGED UPDATE POSITION CALL x:%i y:%i", positionX, positionY);
		if (this->getPositionX() > positionX) {
			this->setScaleX(fScale);
		}
		else {
			this->setScaleX(-fScale);
		}
		this->setScaleY(fScale);

		Point moveToPoint = Point(positionX, positionY);
		float distance = this->getPosition().getDistance(moveToPoint);
		auto action = MoveTo::create(distance/50, moveToPoint);
		this->runAction(action);
	}
}



void SeaHorse::kill(bool withoutSkeleton) {
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
	klSprite->runAction(Sequence::create(Animate::create(animation), CallFunc::create([this, withoutSkeleton]() {
		if (!withoutSkeleton) {
			Skeleton* skeleton = Skeleton::create("sprites/sk6.png");
			this->getParent()->addChild(skeleton, 2);
			this->getPhysicsBody()->setCollisionBitmask(0x01);
			skeleton->run(this->getPosition(), this->getScaleX(), this->getScaleY());
		}
		this->removeFromParentAndCleanup(true);
	}), nullptr));
}

void SeaHorse::drawFish(int fishId) {
	score = fishId * 10;
	currentStage = fishId;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("f11.png"));
	Vector<SpriteFrame*> animFrames(2);

	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, "sh%i%i.png", fishId, i);
		auto spr = CCSprite::create(str);

		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	fScale = visibleSize.width / 15 / CCSprite::create("sprites/f11.png")->getContentSize().width;

	this->addChild(fishSprite);

	int positionX;
	int positionXNeg = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x - visibleSize.width / 3),
		static_cast<int>(origin.x));
	int positionXPos = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x + visibleSize.width),
		static_cast<int>(origin.x + visibleSize.width + visibleSize.width / 3));
	int dirSwitcher = cocos2d::RandomHelper::random_int(static_cast<int>(0), static_cast<int>(1));
	if (dirSwitcher < 0.5) {
		positionX = positionXNeg;
	}
	else {
		positionX = positionXPos;
	}

	int positionY = cocos2d::RandomHelper::random_int(static_cast<int>(origin.y), static_cast<int>(visibleSize.height + origin.y));

	this->setScale(fScale);
	this->setPosition(positionX, positionY);
	auto physicsBody = PhysicsBody::createBox(fishSprite->getContentSize());
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x08);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setTag(10);

	this->setPhysicsBody(physicsBody);

	this->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create(std::bind(&SeaHorse::run, this)),
		nullptr)));

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));
	this->getTexture()->setAliasTexParameters();
}

void SeaHorse::increaseScale() {
	fScale += 0.05;
	float scaleX = this->getScaleX();
	if (scaleX > 0) {
		scaleX += 0.05;
	}
	else {
		scaleX -= 0.05;
	}
	this->setScaleX(scaleX);
	this->setScaleY(fScale);

}

SeaHorse* SeaHorse::create(const std::string &path, ssize_t capacity)
{
	SeaHorse *fish = new SeaHorse();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

SeaHorse* SeaHorse::create() {
	return SeaHorse::create("sprites/sp_main.png", 29L);
}

void SeaHorse::increaseScore(float pts) {
	score += pts;
	//this->evolve();
}

int SeaHorse::getScore() {
	return score;
}