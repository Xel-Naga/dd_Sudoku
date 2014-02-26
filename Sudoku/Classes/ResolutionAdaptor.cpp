
#include "cocos2d.h"
USING_NS_CC;

#include "ResolutionAdaptor.h"

#include "json/json.h"

#include <sstream>
#include <fstream>

namespace XAXA {

enum ResolutionPolicy
{
    // The entire application is visible in the specified area without trying to preserve the original aspect ratio.
    // Distortion can occur, and the application may appear stretched or compressed.
    kResolutionExactFit,
    // The entire application fills the specified area, without distortion but possibly with some cropping,
    // while maintaining the original aspect ratio of the application.
    kResolutionNoBorder,
    // The entire application is visible in the specified area without distortion while maintaining the original
    // aspect ratio of the application. Borders can appear on two sides of the application.
    kResolutionShowAll,
    // The application takes the height of the design resolution size and modifies the width of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedHeight,
    // The application takes the width of the design resolution size and modifies the height of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedWidth,

    kResolutionUnKnown,
};

class Resolution{
public:
    Resolution() :
        _widthRatio(0), _heightRatio(0) {
    }

    bool operator == (const Resolution& rhs) {
        return (_width == rhs._width && _height == rhs._height);
    }

    bool operator < (const Resolution& rhs) {
        return getMinRatio() < rhs.getMinRatio();
    }

    bool isEqual(int& width, int& height) {
        return (_width == width && _height == height);
    }

    bool isAspectRatioEqual(int& width, int& height) {
        return ((width % _width == 0) && (height % _height == 0));
    }

    float getWidthRatio(const int& width=0) {
        if(_widthRatio != 0 && width != 0) {
            _widthRatio = (float)width / (float)_width;
        }
        return _widthRatio;
    }

    float getHeightRatio(const int& height=0) {
        if(_heightRatio != 0 && height != 0) {
            _heightRatio = (float)height / (float)_height;
        }
        return _heightRatio;
    }

    bool isCompatibleForScale(float scaleRatio) {
        return (_minScaleRatio <= scaleRatio) && (scaleRatio <= _maxScaleRatio);
    }

    void getResolution(int& width, int& height) {
        width = _width;
        height = _height;
    }

    float getMinRatio() const {
        return (_widthRatio < _heightRatio)? _widthRatio:_heightRatio;
    }

private:
    int _width;
    int _height;
    float _minScaleRatio;//最小缩放比例
    float _maxScaleRatio;//最大缩放比例
    float _widthRatio;
    float _heightRatio;
    std::string _path;
    bool _isVertical;//画面方向是否为纵向的？true：纵向 false：横向
} ;

//bool operator<(const Resolution& lhs, const Resolution& rhs) {
//    return lhs.getMinRatio() < rhs.getMinRatio();
//}


/*
//给定主流的3种分辨率来适配大部分设备
struResolutionDir res_array[] = {
    {{480,320},"res_0"},
    {{1024,768},"res_1"},
    {{2048,1536},"res_2"},
    {{1920,1080},"res_3"},
};
*/
ResolutionAdaptor::ResolutionAdaptor(int f_width, int f_height) :
    _f_width(f_width),_f_height(f_height),_d_width(0),_d_height(0) {
}

void ResolutionAdaptor::getResolution(int& d_width,int& d_height) {
/*    
    std::vector<Resolution> res_list;

    for(auto res:res_list) {
        //1. 大小相同
        if(res.isEqual(d_width,d_height)) {
            return;
        }

        //2. 大小不同，看尺寸比例，缩放比例
        float widthRatio = res.getWidthRatio(d_width);
        res.getHeightRatio(d_height);
        if(res.isAspectRatioEqual(d_width,d_height)) {    
            if (res.isCompatibleForScale(widthRatio)) {
                res.getResolution(_d_width, _d_height);
                return;
            }
        }
    }

    //3. 缩放比例超过门限，看长宽的比例
*/  
}

void ResolutionAdaptor::getSearchPath(std::vector<std::string>& searchPath) {
    std::stringstream ss;
    ss<<"res_"<<_d_width<<"_"<<_d_height;
    searchPath.push_back(ss.str());
}

int ResolutionAdaptor::getResolutionPolicy() {
    return kResolutionShowAll;
}

void getDesignResolutionFromJson(int& width, int& height) {
  
    std::ifstream ifs;
    ifs.open("design_resolution.json");
    assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        return ;
    }
 
    int size = root.size();
    for (int i=0; i<size; ++i)
    {
        width = root[i]["width"].asInt();
        height = root[i]["height"].asInt();
    }  

}

//倾向于使用kResolutionShowAll，虽然可能有黑边，但是内容能够显示完整
//或者使用kResolutionFixedHeight,kResolutionFixedWidth，这两种会修改设计分辨率，处理起来可能要复杂一些
//我们在系统内使用的点坐标都是基于设计分辨率的，所以如果有setContentScaleFactor，
//那么就不能直接使用图片中的坐标，必须乘上缩放因子
//建议图片分辨率和设计分辨率相同，不设置缩放因子，这样开发起来比较方便
//所以这里的策略就是根据输入的屏幕分辨率找到最接近的资源分辨率，并将之作为设计分辨率
//需要关注androidManifest.xml中关于横竖屏的设置，screenOrientation="portrait"
void ResolutionAdaptor::exec(int& d_width,int& d_height,std::vector<std::string>& searchPath) {
    getDesignResolutionFromJson(d_width,d_height);
    //getResolution(d_width,d_height);
    //getSearchPath(searchPath);
    //getResolutionPolicy();

    d_width = 320;
    d_height = 480;
}

}
