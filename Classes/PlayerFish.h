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
	void blink();
	void refreshPlayerDirection(int posX);
	int getScore();
	int getStage();
	bool isBlinking();
	
private:
	float fScale;
	float score = 0.0f;
	int currentStage;
	int direction = 0;
	bool blinking = false;
	void evolve();
};

#endif // __PLAYERFISH_H__
