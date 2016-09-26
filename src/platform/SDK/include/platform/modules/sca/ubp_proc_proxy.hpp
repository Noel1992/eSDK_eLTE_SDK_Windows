/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      ubp_proc_proxy.hpp
Author:        ZhongWei
Created:       2013/01/07 18/54
Description:   Proxy of SingleProcess
*********************************************************************/
#ifndef  PLATFORM_MODULES_SCA_UBP_PROC_PROXY_HPP_
#define  PLATFORM_MODULES_SCA_UBP_PROC_PROXY_HPP_

#include <string>
namespace ubp
{
namespace platform
{
namespace sca
{
class ProcessProxy
{
  public:
    virtual std::string QueryServiceState(std::string svc_key) = 0;

};
}
}
}


#endif //PLATFORM_MODULES_SCA_UBP_PROC_PROXY_HPP_
