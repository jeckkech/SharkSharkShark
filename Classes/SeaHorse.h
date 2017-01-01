#ifndef __SEAHORSE_H__
#define __SEAHORSE_H__

#include "cocos2d.h"

class SeaHorse : public cocos2d::SpriteBatchNode
{
public:
	static SeaHorse* create(const std::string &path, ssize_t capacity);
	static SeaHorse* create();
	void drawFish(int fishId);
	void increaseScore(float pts);
	void increaseScale();
	int getScore();
	void kill(bool withoutSkeleton);
private:
	void run();
	float fScale;
	float score = 0.0;
	bool dead = false;
	int currentStage;
};

#endif // __SEAHORSE_H__
