/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      thread_def.hpp
Author:        XiaoLongguang
Created:       2012/01/07 14:45
Description:
*********************************************************************/
#ifndef  PLATFORM_MODULES_THREAD_UBP_META_SERVICE_HPP_
#define  PLATFORM_MODULES_THREAD_UBP_META_SERVICE_HPP_

#include <ace/Method_Request.h>
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "platform/modules/error/ubp_error.hpp"
#include "meta_def.hpp"
#include "cm/cm_mocoperator.h"

namespace ubp
{
namespace platform
{
namespace meta
{

class META_SERVICE_IMPORT_EXPORT MetaService
      : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    //static MetaService *Instance(void);

    //通过MOC名称获取对应的Meta信息，需要根据返回值先判断mocmeta是否存在，然后才能使用
    virtual bool getMocMetaByName(const string& mocname, MocMeta& mocmeta)=0;

    virtual bool getMocOperatorByName(const string& mocname, vector<MocOperator*>& opvec)=0;
	virtual bool getNotifyOperator(vector<MocOperator*>& opvec)=0;
    
    //获取带有密码属性的MocMeta列表
    virtual void getHasPassAttrMocMeta(vector<MocMeta>& mocvec)=0;
};

}
}
}

#endif 
