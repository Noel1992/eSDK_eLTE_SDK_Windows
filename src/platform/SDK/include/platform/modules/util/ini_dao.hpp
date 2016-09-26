/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    ini_dao.hpp
  Author:      Wu Gang
  Created:     2014/02/15 11:37
  Description:

*********************************************************************/
#ifndef UBP_PLATFORM_UTIL_INI_DAO_HPP_
#define UBP_PLATFORM_UTIL_INI_DAO_HPP_
#define BOOST_SPIRIT_THREADSAFE
#include <ace/Basic_Types.h>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  
#include "platform/modules/util/util_trace.hpp"
#include "platform_util_def.hpp"


namespace ubp
{
namespace platform
{
namespace util
{

class UBP_PLATFORM_UTIL_IMPORT_EXPORT IniDao
{
  public:
    IniDao();
    virtual ~IniDao();

    ACE_INT32 Parse(const std::string& filename);
	ACE_INT32 Write(const std::string& filename);
    std::string GetProperty(const std::string& key);

	//iniDao.SetPropertyT("ccc", std::string("ddd"));
	//iniDao.Write("./test.ini");
	template<typename T>
	ACE_INT32 SetPropertyT(const std::string& key, const T &value);

    template<typename T>
    T GetProperty(const std::string& key)
	{
		T t;
		try
		{
			t=pt_property_.get<T>(key); 
		}
		catch(std::exception &e)
		{
			UTIL_ERROR("IniDao::GetProperty::"<<e.what());
		}
		catch(...)
		{
			UTIL_ERROR("IniDao::GetProperty::catch unknown exception");
		}
		return t; 
	}

    template<typename T>
    T GetProperty(const std::string& key, const T &default_value)
	{
		T t;
		try
		{
			t=pt_property_.get<T>(key, default_value); 
		}
		catch(std::exception &e)
		{
			UTIL_ERROR("IniDao::GetProperty::default_value"<<e.what());
		}
		catch(...)
		{
			UTIL_ERROR("IniDao::GetProperty::default_value catch unknown exception");
		}
		return t; 
	}

private:  
    short int err_code_;  
    boost::property_tree::ptree pt_property_;  
};

}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_UTIL_INI_DAO_HPP_