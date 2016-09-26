/********************************************************************
Copyright 2014,TD-Tech. Co., Ltd.
Filename:      string_util.hpp
Author:        HuangChunlun
Created:       2014/11/21 15:58
Description:   little function to deal with string
*********************************************************************/
#ifndef UBP_PLATFORM_UTIL_STRING_UTIL_HPP_
#define UBP_PLATFORM_UTIL_STRING_UTIL_HPP_

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <time.h>
#include "ace/Basic_Types.h"


#include "platform/modules/util/no_copy.hpp"
#include "platform/modules/util/platform_util_def.hpp"


namespace ubp
{
namespace platform
{
namespace util
{

using namespace std;

typedef std::vector<std::string> StringList;

class UBP_PLATFORM_UTIL_IMPORT_EXPORT StringUtil: private NonCopyable
{
public:
	StringUtil(void);

	virtual ~StringUtil(void);

	//读取配置文件到map中
	static bool LoadCfg( const std::string fileName, std::map<std::string, std::string> & cfg_map );
	static bool LoadUbpCfg( const std::string fileName, std::map<std::string, std::string> & cfg_map );
	//写配置信息到文件中
	static bool SaveCfg( const std::string file_name, const std::map<std::string, std::string> & cfg_map );
	static bool SaveUbpCfg( const std::string file_name, const std::map<std::string, std::string> & cfg_map );

	static bool IsSubString( const std::string sub_str, const std::string str );
	static bool IsPrefix( const std::string sub_str, const std::string str );
	static bool IsSuffix( const std::string sub_str, const std::string str );
	static bool EraseSuffix( const std::string subfix, std::string & str );
	static bool AddSuffix( const std::string subfix, std::string & str );

	//获取指定目录下的,且使decision_function为真的文件名称列表
	static void ListFile(const std::string &filePath, 	std::vector<std::string> &files,
		const std::string &filter, bool (* decision_function)( const std::string, const std::string ) );
	static void ReplaceRef(::std::string& dataInfo);
};

}//namespace util
}//namespace platform
}//namespace ubp
#endif