/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    svc_mgr_def.hpp
  Author:      ZhongWei
  Created:     2012/11/21 16:58
  Description:
*********************************************************************/

#ifndef  PLATFORM_MODULES_SVCMGR_SVC_MGR_DEF_HPP_
#define  PLATFORM_MODULES_SVCMGR_SVC_MGR_DEF_HPP_


#ifdef WIN32
#ifdef UBP_BUILD_PLATFORM_SVCMGR_DLL
#define SVCMGR_IMPORT_EXPORT __declspec(dllexport)
#else
#define SVCMGR_IMPORT_EXPORT __declspec(dllimport)
#endif
#else
#define SVCMGR_IMPORT_EXPORT
#endif

#define UBP_SVCMGR_SUCCESS  UBP_SUCCESS
#define UBP_SVCMGR_FAIL     UBP_FAIL

#endif  //PLATFORM_MODULES_SVCMGR_SVC_MGR_DEF_HPP_