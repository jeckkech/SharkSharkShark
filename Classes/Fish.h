#ifndef __FISH_H__
#define __FISH_H__

#include "cocos2d.h"

class Fish: public cocos2d::SpriteBatchNode
{
public:
	static Fish* create(const std::string &path, ssize_t capacity);
	static Fish* create();
	void drawFish(int fishId);
	void increaseScore(float pts);
	void increaseScale();
	int getScore();
	void kill(bool withoutSkeleton);
private:
	void run();
	void evolve();
	float fScale;
	float score = 0.0;
	bool dead = false;
	int currentStage;
};

#endif // __FISH_H__
