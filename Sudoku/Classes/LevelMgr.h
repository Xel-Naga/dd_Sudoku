#ifndef _LEVELMGR_H_
#define _LEVELMGR_H_

#include <string>
#include <list>
#include <map>

//暂时给一个游戏引擎取个名字XAXA，意思就是HAHA(哈哈)
namespace XAXA {

template <class T>
class Singleton {
public:
    static T* instance() {
        if(!_instance) {
            _instance = new T();

        }

        return _instance;
    }

    static void release() {
        if(_instance) {
            delete _instance;
        }
    }

private:
    static T* _instance;
};


//关卡的地图信息基类
class LevelMap {
public:
    virtual void nothing_to_do() {}
};

//关卡的状态信息,纯数据类。在服务器端可以考虑需要建立数据库？客户端不需要
class LevelInfo  {
public:
    LevelInfo() {
        is_lock = true;
        complete_time = "";
    }
    bool is_lock;
    std::string complete_time;//完成时间，年月日，时分秒等
};

#define LEVEL_SN_TYPE unsigned int

//关卡管理器，包括关卡信息，关卡地图信息，
//如果是网络游戏，则包括了用户信息，至少需要与某个用户ID关联，暂时可以不考虑，给个接口就行了
class LevelMgr: public Singleton<LevelMgr> {
public:
    void add_level(LevelMap* level);
    LevelInfo* get_level_info(LEVEL_SN_TYPE level_sn);
    LevelMap* get_level_map(LEVEL_SN_TYPE level_sn);
    static LEVEL_SN_TYPE alloc_level_sn() { return _next_level_sn++;}
    LEVEL_SN_TYPE get_curr_level();
    void set_curr_level(LEVEL_SN_TYPE sn);

private:
    LEVEL_SN_TYPE _curr_level_sn;//当前关卡编号
    std::map<LEVEL_SN_TYPE,LevelMap*> _level_map_list;//关卡列表
    std::map<LEVEL_SN_TYPE,LevelInfo*> _level_info_list;//关卡信息列表
    static LEVEL_SN_TYPE _next_level_sn;//
};


}//end namespace XAXA

#endif