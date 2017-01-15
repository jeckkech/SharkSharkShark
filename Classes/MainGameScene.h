#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "Fish.h"
#include "PlayerFish.h"
#include "Shark.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class MainGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	static cocos2d::Scene* createSceneInfinite();
	static cocos2d::Vec2 playerPosition;
	static int mode;
	virtual bool init();
    // a selector callback
	void updateTimer(float dt);
    void menuCloseCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void repeatCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void continueCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void biteCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void achievementUnlocked();
	void blinkNotification(std::string str);
	void staticNotification(std::string str);
	void update(float) override;
	void onEnterTransitionDidFinish() override;
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);
private: 
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) {
		sceneWorld = world;
	};
	void setUpInitial();

	cocos2d::PhysicsWorld *sceneWorld;
	
	int totalScore = 0;
	int bossLimit = 60;
	bool hwStage1 = false;
	bool stage1 = false;
	bool hwStage2 = false;
	bool stage2 = false;
	bool hwStage3 = false;
	bool hwStage4 = false;
	bool hwStage5 = false;
	bool hwStage7_5 = false;
	bool hwStage8 = false;
	bool isGameOver = false;
	bool isBiteMode = false;
	bool isAchievementShowing = false;
	bool isInNotification = false;
	bool hiScoreNotified = false;
	bool timerUpdated = false;

	bool isInCountdown = false;
	int topStage = 2;
	int lastPlayerStage = 2;
	cocos2d::ui::Text *scoreLabel;
	cocos2d::Sprite *parallaxBg;
	cocos2d::Sprite *parallaxBg2;

	void lifebarInitialize();
	void decreaseLifebarPosition();
	void setScore(int points);
	void saveHiScore();
	void gameOver();
	void endingSequence1();
	void endingSequence2();
	void exit(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void resume(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void createScoreLabel(int points, cocos2d::Vec2 position);
	cocos2d::EventListenerTouchOneByOne* playerTouchListener;
	cocos2d::EventListenerPhysicsContact* playerContactListener;
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onContactSeparate(cocos2d::PhysicsContact &contact);
	bool onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouch(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event);
	PlayerFish* playerFish;
	Shark* shark;
	cocostudio::timeline::ActionTimeline* pActionTimeline;
};

#endif // __MAINGAME_SCENE_H__
