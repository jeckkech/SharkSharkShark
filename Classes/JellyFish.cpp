#include "JellyFish.h"

USING_NS_CC;

void JellyFish::run() {
	if (Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this) == 1) {
			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();
			int positionX = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x),
				static_cast<int>(visibleSize.width + origin.x));
			int positionY = origin.y + this->getContentSize().height * fScale;
			//CCLOG("LOGGED UPDATE POSITION CALL x:%i y:%i", positionX, positionY);
			if (this->getPositionX() > positionX) {
				this->setScaleX(-fScale);
			}
			else {
				this->setScaleX(fScale);
			}
			this->setScaleY(fScale);
			float swimTime = cocos2d::RandomHelper::random_real(0.075, 0.035);
			if (this) {
				float xPath = positionX - this->getPosition().x;
				//current x + xPath

				ccBezierConfig bezier;
				bezier.controlPoint_1 = Vec2(this->getPosition().x + xPath / 4, initialY + visibleSize.height / 4);
				bezier.controlPoint_2 = Vec2(this->getPosition().x + xPath / 4 * 2, initialY - visibleSize.height / 4);
				bezier.endPosition = Vec2(positionX, this->getPosition().y);

				auto action = BezierTo::create(swimTime * visibleSize.height, bezier);
				this->runAction(action);

				this->runAction(Sequence::create(action, CallFunc::create([this, positionX, visibleSize, origin]() {
					ccBezierConfig bezier2;
					float xPath = 0.0;
					if (positionX < visibleSize.width / 2) {
						xPath = origin.x - this->getPosition().x;
						bezier2.endPosition = Vec2(origin.x - visibleSize.width / 10, this->getPosition().y);
					}
					else {
						xPath = origin.x + visibleSize.width - this->getPosition().x;
						bezier2.endPosition = Vec2(origin.x + visibleSize.width + visibleSize.width / 10, this->getPosition().y);
					}
					bezier2.controlPoint_1 = Vec2(this->getPosition().x + xPath / 4, initialY + visibleSize.height / 4);
					bezier2.controlPoint_2 = Vec2(this->getPosition().x + xPath / 4 - 2, initialY + visibleSize.height / 4);
					
					float swimTime = cocos2d::RandomHelper::random_real(0.075, 0.035);
					auto action = BezierTo::create(swimTime * visibleSize.height, bezier2);
					this->runAction(action);
				}), nullptr));
			}
	}
}

void JellyFish::drawFish(int fishId) {
	score = 1000 * fishId;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("fj1.png"));
	Vector<SpriteFrame*> animFrames(2);

	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, "fj%i.png", i);
		auto spr = CCSprite::create(str);

		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	fScale = visibleSize.width / 15 / fishSprite->getContentSize().width;

	this->addChild(fishSprite);

	int positionX;
	int positionXNeg = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x - 50),
		static_cast<int>(origin.x));
	int positionXPos = cocos2d::RandomHelper::random_int(static_cast<int>(origin.x + visibleSize.width),
		static_cast<int>(origin.x + visibleSize.width + 50));
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
	auto physicsBody = PhysicsBody::createBox(fishSprite->getContentSize());
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x06);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setTag(10);

	this->setPhysicsBody(physicsBody);
	
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));
	this->getTexture()->setAliasTexParameters();

	this->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create(std::bind(&JellyFish::run, this)),
		nullptr)));
}

void JellyFish::increaseScale() {
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

JellyFish* JellyFish::create(const std::string &path, ssize_t capacity)
{
	JellyFish *fish = new JellyFish();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

JellyFish* JellyFish::create() {
	return JellyFish::create("sprites/sp_main.png", 29L);
}

void JellyFish::increaseScore(float pts) {
	++score;
}

int JellyFish::getScore() {
	return score;
}