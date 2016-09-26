/********************************************************************
Copyright 2014,TD-Tech. Co., Ltd.
Filename:      
Author:        
Created:       
Description:
 *********************************************************************/
#ifndef  PLATFORM_MODULES_UBP_CERT_MAN_HPP_
#define  PLATFORM_MODULES_UBP_CERT_MAN_HPP_

#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "platform/modules/error/ubp_error.hpp"
#include "certman_def.hpp"

namespace ubp
{
namespace platform
{
namespace cert_man
{

class CERTMAN_IMPORT_EXPORT CertManAgent
: public ubp::platform::frame::Module
{
public:

	//Module must support a static function "ModuleName"
	static const std::string &ModuleName(void);

	/* 获取私钥存储密码 */
	virtual std::string getPrivateKeyPassword() = 0;


};

}
}
}

#endif //PLATFORM_MODULES_UBP_CERT_MAN_HPP_
