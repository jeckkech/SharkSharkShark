#ifndef LOCALIZEDSTRING_H_
#define LOCALIZEDSTRING_H_
#include "cocos2d.h"

using namespace cocos2d;

class LocalizedString {
public:

	static __String* create(const char *key);

	static const char * getSystemLang();


	LocalizedString();
	virtual ~LocalizedString();
};

#endif /* LOCALIZEDSTRING_H_ */