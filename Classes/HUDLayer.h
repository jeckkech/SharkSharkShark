#ifndef  __HUDLAYER_H__
#define  __HUDLAYER_H__

#include "cocos2d.h"

class HUDLayer : public cocos2d::LayerColor
{
public:
	HUDLayer();
	~HUDLayer();
	static HUDLayer* createLayer(const std::string& _message);
	void initOptions(const std::string& _message);

	virtual void draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform,
		bool transformUpdated);

	inline virtual cocos2d::LabelBMFont* getMessageLabel() { return _messageLabel; };

private:
	cocos2d::LabelBMFont* _messageLabel;
};

#endif // __HUDLAYER_H__