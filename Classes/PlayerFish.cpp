#include "PlayerFish.h"
#include "SimpleAudioEngine.h"
#include "MainGameScene.h"

USING_NS_CC;
void PlayerFish::createFish(int fishId) {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/evolve.wav");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	this->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	
	char str[100] = { 0 };
	sprintf(str, "mf%i%i.png", fishId, 1);
	auto spr = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName(str));
	//this->getBoundingBox().setRect(this->getPositionX(), this->getPositionY(), fishSprite->getContentSize().width * fScale, fishSprite->getContentSize().height * fScale);
	auto physicsBody = PhysicsBody::createBox(spr->getContentSize());
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);    // 0010
	physicsBody->setCollisionBitmask(0x09);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setTag(10);
	physicsBody->setName("PHDOBY");
	this->addComponent(physicsBody);

	this->getTexture()->setAliasTexParameters();
	drawFish(fishId);
}
void PlayerFish::drawFish(int fishId) {
	currentStage = fishId;
	if (fishId == 1) {
		score = 10 * fishId + 1.0f;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("mf11.png"));
	Vector<SpriteFrame*> animFrames(2);

	auto fnameStr = "mf%i%i.png";
	if (currentStage == 6 && cocos2d::rand_minus1_1() > 0) {
		fnameStr = "mf_%i%i.png";
		UserDefault::getInstance()->setIntegerForKey("playerStageFinal", 1);
	}
	else if (currentStage == 6) {	
		UserDefault::getInstance()->setIntegerForKey("playerStageFinal_", 1);
	}
	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, fnameStr, fishId, i);
		auto spr = CCSprite::create(str);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	/*char str[100] = { 0 };
	sprintf(str, "sprites/mf%i%i.png", fishId, 1);
	fScale = visibleSize.width / 15 / CCSprite::create("sprites/mf11.png")->getContentSize().width;
	*/
	fScale = visibleSize.width / 15 / CCSprite::create("sprites/mf11.png")->getContentSize().width;
	fishSprite->setName("FSPRITE");

	this->addChild(fishSprite);
	

	char str[100] = { 0 };
	sprintf(str, fnameStr, fishId, 1);
	auto newF = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName(str));
	this->setContentSize(newF->getContentSize());

	this->setScale(fScale);

	if (direction == 1) {
		this->setScaleX(-fScale);
	}
	else {
		this->setScaleX(fScale);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));

	
	/*
	auto physicsShape = PhysicsBody::createBox(Size(newF->getContentSize().width*fScale, newF->getContentSize().height*fScale), PhysicsMaterial(1, 1, 1), Vec2(0, 0));
	physicsShape->setDynamic(false);
	physicsShape->setCategoryBitmask(0x02);    // 0010
	physicsShape->setCollisionBitmask(0x09);
	physicsShape->setContactTestBitmask(0xFFFFFFFF);
	physicsShape->setTag(10);

	this->setPhysicsBody(physicsShape);
	*/
}

void PlayerFish::refreshPlayerDirection(int posX) {
	if (this->getPosition().x > posX) {
		direction = 1;
		this->setScaleX(-fScale);
	}
	else {
		direction = 0;
		this->setScaleX(fScale);
	}
}

void PlayerFish::evolve() {

	int stage = std::floor((score - 1.0f) / 10);
	if (stage > currentStage && stage <= 6) {
		//currentStage = stage;
		this->removeChildByName("FSPRITE");
		this->drawFish(stage);
		blink();
		UserDefault::getInstance()->setIntegerForKey("playerStage", stage);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/evolve.wav");
	}
}

void PlayerFish::increaseScale() {
	
	if (score <= 4 + currentStage * 10) {
		fScale += 0.025;

		this->setScale(fScale);
		if (direction == 1) {
			this->setScaleX(-fScale);
		}
		else {
			this->setScaleX(fScale);
		}
	}
}

PlayerFish* PlayerFish::create(const std::string &path, ssize_t capacity)
{
	PlayerFish *fish = new PlayerFish();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

PlayerFish* PlayerFish::create() {
	return PlayerFish::create("sprites/sp_main.png", 29L);
}

void PlayerFish::increaseScore(float pts) {
	score += pts;
	increaseScale();
	evolve();
}
void PlayerFish::blink() {
	ActionInterval *blink = CCBlink::create(2, 10);
	this->runAction(Sequence::create(CallFunc::create([this]() {
		blinking = true;
	}), blink, CallFunc::create([this]() {
		blinking = false;
	}), nullptr));
}
bool PlayerFish::isBlinking() {
	return blinking;
}
int PlayerFish::getScore() {
	return score;
}
int PlayerFish::getStage() {
	return currentStage;
}