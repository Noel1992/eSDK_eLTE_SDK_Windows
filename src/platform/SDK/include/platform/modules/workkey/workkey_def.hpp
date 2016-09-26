/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      mq_query_def.hpp
Author:        ZhouYuanFu
Created:       2014/12/9 17:11
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_WORKKEY_DEF_HPP_
#define UBP_PLATFORM_WORKKEY_DEF_HPP_

#ifdef WIN32
#ifdef UBP_WORKKEY_EXPORT
#define UBP_WORKKEY_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_WORKKEY_EXPORT
#define UBP_WORKKEY_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_WORKKEY_EXPORT
#else//NOT WIN32
#define UBP_WORKKEY_IMPORT_EXPORT
#endif//WIN32

#endif  //UBP_PLATFORM_WORKKEY_DEF_HPP_
