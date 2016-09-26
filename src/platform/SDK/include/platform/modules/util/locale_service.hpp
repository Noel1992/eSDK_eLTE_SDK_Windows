/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    locale_service.hpp
  Author:      Xu Bingbing
  Created:     2013/05/21 15:53
  Description:

*********************************************************************/
#ifndef    UBP_PLATFORM_LOCALE_SERVICE_HPP_
#define    UBP_PLATFORM_LOCALE_SERVICE_HPP_

#include "platform/modules/util/locale_def.hpp"
#include <stdio.h>
//#include <locale.h>
//#include <libintl.h>
#include <string>
#include <map>
#include <ace/Process_Manager.h>
#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <ace/Singleton.h>

/************************************************************************/
/* 实现LOCALE功能                                 */
/************************************************************************/
namespace ubp
{
namespace platform
{
namespace util
{
const std::string LOCALE_ID_ZH_CN = "zh_CN";
const std::string LOCALE_ID_EN = "en";
const std::string LOCALE_STR_WIN_ZH_CN = "Chinese_People's Republic of China.936";
const std::string LOCALE_STR_LINUX_ZH_CN = "zh_CN.utf8";
const std::string LOCALE_UBP = "ubp";

typedef std::map<std::string, std::string> KeyValuesMap_T;
class UBP_LOCALE_IMPORT_EXPORT LocaleService
{
	friend class ACE_Singleton<LocaleService, ACE_Null_Mutex>;
public:    
    static int Init(const std::string &module);
	static const std::string getCurrentLocale();

	static const char* GetText(const char*);
    
protected:
	void InitLocale(const std::string& dir);
	void InitKeyValueMap(const std::string& dir,const std::string& cfgfile);

private:
	LocaleService();
	virtual ~LocaleService();




private:
	std::string locale_;
	KeyValuesMap_T keyValueMap_;
	static ACE_Thread_Mutex map_mutex_;
	static bool bInited;

    //std::string module_;


};

typedef ACE_Unmanaged_Singleton<LocaleService, ACE_Null_Mutex> LocaleService_T;
}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_LOCALE_SERVICE_HPP_
