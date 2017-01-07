#include "Ross.h"
#include "Bubble.h"
#include "MainGameScene.h"

USING_NS_CC;

void Ross::run() {
	if (Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this) == 1) {
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto pos1 = Vec2(cocos2d::RandomHelper::random_int(static_cast<int>(origin.x + this->getContentSize().width/2),
			static_cast<int>(visibleSize.width + origin.x - this->getContentSize().width / 2)), cocos2d::RandomHelper::random_int(
				static_cast<int>(origin.y + this->getContentSize().height / 2),
				static_cast<int>(visibleSize.height + origin.y - this->getContentSize().height / 2)));
		auto pos2 = Vec2(cocos2d::RandomHelper::random_int(static_cast<int>(origin.x + this->getContentSize().width / 2),
			static_cast<int>(visibleSize.width + origin.x - this->getContentSize().width / 2)), cocos2d::RandomHelper::random_int(
				static_cast<int>(origin.y + this->getContentSize().height / 2),
				static_cast<int>(visibleSize.height + origin.y - this->getContentSize().height / 2)));
		auto pos3 = Vec2(cocos2d::RandomHelper::random_int(static_cast<int>(origin.x + this->getContentSize().width / 2),
			static_cast<int>(visibleSize.width + origin.x - this->getContentSize().width / 2)), cocos2d::RandomHelper::random_int(
				static_cast<int>(origin.y + this->getContentSize().height / 2),
				static_cast<int>(visibleSize.height + origin.y - this->getContentSize().height / 2)));
		auto action1 = MoveTo::create(std::abs(pos1.getDistance(this->getPosition()) / 25), pos1);
		auto action2 = MoveTo::create(std::abs(pos2.getDistance(this->getPosition()) / 25), pos2);
		auto action3 = MoveTo::create(std::abs(pos3.getDistance(this->getPosition()) / 25), pos3);
		auto homePosition = Vec2(visibleSize.width / 2, visibleSize.height / 3 * 2);

		this->runAction(Sequence::create(
			MoveTo::create(std::abs(homePosition.getDistance(this->getPosition()) / 25), homePosition),
			CallFunc::create([this, visibleSize, origin]() {
			isAngryMode = true;
			switchMode(true);
			this->getPhysicsBody()->getShape(150)->setCollisionBitmask(0x14);
			projectilesSequence(visibleSize, origin);
		}), DelayTime::create(5.0),
			CallFunc::create([this, visibleSize, origin]() {
			projectilesSequence(visibleSize, origin);
		}), DelayTime::create(5.0),
			CallFunc::create([this, visibleSize, origin]() {
			switchMode(false);
			this->getPhysicsBody()->getShape(150)->setCollisionBitmask(0x15);
		}), action1, action2, action3, MoveTo::create(std::abs(homePosition.getDistance(pos3) / 25), homePosition),
			nullptr));
}
}

void Ross::drawFish() {
	score = 100000;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	switchMode(false);
	this->setScale(fScale);
	this->setPosition(origin.x - visibleSize.width, visibleSize.height / 3 * 2);

	auto physicsBody = PhysicsBody::createCircle(fishSprite->getContentSize().width / 5, 
		PhysicsMaterial(1, 1, 1), Vec2(0.0f, fishSprite->getContentSize().height / 2.75 - fishSprite->getContentSize().height / 5));

	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x14);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setTag(10);

	auto pinfo = AutoPolygon("sprites/or1.png");
	Rect rect = Rect(0, 0, 105, 66);
	std::vector<Vec2> vertices = pinfo.trace(rect);
	Vec2 pts[1000];
	std::copy(vertices.begin(), vertices.end(), pts);

	auto shape = PhysicsShapePolygon::create(pts, vertices.size(), PhysicsMaterial(1, 1, 1), 
		Vec2(-fishSprite->getContentSize().width/2, -fishSprite->getContentSize().height/2));
	
	shape->setCollisionBitmask(0x15);
	shape->setCategoryBitmask(0x02);
	shape->setTag(150);
	shape->setContactTestBitmask(0xFFFFFFFF);

	physicsBody->addShape(shape);
	this->setPhysicsBody(physicsBody);

	this->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create(std::bind(&Ross::run, this)),
		nullptr)));
}

void Ross::switchMode(bool isAngry) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/boss.plist");

	
	fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("or1.png"));
	fishSprite->setName("RBOSS");
	 
	
	Vector<SpriteFrame*> animFrames(2);

	auto fnameStr = "or%i.png";
	//this->getPhysicsBody()->getShape(150)->setCollisionBitmask(0x14);
	if (isAngry) {
		fnameStr = "aor%i.png";
		//this->getPhysicsBody()->getShape(150)->setCollisionBitmask(0x14);
	} 

	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, fnameStr, i); //aor
		auto spr = CCSprite::create(str);

		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	fScale = visibleSize.width / 1.5 / fishSprite->getContentSize().width;

	this->removeChildByName("RBOSS");
	this->addChild(fishSprite);

	fishSprite->stopAllActions();

	auto animation = Animation::createWithSpriteFrames(animFrames, isAngry ? 0.2f : 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));
	this->getTexture()->setAliasTexParameters();
}

Ross* Ross::create(const std::string &path, ssize_t capacity)
{
	Ross *fish = new Ross();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

void Ross::kill() {
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

Ross* Ross::create() {
	return Ross::create("sprites/boss.png", 29L);
}

void Ross::decreaseHealth() {
	--health;
}

void Ross::shootProjectiles(Vec2 position) {
	Bubble* bubble = Bubble::create("sprites/bbl11.png");
	this->getParent()->addChild(bubble, 2);
	bubble->run(this->getPosition());
	bubble->runAction(Sequence::create(MoveTo::create(std::abs(position.getDistance(this->getPosition())/45), position), 
		CallFunc::create([bubble]() {
		bubble->removeFromParentAndCleanup(true);
	}), nullptr));
}

void Ross::projectilesSequence(Size visibleSize, Vec2 origin) {
	shootProjectiles(Vec2(-1, -1));
	shootProjectiles(Vec2(visibleSize.width, -1));
	shootProjectiles(Vec2(visibleSize.width / 3, -1));
	shootProjectiles(Vec2(visibleSize.width / 3 * 2, -1));
	//shootProjectiles(Vec2(visibleSize.width /4*3, -1));
	//shootProjectiles(Vec2(-1, visibleSize.height/4));
	shootProjectiles(Vec2(-1, visibleSize.height / 4 * 2));
	shootProjectiles(Vec2(-1, visibleSize.height / 4 * 3));
	shootProjectiles(Vec2(-1, visibleSize.height));
	//shootProjectiles(Vec2(visibleSize.width + origin.x, visibleSize.height / 4));
	shootProjectiles(Vec2(visibleSize.width + origin.x, visibleSize.height / 4 * 2));
	shootProjectiles(Vec2(visibleSize.width + origin.x, visibleSize.height / 4 * 3));
	shootProjectiles(Vec2(visibleSize.width + origin.x, visibleSize.height));
}

int Ross::getHealth() {
	return health;
}