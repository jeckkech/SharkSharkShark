#include "SplashScene.h"
#include "MainMenuScene.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	experimental::ui::VideoPlayer *videoPlayer = experimental::ui::VideoPlayer::create();
	
	this->setContentSize(visibleSize);
	videoPlayer->setContentSize(Size(1280, 720));
	videoPlayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	videoPlayer->setKeepAspectRatioEnabled(true);
	videoPlayer->setScale(visibleSize.height / videoPlayer->getContentSize().height);
	//videoPlayer->setEffectsVolume(UserDefault::getInstance()->getBoolForKey("silentMode") ? 0.0f : 5.0f);
	videoPlayer->setFileName("video/intro_splash.mp4");

	videoPlayer->setFullScreenEnabled(true);
	videoPlayer->setTouchEnabled(false);
	videoPlayer->addEventListener(CC_CALLBACK_2(SplashScene::videoEventCallback, this));
	this -> addChild(videoPlayer);
	videoPlayer->play();
    return true;
}

void SplashScene::videoEventCallback(cocos2d::Ref *sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
	if(eventType == cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED || 
		eventType == cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED){
		this->removeAllChildrenWithCleanup(true);
		auto scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
	} 
}
void SplashScene::GoToMainMenuScene(float dt) {
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void SplashScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
