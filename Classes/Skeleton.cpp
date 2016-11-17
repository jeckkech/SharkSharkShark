#include "Skeleton.h"

USING_NS_CC;

Skeleton* Skeleton::create(const std::string &path)
{
	Skeleton *fishSkeleton = new Skeleton();
	if (fishSkeleton && fishSkeleton->initWithFile(path))
	{
		// Set to autorelease
		fishSkeleton->autorelease();

		return fishSkeleton;
	}
	CC_SAFE_DELETE(fishSkeleton);
	return NULL;
}

void Skeleton::run(Vec2 position, float scaleX, float scaleY) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->setScaleX(scaleX);
	this->setScaleY(scaleY);
	this->setPosition(position);
	//this->getBoundingBox().setRect(this->getPositionX(), this->getPositionY(), fishSprite->getContentSize().width * fScale, fishSprite->getContentSize().height * fScale);
	auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(1.0f, 1.0f, 1.0f));

	physicsBody->setDynamic(true);
	physicsBody->setRotationEnable(true);
	physicsBody->setCollisionBitmask(0x04);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setEnabled(true);
	this->setPhysicsBody(physicsBody);
	this->getTexture()->setAliasTexParameters();
}

void Skeleton::destroy() {
	this->setOpacity(0);
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
	float scale = visibleSize.width / 15 / CCSprite::create("sprites/kl1.png")->getContentSize().width;
	this->setAnchorPoint(Point(0, 0));
	this->setScale(scale);
	this->addChild(klSprite);
	klSprite->getTexture()->setAliasTexParameters();
	klSprite->runAction(Sequence::create(Animate::create(animation), CallFunc::create([this]() {
		this->removeFromParentAndCleanup(true);
	}), nullptr));
}

int Skeleton::getScore() {
	return 0;
}