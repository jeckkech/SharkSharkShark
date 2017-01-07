#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"

class Bubble : public cocos2d::Sprite
{
public:
	static Bubble* create(const std::string &path);
	void run(cocos2d::Vec2 position);
private:
};

#endif // __BUBBLE_H__
