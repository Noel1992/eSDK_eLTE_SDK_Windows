/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    sds_xmpp_zip.h
Author:      zhangjiansheng
Created:     2013/1/8 16:00
Description: 文件打包管理类，用于SDS信息的存储，导出
*********************************************************************/

#ifndef SDS_XMPP_ZIP_
#define SDS_XMPP_ZIP_

#ifdef __cplusplus
extern "C"{
#endif
#include <zip.h>
#include <unzip.h>
#ifdef __cplusplus
}
#endif

#include <string>
#include <list>
#include "sdk_export.h"

class SDK_IMPORT_EXPORT sds_xmpp_zip
{
public:
    sds_xmpp_zip();
    virtual ~sds_xmpp_zip();
    static void AddFileToZip(zipFile zf, const std::string& fileNameInZip, const std::string& srcFile);
    static int  CollectFilesInDirToZip(zipFile zf, const std::string& dirname, const std::string& parentDir);
    static void CreateZipFromDir(const std::string& dirName, const std::string& zipFileName);
    static bool getfiletime(const std::string& filename,tm_zip&  tmzip);
    static void CreateZipFromFile(const std::string& fileName, const std::string& zipFileName);
    static void CreateZipFromFile(const char * password, const std::string& fileName, const std::string& zipFileName);
    static void AddFileToZipEncrypt(zipFile zf, const char* password,const std::string& srcFile);
};

#endif // SDS_XMPP_ZIP_
