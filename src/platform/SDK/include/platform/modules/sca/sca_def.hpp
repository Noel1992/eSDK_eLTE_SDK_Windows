/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    sca_def.hpp
  Author:      ZhongWei
  Created:     2012/11/21 16:58
  Description:
*********************************************************************/

#ifndef  PLATFORM_MODULES_SCA_SCA_DEF_HPP_
#define  PLATFORM_MODULES_SCA_SCA_DEF_HPP_

#ifdef WIN32
#ifdef SCA_EXPORT
#define SCA_IMPORT_EXPORT __declspec(dllexport)
#define SCA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define SCA_IMPORT_EXPORT __declspec(dllimport)
#define SCA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define SCA_IMPORT_EXPORT
#define SCA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

#endif  //PLATFORM_MODULES_SCA_SCA_DEF_HPP_