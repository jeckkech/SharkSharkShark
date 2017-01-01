#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void exitCallback(cocos2d::Ref* pSender);
	void startCallback(cocos2d::Ref* pSender);
	void optionsCallback(cocos2d::Ref* pSender);
	void muteCallback(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);
	
};

#endif // __MAINMENU_SCENE_H__
#pragma once
