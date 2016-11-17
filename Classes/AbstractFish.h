#ifndef __ABSTRACTFISH_H__
#define __ABSTRACTFISH_H__

#include "cocos2d.h"

class AbstractFish : public cocos2d::SpriteBatchNode
{
public:
	virtual int getScore();
	virtual int getCurrentStage();
	virtual void increaseScore();
private:
	int score = 0;
	int currentStage;
};

#endif // __ABSTRACTFISH_H__
