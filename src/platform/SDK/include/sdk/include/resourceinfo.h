/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    ResourceInfo.h
Author:      Wu yu wei
Created:     2012/07/26 14:00
Description:
*********************************************************************/
#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#include <string>
#include "sdk_export.h"
#include "common.h"

typedef int MSISDN;

/**
  \brief resource info definition
  \note for DC_LOGIN/DC_LOGOUT operations, all parameters are necessary
        for all other operations, only resId matters
*/
class SDK_IMPORT_EXPORT ResourceInfo
{
public:
    ResourceInfo(MSISDN resId);
    virtual ~ResourceInfo();
    virtual std::string datalog_str();

    void setResId(MSISDN rid);
    const MSISDN getResId();

    const char* getUserName();
    const char* getPassWord();
    const char* getServerIp();
    const char* getLocalIp();
    const char* getSipPort();

    void setUserName(const char* user);
    void setPassWord(const char* pwd);
    void setServerIp(const char* serverip);
    void setLocalIp(const char* localip);
    void setSipPort(const char* sipport);

    MSISDN resId; /** resource ID, maybe user ID or group ID */

private:

    char user_name[LOGIN_FIELD_LENG];       /** user name, the same as resId but in string format */
    char password[LOGIN_FIELD_LENG*100];    /** user password */
    char server_ip[LOGIN_FIELD_LENG];       /** server IP address */
    char local_ip[LOGIN_FIELD_LENG];        /** local IP address */
    char sip_port[LOGIN_FIELD_LENG];        /** local SIP port */
};

/**
    \brief subclass of ResourceInfo used for DynamicGroup
*/
class SDK_IMPORT_EXPORT DGResourceInfo : public ResourceInfo
{
public:
    DGResourceInfo(MSISDN resId, MSISDN dcid);
    ~DGResourceInfo();
    virtual std::string datalog_str();

protected:
    MSISDN dcid;
};

/**
    \brief subclass of ResourceInfo used for TelephoneInterconnect(TIC)
    \note telNumber is the telephone number for this TIC service
*/
class SDK_IMPORT_EXPORT TIResourceInfo : public ResourceInfo
{
public:
    TIResourceInfo(MSISDN resId, std::string telNumber);
    ~TIResourceInfo();
    const char* getTeleNumber();
    virtual std::string datalog_str();
protected:
    char telNumber[MAX_TELENUMBER_LENGTH];
};

#endif // RESOURCEINFO_H
