#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PluginIAP/PluginIAP.h"

class MainMenu : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void exitCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void startCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void startInfiniteCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void galleryCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void muteCallback(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void purchaseAppCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);
private:   
		virtual void onInitialized(bool ok) override;
		virtual void onSuccess(sdkbox::Product const& p) override;
		virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
		virtual void onCanceled(sdkbox::Product const& p) override;
		virtual void onRestored(sdkbox::Product const& p) override;
		virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
		virtual void onProductRequestFailure(const std::string &msg) override;
		void onRestoreComplete(bool ok, const std::string &msg);
};

#endif // __MAINMENU_SCENE_H__
#pragma once
