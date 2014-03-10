
#include "LevelMgr.h"
#include "ScoreMgr.h"

//template <class T>
//T* Singleton<T>::_instance = NULL;

//暂时给一个游戏引擎取个名字XAXA，意思就是HAHA(哈哈)
template <>
XAXA::LevelMgr* BAGUA::Singleton<XAXA::LevelMgr>::_instance = NULL;

namespace XAXA {

//关卡编号初始化为0
LEVEL_SN_TYPE LevelMgr::_next_level_sn = 0;

void LevelMgr::clear() {
    _next_level_sn = 0;
    _curr_level_sn = 0;
    std::map<LEVEL_SN_TYPE,LevelMap*>::iterator iterMap = _level_map_list.begin();
    std::map<LEVEL_SN_TYPE,LevelInfo*>::iterator iterInfo = _level_info_list.begin();//关卡信息列表
    for(;iterMap != _level_map_list.end();++iterMap) {
        delete iterMap->second;
    }
    for(;iterInfo != _level_info_list.end(); ++iterInfo) {
        delete iterInfo->second;
    }
    _level_map_list.clear();
    _level_info_list.clear();
}

void LevelMgr::add_level(LevelMap* level) {
    LEVEL_SN_TYPE next = alloc_level_sn();
    if(_level_map_list.count(next) == 0) {
        _level_map_list[next] = level;
        //同时创建关卡相关信息
        _level_info_list[next] = new LevelInfo();
    }
}

void LevelMgr::loadLevelInfo() {
    char buff[32];
    for(int i = 0; i < get_level_count(); ++i) {
        LevelInfo* lvlInfo = get_level_info(i);
        sprintf(buff,"level_%d_lock",(int)i);
        lvlInfo->is_lock = (bool)ScoreMgr::instance()->loadInt(buff,1);
        sprintf(buff,"level_%d_timecost",(int)i);
        lvlInfo->complete_time = ScoreMgr::instance()->loadInt(buff,0);
        sprintf(buff,"level_%d_win",(int)i);
        lvlInfo->is_win = ScoreMgr::instance()->loadInt(buff,0);
    }
}

LevelInfo* LevelMgr::get_level_info(LEVEL_SN_TYPE level_sn) {
    if(_level_info_list.count(level_sn) > 0) {
        return _level_info_list[level_sn];
    }
    else {
        return NULL;
    }
}

LevelMap* LevelMgr::get_level_map(LEVEL_SN_TYPE level_sn) {
    if(_level_map_list.count(level_sn) > 0) {
        return _level_map_list[level_sn];
    }
    else {
        return NULL;
    }
}

LEVEL_SN_TYPE LevelMgr::get_curr_level() {
    return _curr_level_sn;
}

void LevelMgr::set_curr_level(LEVEL_SN_TYPE sn) {
    _curr_level_sn = sn;
}

}//end namespace XAXA
