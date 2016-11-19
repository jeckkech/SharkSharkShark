#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "Fish.h"
#include "PlayerFish.h"
#include "ui/CocosGUI.h"

class MainGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float) override;
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);
private: 
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) {
		sceneWorld = world;
	};
	cocos2d::PhysicsWorld *sceneWorld;
	
	int totalScore = 0;
	cocos2d::ui::Text *scoreLabel;
	cocos2d::Sprite *parallaxBg;
	cocos2d::Sprite *parallaxBg2;
	void setScore(int points);
	cocos2d::EventListenerTouchOneByOne* playerTouchListener;
	cocos2d::EventListenerPhysicsContact* playerContactListener;
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event);
	PlayerFish* playerFish;
	std::deque<Fish*> fishList;
};

#endif // __MAINGAME_SCENE_H__
