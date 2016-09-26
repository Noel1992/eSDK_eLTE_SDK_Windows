/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      module_context.hpp
Author:        ZhongWei
Created:       2013/01/08 15/18
Description:   
*********************************************************************/
#ifndef  PLATFORM_MODULES_FRAME_MODULE_CONTEXT_HPP_
#define  PLATFORM_MODULES_FRAME_MODULE_CONTEXT_HPP_

#include <string>
#include "ace/Basic_Types.h"
#include "frame_system.hpp"


namespace ubp
{
namespace platform
{
namespace frame
{

class ModuleContext
{
public:
    virtual ~ModuleContext(){};

    virtual ACE_INT32 Init() = 0;

    virtual ACE_INT32 Fini() = 0;

    //从文件中增量加载所有的Module，module_file为ModuleDef文件的全路径
    virtual ACE_INT32 LoadModulesFile(const std::string& module_file) = 0;

    //从动态库lib_name中调用entry_name的接口，创建一个Module对象，并调用其Init()
    virtual ACE_INT32 LoadModule(const std::string& lib_name,
        const std::string& entry_name)= 0;

    //卸载一个Module对象，调用其Fini()，然后释放其对象
    virtual ACE_INT32 UnloadModule(const std::string& module_name)=0;

    //将一个创建好的Module对象，放入context中管理，但对象须由用户自己负责释放
    virtual ACE_INT32 AttachModule(Module* module) = 0;

    //取一个Module对象
    virtual Module* FindModule(const std::string& module_name) = 0; 

    //取一个指定接口类型的Module对象指针
    template<typename T>
    T* ModuleRef();

    //创建ModuleContext
    static ModuleContext* CreateContext(void);
};

template<typename T>
T* ModuleContext::ModuleRef()
{
    Module* buf= FindModule(T::ModuleName());
    T* r = dynamic_cast<T*>(buf);
    return r;
}

}
}
}


#endif //PLATFORM_MODULES_FRAME_MODULE_CONTEXT_HPP_
