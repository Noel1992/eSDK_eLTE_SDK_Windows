/********************************************************************
        Copyright 2012,TD-Tech. Co., Ltd.
        Filename: 	   platform_export.hpp
        Author:		   Wu yu wei
    Created:       2012/11/25 10:10
        Description:
*********************************************************************/
#ifndef MEDIA_EXPORT_H
#define MEDIA_EXPORT_H

#ifdef WIN32
#ifdef  MEDIA_BUILD_DLL //CMake中定义
#define MEDIA_IMPORT_EXPORT __declspec(dllexport)
#define MEDIA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define MEDIA_IMPORT_EXPORT __declspec(dllimport)
#define MEDIA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define MEDIA_IMPORT_EXPORT
#define MEDIA_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

typedef int MSISDN;

#endif // MEDIA_EXPORT_H
