/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      thread_def.hpp
Author:        XiaoLongguang
Created:       2012/12/29 14:49
Description:
*********************************************************************/

#ifndef PLATFORM_MODULES_THREAD_THREAD_DEF_HPP_
#define PLATFORM_MODULES_THREAD_THREAD_DEF_HPP_


#define UBP_THREAD_SUCCESS 0
#define UBP_THREAD_FAIL   -1

#ifdef WIN32
#ifdef THREAD_EXPORT
#define THREAD_IMPORT_EXPORT __declspec(dllexport)
#define THREAD_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define THREAD_IMPORT_EXPORT __declspec(dllimport)
#define THREAD_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define THREAD_IMPORT_EXPORT
#define THREAD_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

#endif  //PLATFORM_MODULES_THREAD_THREAD_DEF_HPP_
