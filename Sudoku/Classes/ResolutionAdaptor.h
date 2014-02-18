/*
�������Ȩ��XAXA���У�δ����ɣ��������Կ��������ã�
*/
#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include <vector>
#include <string>

namespace XAXA {

//���ڷֱ������䣬ʹ���ڲ�ͬ���豸����ʾ�ߴ���ʵ�ͼƬ
class ResolutionAdaptor {
public:
    ResolutionAdaptor(int f_width, int f_height);
    //�����豸�ֱ��ʣ����ú��ʵ���ʾ����
    void exec(int& d_width,int& d_height,std::vector<std::string>& searchPath);

    void getResolution(int& d_width,int& d_height);

    void getSearchPath(std::vector<std::string>& searchPath);

    int getResolutionPolicy();

private:
    int _f_width;
    int _f_height;

    int _d_width;
    int _d_height;
};

}

#endif