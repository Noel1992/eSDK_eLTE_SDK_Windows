/********************************************************************
        Copyright 2012,TD-Tech. Co., Ltd.
        Filename: 	   platform_export.hpp
        Author:		   Wu yu wei
    Created:       2012/11/25 10:10
        Description:
*********************************************************************/
#ifndef SDK_EXPORT_H
#define SDK_EXPORT_H

#ifdef WIN32
#ifdef  SDK_BUILD_DLL //CMake中定义
#define SDK_IMPORT_EXPORT __declspec(dllexport)
#define SDK_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define SDK_IMPORT_EXPORT __declspec(dllimport)
#define SDK_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define SDK_IMPORT_EXPORT
#define SDK_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

typedef int MSISDN;
typedef unsigned long CALLID_T;

typedef int Op_Return_t;

#endif // SDK_EXPORT_H
