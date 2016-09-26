/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      mq_export.hpp
Author:        Wu Gang
Created:       2013/01/08 16/00
Description:   
*********************************************************************/

#ifndef  UBP_PLATFORM_MOUDLES_MQ_EXPORT_HPP_
#define  UBP_PLATFORM_MOUDLES_MQ_EXPORT_HPP_

//////////////////////////////////////////////////////////////////////////
// 定义此宏是为了解决WINDOWS平台单实例类对象在dll和exe中并非同一实例的bug
// dll中需要export，使用时需要import
//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#ifdef MQ_EXPORT
#define MQ_IMPORT_EXPORT __declspec(dllexport)
#define MQ_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#else//NOT MQ_EXPORT
#define MQ_IMPORT_EXPORT __declspec(dllimport)
#define MQ_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
    extern template class SINGLETON_TYPE <CLASS, LOCK>;
#endif//MQ_EXPORT
#else//NOT WIN32
#define MQ_IMPORT_EXPORT
#define MQ_IMPORT_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif//WIN32

#endif  //UBP_PLATFORM_MOUDLES_MQ_EXPORT_HPP_