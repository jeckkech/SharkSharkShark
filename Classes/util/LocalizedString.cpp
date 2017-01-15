#include "LocalizedString.h"
#include <string>
//#include "util/jsoncpp.hpp"

using namespace std;
//using json = nlohmann::json;

__String* LocalizedString::create(const char *key) {

	const char * fileName = getSystemLang();

	__String * str;

	string fname = fileName;
	fname = "/" + fname + ".json";

	

	//if(CCFileUtils::isFileExist()){

	auto fileData = FileUtils::getInstance()->getStringFromFile(fname.c_str());

	//json j = json::parse(fileData.c_str());

	CCLOG("RELAX DONT DO IT");
	//CCLOG("%s", j.at("HIGHSCORE"));


	return str;
}

const char * LocalizedString::getSystemLang()
{
	LanguageType curLanguage = Application::getInstance()->getCurrentLanguage();

	switch (curLanguage) {

	case LanguageType::RUSSIAN:
		return "ru";
		break;
	case LanguageType::ENGLISH:
		return "en";
	default:
		return "en";
		break;
	}

	return "en";

}
LocalizedString::LocalizedString() {
}

LocalizedString::~LocalizedString() {
}