/********************************************************************
	Copyright 2012,TD-Tech. Co., Ltd.
	Filename: 	   platform_export.hpp
	Author:		   Wu Gang
    Created:       2012/07/11 10:10
	Description: 
*********************************************************************/
#ifndef UBP_PLATFORM_UTIL_PLATFORM_EXPORT_HPP_ 
#define UBP_PLATFORM_UTIL_PLATFORM_EXPORT_HPP_

#ifdef WIN32
#ifdef UBP_BUILD_PLATFORM_DLL
#define PLATFORM_IMPORT_EXPORT __declspec(dllexport)
#define PLATFORM_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define PLATFORM_IMPORT_EXPORT __declspec(dllimport)       
#define PLATFORM_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define PLATFORM_IMPORT_EXPORT 
#define PLATFORM_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

#endif //UBP_PLATFORM_UTIL_PLATFORM_EXPORT_HPP_