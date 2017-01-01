#ifndef __PLAYERFISH_H__
#define __PLAYERFISH_H__

#include "cocos2d.h"

class PlayerFish : public cocos2d::SpriteBatchNode
{
public:
	static PlayerFish* create(const std::string &path, ssize_t capacity);
	static PlayerFish* create();
	void drawFish(int fishId);
	void createFish(int fishId);
	void increaseScore(float pts);
	void increaseScale();
	void refreshPlayerDirection(int posX);
	int getScore();
private:
	float fScale;
	float score = 0.0;
	int currentStage;
	int direction = 0;
	void evolve();

};

#endif // __PLAYERFISH_H__
