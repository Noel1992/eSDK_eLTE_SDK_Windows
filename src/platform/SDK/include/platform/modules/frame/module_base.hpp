/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      module_base.hpp
Author:        ZhongWei
Created:       2013/01/08 16/40
Description:   
*********************************************************************/
#ifndef  PLATFORM_MODULES_FRAME_MODULE_BASE_HPP_
#define  PLATFORM_MODULES_FRAME_MODULE_BASE_HPP_

#include "module.hpp"
#include "module_context.hpp"

namespace ubp
{
namespace platform
{
namespace frame
{
template<typename ITF>
class ModuleBase:public ITF
{
public:
    virtual const std::string& Name(void) const 
    {
        return ITF::ModuleName();
    }

    template<typename T>
    T* ModuleRef()
    {
        T* ptr = NULL;      
        if (context_ != NULL)
        {
            ptr = context_->ModuleRef<T>();
        }
        return ptr;
    }

    virtual ModuleContext * Context(void) 
    {
        return context_;
    }
    virtual ModuleContext const * Context(void) const 
    {
        return context_;
    }
    virtual void Attach(ModuleContext* _context)
    {
        context_ = _context;
    }
protected:
    typedef ModuleBase Base;

    ModuleBase(ModuleContext* _context) :
    context_(_context)
    {
    }

    virtual ~ModuleBase(){};
private:
    ModuleContext* context_;
};
}
}
}


#endif //PLATFORM_MODULES_FRAME_MODULE_BASE_HPP_
