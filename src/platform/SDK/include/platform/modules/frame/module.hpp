/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      module.hpp
Author:        ZhongWei
Created:       2013/01/08 15/09
Description:   interface of Module
*********************************************************************/
#ifndef  PLATFORM_MODULES_FRAME_MODULE_HPP_
#define  PLATFORM_MODULES_FRAME_MODULE_HPP_

#include <string>
#include "ace/Basic_Types.h"

#include "frame_system.hpp"

namespace ubp
{
namespace platform
{
namespace frame
{
class Module
{
public:
    virtual ACE_INT32 Init(void) = 0;
    virtual ACE_INT32 Fini(void) = 0;

    //各个具体模块实现，模块的名字在同一个配置服务中不可以重复，模块互相之间通过名字寻找
    virtual const std::string& Name(void) const = 0;

    virtual ModuleContext * Context(void) = 0;
    virtual ModuleContext const * Context(void) const = 0;
    virtual void Attach(ModuleContext* _context) = 0;
    virtual ~Module(){};
};

}
}
}


#endif //PLATFORM_MODULES_FRAME_MODULE_HPP_
