#ifndef __SHARK_H__
#define __SHARK_H__

#include "cocos2d.h"

class Shark : public cocos2d::SpriteBatchNode
{
public:
	static Shark* create(const std::string &path, ssize_t capacity);
	static Shark* create();
	void drawFish();
	void increaseScore(float pts);
	void increaseScale();
	int getScore();
private:
	void run();
	float fScale;
	float score = 0.0;
	float initialY = 0.0;

};

#endif // __SHARK_H__
#pragma once
