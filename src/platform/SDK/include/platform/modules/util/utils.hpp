/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      utils.hpp
Author:        ZhongWei
Created:       2012/12/31 10/22
Description:   Utils support
*********************************************************************/

#ifndef UBP_PLATFORM_UTIL_UTILS_HPP_
#define UBP_PLATFORM_UTIL_UTILS_HPP_

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <time.h>
#include "ace/Basic_Types.h"

#include "platform/modules/util/no_copy.hpp"
#include "platform/modules/util/platform_util_def.hpp"

//#include "platform/modules/dao/dao_assistant.hpp"
//#include "ubp_svcmgr_svc_info-odb.hxx"

//using namespace ubp::platform::dao;

namespace ubp
{
namespace platform
{
namespace util
{

typedef std::vector<std::string> StringList;

class UBP_PLATFORM_UTIL_IMPORT_EXPORT Utils: private NonCopyable
{
  public:
  	
  	static std::string ReplaceAllDistinct(const std::string& str, const std::string old_value, const std::string new_value);
    //Split "text" with delimiter to a string array(into tokens)
    static void Split(const std::string &text, const std::string &delimiter,
                      std::list<std::string> &tokens);

    static void Split(const std::string &text, const std::string &delimiter,
                      std::vector<std::string> &tokens);

    static void SplitB(const std::string &text, const std::string &delimiter, std::vector<std::string> &tokens);
    //对time_t 类型的时间格式化为可读
    static void TimeFormat(const time_t &source, std::string &dest);

    /*判断指定IP是否是本机上的IP*/
    static bool IsLocalIP(const std::string &ip);

    /*获取本机上的所有IP地址，返回IP地址的个数*/
    static const std::vector<std::string> &GetLocalIP();

    /*初始化ip列表状态*/
    static void RefreshIp();
    
    /*检查文件是否存在*/
    static bool FileExist(const std::string &fullFileName);

    /*创建文件目录*/
    static bool MkDir(const std::string &path);

    /*删除文件*/
	static bool DeleteFile(const std::string &path);

    /*删除目录*/
	static bool DeleteDir(const std::string &path);

    /*取得目录大小*/
	static unsigned int DirSize(const std::string &path);

    /*判断是否为有效目录*/
    static bool Is_Dir(const std::string &path);

    /*获取指定路径下指定文件类型的全文件名*/
    static std::string GetFullFileName(const std::string &filePath
                                       , const char *suffix = NULL);

    //获取UBP的安装路径
    static std::string GetUBPRootPath();

	//获取当前语言
	static std::string GetLang();

	//获取语言目录
	static std::string GetLangResPath();
		
	//获取指定目录下的文件名称列表，可设置文件名称的过滤条件(待优化支持正则)
    static void ListFile(const std::string &filePath,
                         std::vector<std::string> &files,
                         const std::string &filter = "");

    //重命名
    static bool Rename(const std::string &aSrcName,const std::string &aDestName);

    //文件大小
    static bool GetFileSize(const std::string &aName, unsigned long &aSize);

    static ACE_UINT64 GetTickCount();
    static bool IsNameExistsWithPid(const std::string &aName, ACE_INT32 aPid);
    static std::string GetLocalHostName();
    //static void PrintTabData(const vec_table_data &recordset);
    static bool IsStringNull(const std::string& orgString);
    static std::string Int2Str(int a);
    static int Str2Int(const std::string &a);
    static int TrimLeft(std::string& orgString);
    static int TrimRight(std::string& orgString);
    static int TrimLeftRight(std::string& orgString);
    static bool splitteStringEx(const std::string &strToSplitte, const char separator, StringList & vecResult);
    static std::string GetCurTime();
    
    //DES加解密
    static int extend ( int size );
    //static int encryptStr ( unsigned char key[8], unsigned char* data, int blocks = 1 );
    //static int decryptStr ( unsigned char key[8], unsigned char* data, int blocks = 1 );
    static int IsValidIp(const char *ip);
    static int base64_encode_str(const std::string &aIn, std::string &aOut);
    static int base64_decode_str(const std::string &aIn, std::string &aOut);
    //废弃
    static int aes_base64_encode(const std::string &aIn, std::string &aOut);
    //废弃
    static int aes_base64_decode(const std::string &aIn, std::string &aOut);


    /////////////////////////////////////////////////////////////////////
    // Method:      CheckPasswd
    // FullName:    IMessageContext::SendMessage
    // Access:      public
    // Returns:     true:password is valid;
    //              false:password is invalid
    // Description: check password is valid or not
    // Parameter:   const std::string &Plaintext - plain password
    // Parameter:   const std::string &Ciphertext - password encrypted by AES
    /////////////////////////////////////////////////////////////////////
    static bool CheckPasswd(const std::string &Plaintext, const std::string &Ciphertext);
    static bool CheckPasswdCompatible(const std::string &PlaintextOrCiphertext, const std::string &CiphertextOrPlaintext);
    static std::string GetRVersion(const std::string &type);
	static std::string GetBVersion(const std::string &type);
    static ACE_INT32 GetIpBySvcKey(const std::string &fullSvcKey, std::string &destIp);
    //字符串长度不能超过511
    static ACE_INT32 aes_cfb128_enc(const std::string &in, std::string &out);
    //字符串长度不能超过511
    static ACE_INT32 aes_cfb128_dec(const std::string &in, std::string &out);

	static bool Local_charset_to_utf8(const std::string& in, std::string& out);
	static bool Utf8_to_local_charset(const std::string& in, std::string& out);
	static void convert_gbk_to_utf8(const std::string& in_string, std::string& out_string);
	static void convert_utf8_to_gbk(const std::string& in_string, std::string& out_string);
private:
    /*获取本机上的所有IP地址，返回IP地址的个数*/
    static int LoadLocalIP(std::vector<std::string> &local_ips);
    static std::vector<std::string> local_ips_;
    /*ip列表是否初始化的标志位，默认为true：没有初始化*/
    static bool refreshIp_flag;
};

//释放指针对象
#define UBP_RELEASE_PTR(OBJ_POINTER) \
    do { if(OBJ_POINTER != NULL) { \
    delete OBJ_POINTER; \
    OBJ_POINTER = NULL;}  \
    } while (0)

//判断两个字符串是否相等
#define STR_EQ(s1,s2)    (!strcmp ((s1), (s2)))

//判断两个字符串是否不相等
#define STR_NEQ(s1,s2)   (strcmp ((s1), (s2)))

}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_UTIL_UTILS_HPP_
