#include "Bubble.h"

USING_NS_CC;

Bubble* Bubble::create(const std::string &path)
{
	Bubble *bubble = new Bubble();
	if (bubble && bubble->initWithFile(path))
	{
		// Set to autorelease
		bubble->autorelease();

		return bubble;
	}
	CC_SAFE_DELETE(bubble);
	return NULL;
}

void Bubble::run(Vec2 position) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float scale = visibleSize.width / 15 / this->getContentSize().width;
	this->setScaleX(scale);
	this->setPosition(position);
	//this->getBoundingBox().setRect(this->getPositionX(), this->getPositionY(), fishSprite->getContentSize().width * fScale, fishSprite->getContentSize().height * fScale);
	auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(1.0f, 1.0f, 1.0f));
	//physicsBody->setMass(2000);
	//physicsBody->applyForce(Vec2(-20000, -20000), Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x16);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setRotationEnable(true);
	physicsBody->setEnabled(true);
	this->setPhysicsBody(physicsBody);
	this->getTexture()->setAliasTexParameters();
}