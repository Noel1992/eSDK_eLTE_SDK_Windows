/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    code_util.h
Author:      zhangjiansheng
Created:     2012/07/20 10:00
Description:
*********************************************************************/
#ifndef SDS_CUTILITY_H
#define SDS_CUTILITY_H
#include "common.h"
#include "sdk_export.h"

class SDK_IMPORT_EXPORT Code_Utility
{
public:    

    static int code_convert(const char *from_charset,const char *to_charset,const char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    static int dc_code_convert(const char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    static int dc_decode_convert(const char *inbuf,size_t inlen,char *outbuf,size_t outlen);

    /* UTF-8编码方式转换为UNICODE编码 */
    static int FUTF82WcharConvert(const char* a_szSrc, int a_nSrcSize, wchar_t* a_szDest, int a_nDestSize );
    static int FWchar2UTF8Convert(const wchar_t* a_szSrc,int a_nSrcSize, char* a_szDest, int a_nDestSize );

 private:
    Code_Utility();
    void multiToUnicode(const std::string& multiText, std::wstring& unicodeText);

};

#endif // SDS_CUTILITY_H
