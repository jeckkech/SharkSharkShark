#ifndef __CANCER_H__
#define __CANCER_H__

#include "cocos2d.h"

class Cancer : public cocos2d::SpriteBatchNode
{
public:
	static Cancer* create(const std::string &path, ssize_t capacity);
	static Cancer* create();
	void drawFish(int fishId);
	void increaseScore(float pts);
	void increaseScale();
	int getScore();
private:
	void run();
	void emerge();
	void sink();
	float fScale;
	float score = 0.0;	
	float initialY = 0.0;
	bool cancerLanded = true;
	int walkCycles = 0;
	
};

#endif // __CANCER_H__
