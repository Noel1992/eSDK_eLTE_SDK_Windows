/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    error_module.hpp
Author:      Wu Gang
Created:     2012/08/21 10:08
Description:
*********************************************************************/
#ifndef  UBP_PLATFORM_MODULES_ERROR_ERROR_MODULE_HPP_
#define  UBP_PLATFORM_MODULES_ERROR_ERROR_MODULE_HPP_

#ifdef WIN32
#ifdef UBP_PLATFORM_ERROR_EXPORT
#define UBP_PLATFORM_ERROR_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_PLATFORM_ERROR_EXPORT
#define UBP_PLATFORM_ERROR_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_PLATFORM_ERROR_EXPORT
#else//NOT WIN32
#define UBP_PLATFORM_ERROR_IMPORT_EXPORT
#endif//WIN32

#include "ace/Basic_Types.h"
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"


namespace ubp
{
namespace platform
{
namespace error
{

class UBP_PLATFORM_ERROR_IMPORT_EXPORT ErrorInfo
{
  public:
    ErrorInfo();
    ErrorInfo(ACE_INT64 code, const std::string& message, const std::string& suggestion);
    virtual ~ErrorInfo();

  public:
    ACE_INT64 code_;
    std::string message_;
    std::string suggestion_;
};


class UBP_PLATFORM_ERROR_IMPORT_EXPORT ErrorModule
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    virtual ACE_INT32 Init() = 0;

    virtual ACE_INT32 Fini() = 0;

    virtual const ErrorInfo *GetErrorInfo(ACE_INT64 code) const = 0;

    //virtual void PrintError(long long code) = 0;

    virtual ACE_INT32 LoadErrorInfo(const std::string &filename) = 0;
};

}//error
}//platform
}//ubp

#endif //UBP_PLATFORM_MODULES_ERROR_ERROR_MODULE_HPP_
