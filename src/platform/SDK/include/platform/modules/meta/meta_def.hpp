/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      meta_def.hpp
Author:        
Created:       2014/04/29 14:49
Description:
*********************************************************************/

#ifndef PLATFORM_MODULES_META_SERVICE_DEF_HPP_
#define PLATFORM_MODULES_META_SERVICE_DEF_HPP_


#ifdef WIN32
#ifdef META_SERVICE_EXPORT
#define META_SERVICE_IMPORT_EXPORT __declspec(dllexport)
#define META_SERVICE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else
#define META_SERVICE_IMPORT_EXPORT __declspec(dllimport)
#define META_SERVICE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif
#else
#define META_SERVICE_IMPORT_EXPORT
#define META_SERVICE_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif

#endif  //PLATFORM_MODULES_META_SERVICE_DEF_HPP_
