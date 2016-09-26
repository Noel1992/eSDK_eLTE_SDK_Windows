/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      locale_def.hpp
Author:        Xu Bingbing
Created:       2013/05/21 17:11
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_LOCALE_DEF_HPP_
#define UBP_PLATFORM_LOCALE_DEF_HPP_

#ifdef WIN32
#ifdef UBP_LOCALE_EXPORT
#define UBP_LOCALE_IMPORT_EXPORT __declspec(dllexport)
#else//NOT UBP_LOCALE__EXPORT
#define UBP_LOCALE_IMPORT_EXPORT __declspec(dllimport)
#endif//UBP_LOCALE_EXPORT
#else//NOT WIN32
#define UBP_LOCALE_IMPORT_EXPORT
#endif//WIN32


#endif  //UBP_PLATFORM_LOCALE_CLIENT_DEF_HPP_
