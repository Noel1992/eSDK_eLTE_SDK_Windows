/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      module_export.hpp
Author:        ZhongWei
Created:       2013/01/08 16/00
Description:   
*********************************************************************/

#ifndef  PLATFORM_FRAME_MODULE_EXPORT_HPP_
#define  PLATFORM_FRAME_MODULE_EXPORT_HPP_

#ifdef WIN32
#ifdef UBP_BUILD_PLATFORM_FRAME_DLL
#define FRM_IMPORT_EXPORT __declspec(dllexport)
#else
#define FRM_IMPORT_EXPORT __declspec(dllimport)
#endif
#else
#define FRM_IMPORT_EXPORT
#endif

#ifdef WIN32
#define MODULE_EXPORT __declspec(dllexport)
#else
#define MODULE_EXPORT
#endif

#endif  //PLATFORM_FRAME_MODULE_EXPORT_HPP_