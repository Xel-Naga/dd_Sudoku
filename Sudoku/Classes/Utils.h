#ifndef _UTILS_H_
#define _UTILS_H_
#include <string>

typedef unsigned char uint8;
typedef unsigned short uint16;

//将Unicode小端转为UTF8，主要用于中文输出
std::string UCS2ToUTF8(const std::string& src);
int UTF8ToUCS2(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen);
std::string WStrToUTF8(const std::wstring& str);

//获取边界，用于当坐标超过屏幕左边界或者右边界时自动对齐边界值，
//是的对象显示不会超出屏幕
float getBound(float x,float offset,float min,float max);


#endif