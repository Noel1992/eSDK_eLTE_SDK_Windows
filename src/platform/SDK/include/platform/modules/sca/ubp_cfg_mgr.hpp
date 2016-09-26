/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      ubp_cfg_mgr.hpp
Author:        ZhongWei
Created:       2012/12/28 15/13
Description:   interface for ConfigMgr
*********************************************************************/
#ifndef  PLATFORM_MODULES_SCA_UBP_CFG_MGR_HPP_
#define  PLATFORM_MODULES_SCA_UBP_CFG_MGR_HPP_

#include <string>
#include <map>
#include <vector>
#include "ace/Basic_Types.h"
#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "sca_def.hpp"

namespace ubp
{
namespace platform
{
namespace sca
{
class SCA_IMPORT_EXPORT ConfigMgr
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    //Todo:　后面加入本Module对外提供的接口

    virtual ACE_INT32 LoadConfig(const std::string& service_name) = 0;
    virtual ACE_INT32 ReLoadCfg(const std::string& service_type) = 0;
    virtual ACE_INT32 AttachConfig(const std::string& filename) = 0;
    virtual const std::string GetParam(const std::string& param_name) = 0;
    virtual const std::vector<std::pair<std::string,std::string> >& GetModules() = 0;

    static ConfigMgr *CreateModule(ubp::platform::frame::ModuleContext *context);
};
}
}
}


#endif //PLATFORM_MODULES_SCA_UBP_CFG_MGR_HPP_
