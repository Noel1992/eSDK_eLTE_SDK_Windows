/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      mq_query_def.hpp
Author:        WuGang
Created:       2013/01/9 17:11
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_QUERY_DEF_HPP_
#define UBP_PLATFORM_QUERY_DEF_HPP_

#ifdef WIN32
#ifdef UBP_MQ_QUERY_EXPORT
#define UBP_MQ_QUERY_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_MQ_QUERY_EXPORT
#define UBP_MQ_QUERY_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_MQ_QUERY_EXPORT
#else//NOT WIN32
#define UBP_MQ_QUERY_IMPORT_EXPORT
#endif//WIN32

#endif  //UBP_PLATFORM_QUERY_DEF_HPP_
