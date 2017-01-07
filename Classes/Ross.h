#ifndef __ROSS_H__
#define __ROSS_H__

#include "cocos2d.h"

class Ross : public cocos2d::SpriteBatchNode
{
public:
	static Ross* create(const std::string &path, ssize_t capacity);
	static Ross* create();
	void drawFish();
	void decreaseHealth();
	void kill();
	void switchMode(bool isAngry);
	int getHealth();
private:
	void run();
	void projectilesSequence(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
	void shootProjectiles(cocos2d::Vec2 position);
	float fScale;
	float score = 0.0;
	int health = 10;
	bool isAngryMode = false;
	cocos2d::Sprite* fishSprite;
};

#endif // __ROSS_H__
#pragma once
