#ifndef __JELLYFISH_H__
#define __JELLYFISH_H__

#include "cocos2d.h"

class JellyFish : public cocos2d::SpriteBatchNode
{
public:
	static JellyFish* create(const std::string &path, ssize_t capacity);
	static JellyFish* create();
	void drawFish(int fishId);
	void increaseScore(float pts);
	void increaseScale();
	int getScore();
private:
	void run();
	float fScale;
	float score = 0.0;
	float initialY = 0.0;

};

#endif // __JELLYFISH_H__
#pragma once
