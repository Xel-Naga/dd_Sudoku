#ifndef _LEVELMGR_H_
#define _LEVELMGR_H_

#include <string>
#include <list>
#include <map>

#include "singleton.h"

//��ʱ��һ����Ϸ����ȡ������XAXA����˼����HAHA(����)
namespace XAXA {

//�ؿ��ĵ�ͼ��Ϣ����
class LevelMap {
public:
    virtual void nothing_to_do() {}
};

//�ؿ���״̬��Ϣ,�������ࡣ�ڷ������˿��Կ�����Ҫ�������ݿ⣿�ͻ��˲���Ҫ
class LevelInfo  {
public:
    LevelInfo() {
        is_lock = true;
        complete_time = 0;
    }
    bool is_lock;
    int complete_time;//���ʱ��
};

#define LEVEL_SN_TYPE unsigned int

//�ؿ��������������ؿ���Ϣ���ؿ���ͼ��Ϣ��
//�����������Ϸ����������û���Ϣ��������Ҫ��ĳ���û�ID��������ʱ���Բ����ǣ������ӿھ�����
class LevelMgr: public BAGUA::Singleton<LevelMgr> {
public:
    void clear();
    void add_level(LevelMap* level);
    void loadLevelInfo();
    LevelInfo* get_level_info(LEVEL_SN_TYPE level_sn);
    LevelMap* get_level_map(LEVEL_SN_TYPE level_sn);
    static LEVEL_SN_TYPE alloc_level_sn() { return _next_level_sn++;}
    LEVEL_SN_TYPE get_curr_level();
    void set_curr_level(LEVEL_SN_TYPE sn);
    int get_level_count() { return _next_level_sn;}

private:
    LEVEL_SN_TYPE _curr_level_sn;//��ǰ�ؿ����
    std::map<LEVEL_SN_TYPE,LevelMap*> _level_map_list;//�ؿ��б�
    std::map<LEVEL_SN_TYPE,LevelInfo*> _level_info_list;//�ؿ���Ϣ�б�
    static LEVEL_SN_TYPE _next_level_sn;//
};


}//end namespace XAXA

#endif