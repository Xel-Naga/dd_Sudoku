#ifndef _SCOREMGR_H_
#define _SCOREMGR_H_

#include <string>
#include <list>
#include <map>

#include "singleton.h"
namespace XAXA {

class ScoreMgr: public BAGUA::Singleton<ScoreMgr> {
public:
    void saveInt(const char* key, int value, bool needFlush = true);
    void saveString(const char* key, const std::string& value, bool needFlush = true);
    int loadInt(const char* key,int defaultValue=0);
    std::string loadString(const char* key, const std::string& defaultValue=std::string(""));
    void flush();
};

}

#endif