#ifndef __SKELETON_H__
#define __SKELETON_H__

#include "cocos2d.h"

class Skeleton : public cocos2d::Sprite
{
public:
	static Skeleton* create(const std::string &path);
	void run(cocos2d::Vec2 position, float scaleX, float scaleY);
	int getScore();
	void destroy();
};

#endif // __SKELETON_H__
