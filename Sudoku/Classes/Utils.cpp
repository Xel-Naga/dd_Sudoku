#include <string.h>

#include "Utils.h"

//! convert from wstring to UTF8 using self-coding-converting

inline void WStrToUTF8(std::string& dest, const std::wstring& src){

    dest.clear();

    for (size_t i = 0; i < src.size(); i++){

        wchar_t w = src[i];

        if (w <= 0x7f)

            dest.push_back((char)w);

        else if (w <= 0x7ff){

            dest.push_back(0xc0 | ((w >> 6)& 0x1f));

            dest.push_back(0x80| (w & 0x3f));

        }

        else if (w <= 0xffff){

            dest.push_back(0xe0 | ((w >> 12)& 0x0f));

            dest.push_back(0x80| ((w >> 6) & 0x3f));

            dest.push_back(0x80| (w & 0x3f));

        }

        else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){

            dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation

            dest.push_back(0x80| ((w >> 12) & 0x3f));

            dest.push_back(0x80| ((w >> 6) & 0x3f));

            dest.push_back(0x80| (w & 0x3f));

        }

        else

            dest.push_back('?');

    }

}

//! simple warpper

std::string WStrToUTF8(const std::wstring& str){

    std::string result;

    WStrToUTF8(result, str);

    return result;

}

//将Unicode小端转为UTF8  
std::string UCS2ToUTF8(const std::string& src)  
{  
    uint8 *unicode = (uint8*)src.c_str();
    int size = (src.length()+1)*3;
    uint8 *utf8 = new uint8[size];
    
    int u = 0, i = 0;  
  
    memset(utf8, 0, size);  
    while((unicode[u] || unicode[u+1]) && i < size-1)  
    {  
        if(unicode[u] == 0 && unicode[u+1] < 128)  
        {  
            // 0 - 7 bits  
            utf8[i++] = unicode[u+1];  
        }else if((unicode[u] & 0xf8) == 0){  
            // 8 - 11 bits  
            utf8[i++] = 0xc0 | (unicode[u] << 2) | (unicode[u+1] >> 6);  
            utf8[i++] = 0x80 | (unicode[u+1] & 0x3f);  
        }else{  
            // 12 - 16 bits  
            utf8[i++] = 0xe0 | (unicode[u] >> 4);  
            utf8[i++] = 0x80 | ((unicode[u] & 0x0f) << 2) | (unicode[u+1] >> 6);  
            utf8[i++] = 0x80 | (unicode[u+1] & 0x3f);  
        }  
  
        u+=2;  
    }  
    utf8[i] = 0;  
    std::string tmp((char*)utf8);  
    delete []utf8;
    return tmp;
}

// 转换utf8字符串为unicode小端  
int UTF8ToUCS2(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen)  
{  
    uint16 *ucs2;  
    uint8 v_Byte;  
  
    ucs2 = ucs2Buf;  
  
    if (ucs2 && utf8Str) {  
        while ((v_Byte = *utf8Str) && (ucs2MaxLen-- > 0)) {  
            if ((v_Byte & 0x80) == 0) {  
                /* 1 Byte */  
                *ucs2++ = v_Byte;  
                utf8Str++;  
            } else if ((v_Byte & 0xe0) == 0xc0) {  
                /* 2 Bytes */  
                *ucs2++ = ((v_Byte & 0x1f) << 6) | ((*(utf8Str + 1) & 0x3f));  
                utf8Str += 2;  
            } else if ((v_Byte & 0xf0) == 0xe0) {  
                /* 3 Bytes */  
                *ucs2++ = ((v_Byte & 0x0f) << 12)  
                        | ((*(utf8Str + 1) & 0x3f) << 6)  
                        | ((*(utf8Str + 2) & 0x3f));  
                utf8Str += 3;  
            } else {  
                break;  
            }  
        }  
          
        *ucs2 = 0;  
    }  
  
    return ucs2 - ucs2Buf;  
}  

float getBound(float x,float offset,float min,float max) {
    float test = x+offset;
    float bound = max;
    //CCLOG("getBound() x:%0.1f, test:%0.1f, max:%0.1f",x,test,max);
    if(test > bound) {
        return max-offset;
    }

    test = x - offset;
    bound = min;
    //CCLOG("getBound() x:%0.1f, test:%0.1f, min:%0.1f",x, test,min);
    if(test < bound) {
        return offset;
    }

    return x;
}
