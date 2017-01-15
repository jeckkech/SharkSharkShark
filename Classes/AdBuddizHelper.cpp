/*
Copyright (c) 2014 Mudafar
GPLv3
*/

#include "AdBuddizHelper.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>


const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void AdBuddizHelper::showAd()
{
	if(!cocos2d::UserDefault::getInstance()->getBoolForKey("removeAds")){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

}

#endif
