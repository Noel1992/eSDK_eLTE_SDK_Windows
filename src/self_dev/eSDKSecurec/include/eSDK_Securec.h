/********************************************************************
filename		: 	eSDK_Securec.h
author			:	zwx211831
created			:	2015/3/10
description	:	��ȫ����API�ӿ�;
copyright		:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history			:	2015/3/10 ��ʼ�汾;
*********************************************************************/
#ifndef __ESDK_SECUREC_H__
#define __ESDK_SECUREC_H__

#if defined __GNUC__ || defined LINUX
#include "securec.h"
#endif

#ifdef WIN32 || defined _MSC_VER
#include "securec_win.h"
#endif

/**************************************************************************
* name			: eSDK_MEMCPY
* description	: �ڴ濽��
* input			: 1)destĿ�ĵ�ַ��2)Ŀ���ڴ��С��3)Դ��ַ��4)Ԥ��������
* output			: dest�������
* remark		: ��Ԥ��������count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_MEMCPY(dest, destMax, src, count)								\
	do {																													\
	const size_t copylen = ((count)>(destMax)?(destMax):(count));		\
	memcpy_s(dest, destMax, src, copylen);												\
	} while (0)

/**************************************************************************
* name			: eSDK_WMEMCPY
* description	: ���ֽ��ڴ濽��
* input			: 1)destĿ�ĵ�ַ��2)Ŀ���ڴ��С��3)Դ��ַ��4)Ԥ��������
* output			: dest�������
* remark		: ��Ԥ��������count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_WMEMCPY(dest, destMax, src, count)							\
	do {																													\
	const size_t copylen = ((count)>(destMax)?(destMax):(count));		\
	wmemcpy_s(dest, destMax, src, copylen);											\
	} while (0)

/**************************************************************************
* name			: eSDK_MEMMOVE
* description	: �ڴ��ƶ�
* input			: 1)destĿ�ĵ�ַ��2)Ŀ���ڴ��С��3)Դ��ַ��4)Ԥ�ƶ�����
* output			: dest�ƶ����
* remark		: ��Ԥ�ƶ�����count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_MEMMOVE(dest, destMax, src, count)							\
	do {																													\
	const size_t movelen = ((count)>(destMax)?(destMax):(count));	\
	memmove_s(dest, destMax, src, movelen);											\
	} while (0)

/**************************************************************************
* name			: eSDK_WMEMMOVE
* description	: �ڴ��ƶ�
* input			: 1)destĿ�ĵ�ַ��2)Ŀ���ڴ��С��3)Դ��ַ��4)Ԥ�ƶ�����
* output			: dest�ƶ����
* remark		: ��Ԥ�ƶ�����count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_WMEMMOVE(dest, destMax, src, count)						\
	do {																													\
	const size_t movelen = ((count)>(destMax)?(destMax):(count));	\
	wmemmove_s(dest, destMax, src, movelen);										\
	} while (0)

/**************************************************************************
* name			: eSDK_MEMSET
* description: �ڴ��ʼ������
* input			: 1)destĿ�ĵ�ַ��2)��ʼ���ַ���3)��ʼ������
* output			: ��ʼ������ڴ�
* remark		: 
**************************************************************************/
#define eSDK_MEMSET(dest, c, destMax)		\
	do {																		\
	memset_s(dest, (destMax), c, (destMax)); \
	} while (0)

/**************************************************************************
* name			: eSDK_STRCPY
* description	: �ַ�������
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Դ������
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�������е����ݺͽ�����
							strcpy_s(char* strDest, size_t destMax, const char* strSrc)
**************************************************************************/
#define eSDK_STRCPY strcpy_s

/**************************************************************************
* name			: eSDK_WCSCPY
* description	: ���ֽ��ַ�������
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Դ������
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�������е����ݺͽ�����
							wcscpy_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc)
**************************************************************************/
#define eSDK_WCSCPY wcscpy_s

/**************************************************************************
* name			: eSDK_STRNCPY
* description	: �ַ�������
* input			: 1)Ŀ���ַ�����2)Ŀ�ĳ��ȣ�3)Դ�ִ���4)Ԥ��������
* output			: dest�������
* remark		: ��Ԥ��������count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_STRNCPY(dest, destMax, src, count)											\
    do {																																\
        if (0 < (destMax))																									\
        {																																\
            const size_t copylen = ((count)>=(destMax)?((destMax)-1):(count));\
            strncpy_s(dest, (destMax), src, copylen);													\
        }																																\
    } while (0)

/**************************************************************************
* name			: eSDK_WCSNCPY
* description	: ���ֽ��ַ�������
* input			: 1)Ŀ���ַ�����2)Ŀ�ĳ��ȣ�3)Դ�ִ���4)Ԥ��������
* output			: dest�������
* remark		: ��Ԥ��������count����Ŀ�Ļ����С����ض�
**************************************************************************/
#define eSDK_WCSNCPY(dest, destMax, src, count)										\
	do {																															\
	if (0 < (destMax))																									\
		{																															\
		const size_t copylen = ((count)>=(destMax)?((destMax)-1):(count));\
		wcsncpy_s(dest, (destMax), src, copylen);													\
		}																															\
	} while (0)

/**************************************************************************
* name			: eSDK_STRCAT
* description: �ַ�������
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Դ������
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�ַ�����Ŀ���ַ����ͽ�����
						strcat_s(char* strDest, size_t destMax, const char* strSrc)
**************************************************************************/
#define eSDK_STRCAT strcat_s

/**************************************************************************
* name			: eSDK_WCSCAT
* description	: ���ֽ��ַ�������
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Դ������
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�ַ�����Ŀ���ַ����ͽ�����
						wcscat_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc)
**************************************************************************/
#define eSDK_WCSCAT wcscat_s

/**************************************************************************
* name			: eSDK_STRNCAT
* description	: �ַ�������
* input			: 1)Ŀ���ַ�����2)Ŀ�ĳ��ȣ�3)Դ�ִ���4)���ӳ���
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�ַ�����Ŀ���ַ����ͽ�����
               strncat_s(char* strDest, size_t destMax, const char* strSrc, size_t count)
**************************************************************************/
#define eSDK_STRNCAT strncat_s

/**************************************************************************
* name			: eSDK_WCSNCAT
* description	: ���ֽ��ַ�������
* input			: 1)Ŀ���ַ�����2)Ŀ�ĳ��ȣ�3)Դ�ִ���4)���ӳ���
* output			: dest�������
* remark		: Ŀ�Ļ������������㹻�Ŀռ����洢Դ�ַ�����Ŀ���ַ����ͽ�����
			wcsncat_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc, size_t count)
**************************************************************************/
#define eSDK_WCSNCAT wcsncat_s

/**************************************************************************
* name			: eSDK_STRTOK
* description	: �ָ��ַ���
* input			: 1)Ҫ�ָ���ַ�����2)�ַ����ָ�����3)�������strok_s���λ����Ϣ
* output			: ָ�򱻷ָ�������ַ�����ָ��
* remark		: ���strTokenΪNULL����context��һ��ָ��Ϸ�contextָ���ָ������������ȷ��
						strtok_s(char* strToken, const char* strDelimit, char** context)
**************************************************************************/
#define eSDK_STRTOK strtok_s

/**************************************************************************
* name			: eSDK_WCSTOK
* description	: ���ֽڷָ��ַ���
* input			: 1)Ҫ�ָ���ַ�����2)�ַ����ָ�����3)�������strok_s���λ����Ϣ
* output			: ָ�򱻷ָ�������ַ�����ָ��
* remark		: ���strTokenΪNULL����context��һ��ָ��Ϸ�contextָ���ָ������������ȷ��
						wcstok_s(wchar_t* strToken, const wchar_t* strDelimit, wchar_t** context)
**************************************************************************/
#define eSDK_WCSTOK wcstok_s

/**************************************************************************
* name			: eSDK_SPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)��ʽ�������ַ���������ѡ����
* output			: Ŀ�Ļ�����
* remark		: ��strDest̫С�����ڲ��ܱ���Ҫ��ӡ��ȥ������ʱ��strDest���ֽڻᱻ����Ϊ�գ����ҷ���-1
						sprintf_s�ܹ�ȷ��strDest�е��ַ����Ǵ��������ģ�����strDest�Ĵ�СΪ0��
						sprintf_s(char* strDest, size_t destMax, const char* format, ...)
**************************************************************************/
#define eSDK_SPRINTF sprintf_s

/**************************************************************************
* name			: eSDK_SWPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)��ʽ�������ַ���������ѡ����
* output			: Ŀ�Ļ�����
* remark		: ��strDest̫С�����ڲ��ܱ���Ҫ��ӡ��ȥ������ʱ��strDest���ֽڻᱻ����Ϊ�գ����ҷ���-1
						swprintf_s�ܹ�ȷ��strDest�е��ַ����Ǵ��������ģ�����strDest�Ĵ�СΪ0��
						swprintf_s(wchar_t* strDest, size_t destMax, const wchar_t* format, ...)
**************************************************************************/
#define eSDK_SWPRINTF swprintf_s

/**************************************************************************
* name			: eSDK_SNPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Ҫ�����Ŀ�Ļ������ĸ�ʽ���ַ�������4)��ʽ�������ַ���������ѡ����
* output			: Ŀ�Ļ�����
* remark		: 
						snprintf_s(char* strDest, size_t destMax, size_t count, const char* format, ...)
**************************************************************************/
#if defined WIN32 || defined _MSC_VER
#define eSDK_SNPRINTF  _snprintf_s
#else
#define eSDK_SNPRINTF snprintf_s
#endif

/**************************************************************************
* name			: eSDK_VSPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)��ʽ�������ַ�����4)ָ������б�(va_list)��ָ��
* output			: Ŀ�Ļ�����
* remark		: ��format���зǷ���ʽ��˵����ʱ��vsprintf_s�᷵��-1
						vsprintf_s(char* strDest, size_t destMax, const char* format, va_list arglist)
**************************************************************************/
#define eSDK_VSPRINTF vsprintf_s

/**************************************************************************
* name			: eSDK_VSWPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)��ʽ�������ַ�����4)ָ������б�(va_list)��ָ��
* output			: Ŀ�Ļ�����
* remark		: ��format���зǷ���ʽ��˵����ʱ��vswprintf_s�᷵��-1
						vswprintf_s(wchar_t* strDest, size_t destMax, const wchar_t* format, va_list arglist)
**************************************************************************/
#define eSDK_VSWPRINTF vswprintf_s

/**************************************************************************
* name			: eSDK_VSNPRINTF
* description	: ��ʽ�������Ŀ�Ļ�����
* input			: 1)Ŀ�Ļ�������2)Ŀ�Ļ�������С��3)Ҫ�����Ŀ�Ļ������ĸ�ʽ���ַ�����
						  4)��ʽ�������ַ�����5)ָ������б�(va_list)��ָ��
* output			: Ŀ�Ļ�����
* remark		: ��format���зǷ���ʽ��˵����ʱ��vsnprintf_s�᷵��-1
						Ϊ�˱�֤�ַ�����β�ܹ���ӽ�������һ��Ҫ��֤count�ϸ��С��strDest�ĳ���
						vsnprintf_s(char* strDest, size_t destMax, size_t count, const char* format, va_list arglist)
**************************************************************************/
#define eSDK_VSNPRINTF vsnprintf_s

/**************************************************************************
* name			: eSDK_FSCANF
* description	: ��ʽ��������ļ�
* input			: 1)ָ��FILE�ṹ��ָ�룻2)��ʽ�������ַ���������ѡ����
* output			: ������
* remark		: ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						fprintf_s(FILE* stream, const char * _Format, ...)
**************************************************************************/
#if defined WIN32 || defined _MSC_VER
#define eSDK_FPRINTF fprintf_s
#endif

/**************************************************************************
* name			: eSDK_FSCANF
* description	: ���ļ��������ж�ȡ��ʽ������
* input			: 1)ָ��FILE�ṹ��ָ�룻2)��ʽ�������ַ���������ѡ����
* output			: ������
* remark		: ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						fscanf_s(FILE* stream, const char* format, ...)
**************************************************************************/
#define eSDK_FSCANF fscanf_s

/**************************************************************************
* name			: eSDK_FWSCANF
* description	: ���ļ��������ж�ȡ��ʽ�����ݣ����ֽڰ汾
* input			: 1)ָ��FILE�ṹ��ָ�룻2)��ʽ�������ַ���������ѡ����
* output			: ������
* remark		: ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						fwscanf_s(FILE* stream, const wchar_t* format, ...)
**************************************************************************/
#define eSDK_FWSCANF fwscanf_s

/**************************************************************************
* name			: eSDK_SCANF
* description	: ��׼����������
* input			: 1)��ʽ�������ַ���������ѡ����
* output			: ������
* remark		: ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						scanf_s(const char* format, ...)
**************************************************************************/
#define eSDK_SCANF scanf_s

/**************************************************************************
* name			: eSDK_WSCANF
* description	: ��׼���������룬���ֽڰ汾
* input			: 1)��ʽ�������ַ���������ѡ����
* output			: ������
* remark		: ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						wscanf_s(const char* format, ...)
**************************************************************************/
#define eSDK_WSCANF wscanf_s

/**************************************************************************
* name			: eSDK_SSCANF
* description	: ��׼����������
* input			: 1)Ŀ���ַ�����2)��ʽ������
* output			: ������
* remark		: ��ѡ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						���buffer���������ָ��֮����ڴ�����ص�����sscanf_s����Ϊδ����
						sscanf_s(const char* buffer, const char* format, ...)
**************************************************************************/
#define eSDK_SSCANF sscanf_s

/**************************************************************************
* name			: eSDK_SWSCANF
* description	: ��׼���������룬���ֽڰ汾
* input			: 1)Ŀ���ַ�����2)��ʽ������
* output			: ������
* remark		: ��ѡ���������ͱ������ʽ�������ַ�����format���е����ͱ���һ��
						���buffer���������ָ��֮����ڴ�����ص�����swscanf_s����Ϊδ����
						swscanf_s(const wchar_t* buffer, const wchar_t* format, ...)
**************************************************************************/
#define eSDK_SWSCANF swscanf_s

/**************************************************************************
* name			: eSDK_GETS
* description	: ��׼����������
* input			: 1)Ŀ���ַ�����2)Ŀ�ĳ���
* output			: ������
* remark		: gets_s(char* buffer, size_t destMax)
**************************************************************************/
#define eSDK_GETS gets_s


#endif

