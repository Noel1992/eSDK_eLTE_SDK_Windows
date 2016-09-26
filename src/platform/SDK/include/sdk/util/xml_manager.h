/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    xml_manager.h
Author:      zhangjiansheng
Created:     2014/06/23 10:00
Description: 日志管理类
*********************************************************************/
#ifndef XML_MANAGER_H
#define XML_MANAGER_H

#include <ace/Basic_Types.h>
#include "sdk_export.h"
#include "xercesc/dom/DOM.hpp"

#include "xercesc/parsers/XercesDOMParser.hpp"


XERCES_CPP_NAMESPACE_USE

class SDK_IMPORT_EXPORT xml_manager
{
public:
    xml_manager();

    static void Initialize();
    static void Terminate();
    static void DoOutput2File(xercesc::DOMDocument* pmyDOMDocument, const char * FullFilePath);
};


class LStr
{
public :

    LStr(const char* const toTranscode)
    {
        UnicodeForm = XMLString::transcode(toTranscode);
    }

    ~LStr()
    {
        XMLString::release(&UnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* _unicodeForm() const
    {
        return UnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  UnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   UnicodeForm;
};

#define X(str) LStr(str)._unicodeForm()


class CXMLStr
{
public :

    CXMLStr(const XMLCh* const toTranscode)
    {
        UnicodeForm = XMLString::transcode(toTranscode);
    }

    ~CXMLStr()
    {
        XMLString::release(&UnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* _unicodeForm() const
    {
        return UnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  UnicodeForm
    //      This is the Unicode char format of the string.
    // -----------------------------------------------------------------------
    char* UnicodeForm;
};

#define XMLStr(str) CXMLStr(str)._unicodeForm()


#endif // XML_MANAGER_H
