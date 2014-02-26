
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
    float _minScaleRatio;//��С���ű���
    float _maxScaleRatio;//������ű���
    float _widthRatio;
    float _heightRatio;
    std::string _path;
    bool _isVertical;//���淽���Ƿ�Ϊ����ģ�true������ false������
} ;

//bool operator<(const Resolution& lhs, const Resolution& rhs) {
//    return lhs.getMinRatio() < rhs.getMinRatio();
//}


/*
//����������3�ֱַ���������󲿷��豸
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
        //1. ��С��ͬ
        if(res.isEqual(d_width,d_height)) {
            return;
        }

        //2. ��С��ͬ�����ߴ���������ű���
        float widthRatio = res.getWidthRatio(d_width);
        res.getHeightRatio(d_height);
        if(res.isAspectRatioEqual(d_width,d_height)) {    
            if (res.isCompatibleForScale(widthRatio)) {
                res.getResolution(_d_width, _d_height);
                return;
            }
        }
    }

    //3. ���ű����������ޣ�������ı���
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

//������ʹ��kResolutionShowAll����Ȼ�����кڱߣ����������ܹ���ʾ����
//����ʹ��kResolutionFixedHeight,kResolutionFixedWidth�������ֻ��޸���Ʒֱ��ʣ�������������Ҫ����һЩ
//������ϵͳ��ʹ�õĵ����궼�ǻ�����Ʒֱ��ʵģ����������setContentScaleFactor��
//��ô�Ͳ���ֱ��ʹ��ͼƬ�е����꣬���������������
//����ͼƬ�ֱ��ʺ���Ʒֱ�����ͬ���������������ӣ��������������ȽϷ���
//��������Ĳ��Ծ��Ǹ����������Ļ�ֱ����ҵ���ӽ�����Դ�ֱ��ʣ�����֮��Ϊ��Ʒֱ���
//��Ҫ��עandroidManifest.xml�й��ں����������ã�screenOrientation="portrait"
void ResolutionAdaptor::exec(int& d_width,int& d_height,std::vector<std::string>& searchPath) {
    getDesignResolutionFromJson(d_width,d_height);
    //getResolution(d_width,d_height);
    //getSearchPath(searchPath);
    //getResolutionPolicy();

    d_width = 320;
    d_height = 480;
}

}
