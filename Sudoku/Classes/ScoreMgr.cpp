
#include "ScoreMgr.h"

#include "support\user_default\CCUserDefault.h"
USING_NS_CC;

template <>
XAXA::ScoreMgr* BAGUA::Singleton<XAXA::ScoreMgr>::_instance = NULL;

namespace XAXA {

void ScoreMgr::saveInt(const char* key, int value, bool needFlush) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey(key,value);
    if(needFlush) {
        CCUserDefault::sharedUserDefault()->flush();//����һ��Ҫ�ύд��Ŷ�����򲻻��¼��xml�У��´�������Ϸ��ͻ�ȡ����value�ˡ�
    }
}

void ScoreMgr::saveString(const char* key, const std::string& value, bool needFlush) {
    CCUserDefault::sharedUserDefault()->setStringForKey (key,value);
    if(needFlush) {
        CCUserDefault::sharedUserDefault()->flush();
    }
}

int ScoreMgr::loadInt(const char* key,int defaultValue) {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(key,defaultValue);
}

std::string ScoreMgr::loadString(const char* key,const std::string& defaultValue) {
    return CCUserDefault::sharedUserDefault()->getStringForKey(key,defaultValue);
}

void ScoreMgr::flush() {
    CCUserDefault::sharedUserDefault()->flush();
}

}


