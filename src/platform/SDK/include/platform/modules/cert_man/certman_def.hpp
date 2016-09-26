/********************************************************************
Copyright 2014,TD-Tech. Co., Ltd.
Filename:     
Author:      
Created:      
Description:
 *********************************************************************/

#ifndef PLATFORM_MODULES_CERT_MAN_DEF_HPP_
#define PLATFORM_MODULES_CERT_MAN_DEF_HPP_

#define UBP_CERTMAN_SUCCESS 0
#define UBP_CERTMAN_FAIL   -1


#ifdef WIN32
#ifdef UBP_CERT_MAN_EXPORT
#define CERTMAN_IMPORT_EXPORT __declspec(dllexport)
#else
#define CERTMAN_IMPORT_EXPORT __declspec(dllimport)
#endif
#else
#define CERTMAN_IMPORT_EXPORT
#endif

#endif  //PLATFORM_MODULES_CERT_MAN_DEF_HPP_
