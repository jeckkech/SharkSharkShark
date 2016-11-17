#include "HUDLayer.h"
USING_NS_CC;

HUDLayer::HUDLayer() {}

HUDLayer::~HUDLayer() {}

HUDLayer* HUDLayer::createLayer(const std::string& _message)
{   
	HUDLayer* a = new HUDLayer();
    a->create();
    a->setColor(cocos2d::Color3B(0, 0, 0));

    a->setContentSize(cocos2d::Size(1, 1));
    a->setAnchorPoint(cocos2d::Vec2(0, 0)); 

    a->initOptions(_message);

    return a;
}

void HUDLayer::initOptions(const std::string& _message)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
/*
    _messageLabel = cocos2d::LabelBMFont::create(_message.c_str(), "Marker Felt Small.fnt");
    _messageLabel->setColor(cocos2d::Color3B(255, 215, 0));*/

	auto hudSprite = CCSprite::create("sprites/hud/hud_panel.png");
	float scaleWidth = visibleSize.width / hudSprite->getContentSize().width;
	float scaleHeight = visibleSize.height / 10 / hudSprite->getContentSize().height;
	hudSprite->setAnchorPoint(Point(0, 0));
	hudSprite->setScaleX(scaleWidth);
	hudSprite->setScaleY(scaleHeight);
	hudSprite->setPosition(0, visibleSize.height - hudSprite->getContentSize().height * scaleHeight);
    addChild(hudSprite, 5);

   /* _messageLabel->setPosition(cocos2d::Vec2(Director::getInstance()->getVisibleSize().width, 
        Director::getInstance()->getVisibleSize().height));*/


}

void HUDLayer::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform,
	bool transformUpdated) {}