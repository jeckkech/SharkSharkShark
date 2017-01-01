#include "Cancer.h"

USING_NS_CC;

void Cancer::run() {
	//CCLOG("RUNNING ACTIONS: %i", Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this));
	if(Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(this) == 1){
		if (walkCycles >= 1 && cancerLanded) {
			Cancer::emerge();
		}
		else if (!cancerLanded && walkCycles >= 1) {
			Cancer::sink();
		} else if (cancerLanded) {
			++walkCycles;
			cancerLanded = true;
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
			if(this){
				auto action = MoveTo::create(swimTime * visibleSize.height, Point(positionX, this->getPosition().y));
				this->runAction(action);
			}
		}
	}
}

void Cancer::emerge() {
	cancerLanded = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float swimTime = cocos2d::RandomHelper::random_real(0.005, 0.001);
	auto action = MoveTo::create(visibleSize.height * swimTime, Vec3(this->getPosition().x, visibleSize.height/4+origin.y, 0));
	this->runAction(action);
}

void Cancer::sink() {
	cancerLanded = true;
	walkCycles = 0;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto action = MoveTo::create(visibleSize.height * 0.008, Vec3(this->getPosition().x, initialY, 0));
	this->runAction(action);
}
void Cancer::drawFish(int fishId) {
	score = 1000 * fishId;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("sprites/sp_main.plist");
	auto fishSprite = CCSprite::createWithSpriteFrame(cache->getSpriteFrameByName("can11.png"));
	Vector<SpriteFrame*> animFrames(2);

	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, "can%i%i.png", fishId, i);
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
	int positionY = origin.y + fishSprite->getContentSize().height*fScale;
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

	//CallFuncN::create(std::bind(&Cancer::emerge, this)),
	//	CallFuncN::create(std::bind(&Cancer::sink, this)),
	
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	fishSprite->runAction(RepeatForever::create(Animate::create(animation)));
	this->getTexture()->setAliasTexParameters();

	this->runAction(RepeatForever::create(Sequence::create(
		CallFuncN::create(std::bind(&Cancer::run, this)),
		nullptr)));
}

void Cancer::increaseScale() {
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

Cancer* Cancer::create(const std::string &path, ssize_t capacity)
{
	Cancer *fish = new Cancer();
	if (fish && fish->initWithFile(path))
	{
		// Set to autorelease
		fish->autorelease();

		return fish;
	}
	CC_SAFE_DELETE(fish);
	return NULL;
}

Cancer* Cancer::create() {
	return Cancer::create("sprites/sp_main.png", 29L);
}

void Cancer::increaseScore(float pts) {
	++score;
}

int Cancer::getScore() {
	return score;
}