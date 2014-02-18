/*
本代码版权归XAXA所有，未经许可，不得擅自拷贝，引用，
*/
#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include <vector>
#include <string>

namespace XAXA {

//用于分辨率适配，使得在不同的设备上显示尺寸合适的图片
class ResolutionAdaptor {
public:
    ResolutionAdaptor(int f_width, int f_height);
    //根据设备分辨率，设置合适的显示策略
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