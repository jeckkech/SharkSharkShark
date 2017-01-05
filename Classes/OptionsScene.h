#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Options : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void backCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(Options);
private: 
	void unlockElements();

};

#endif // __OPTIONS_SCENE_H__
#pragma once
