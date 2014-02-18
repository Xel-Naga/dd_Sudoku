
#include "LevelMgr.h"

//template <class T>
//T* Singleton<T>::_instance = NULL;

//��ʱ��һ����Ϸ����ȡ������XAXA����˼����HAHA(����)
template <>
XAXA::LevelMgr* BAGUA::Singleton<XAXA::LevelMgr>::_instance = NULL;

namespace XAXA {

//�ؿ���ų�ʼ��Ϊ0
LEVEL_SN_TYPE LevelMgr::_next_level_sn = 0;

void LevelMgr::add_level(LevelMap* level) {
    LEVEL_SN_TYPE next = alloc_level_sn();
    if(_level_map_list.count(next) == 0) {
        _level_map_list[next] = level;
        //ͬʱ�����ؿ������Ϣ
        _level_info_list[next] = new LevelInfo();
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
