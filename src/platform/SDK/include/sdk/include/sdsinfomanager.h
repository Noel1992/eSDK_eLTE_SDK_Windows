/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    sdsinfomanager.h
Author:      zhangjiansheng
Created:     2012/12/04 14:00
Description: SDS信息管理类
*********************************************************************/

#ifndef SDSINFO_MANAGER_H
#define SDSINFO_MANAGER_H

#include <list>
#include <map>
#include <vector>
#include <string>
#include "sdk_export.h"

//前向声明 UserStatusIndicator and GroupStatusIndicator

typedef enum {
	IN_BOX = 0,   //收件箱
	OUT_BOX   //发件箱
} msg_box_t;

typedef int MSISDN;

#define ALIAS_MAX_LENGTH 64
#define STMSG_MAX_LENGTH 64

class SDK_IMPORT_EXPORT SdsInfo
{
    public:
        std::string message_t;
        std::string date_t;
        std::string _time_t;
        std::string msgid_t;
        std::string from_t;
        std::string to_t;
        std::string type_t;
        std::string content_t;
        std::string attachfile_t;
        std::string trid_t;
        std::string flag_t;//0:success,1:failed
        std::string group_t;
        std::string subject;
        int direct;//0,接收; 1,发送; 2,双向
        SdsInfo();
        SdsInfo( SdsInfo& sds);
};


typedef std::map<std::string, SdsInfo*> UserSdsinfo;//msgId，即时间和短数据信息的映射关系
typedef std::map<std::string, UserSdsinfo*> AllUserSdsInfoMap_T;//用户和短数据信息的映射关系
typedef std::list<std::string> UserLst_T;//用户和短数据信息的映射关系

class SDK_IMPORT_EXPORT SdsInfoManageritf
{
    friend class SdsInfoManageritf_T;

public:

    //单例模式的访问函数，在非本DLL中只用使用此函数！！！
    static SdsInfoManageritf* getInstance();


    SdsInfoManageritf();
    /*
      SdsInfoManager初始化，从服务器端下载数据
        @param serverIP： 服务器的IP地址
        @param dcid： 当DC调用ProvisionManager时需要设置DCID， 非DC调用ProvisionManager时可以不填
    */
    void init();

    void exit();

    /*
      获得本DC管理的所有Group，返回值为一份克隆的数据，由调用者负责操纵并最终销毁
      @param dcid： 本调度台的用户号，如果 Dcid==0， 则返回全网所有的Group

    */
    UserSdsinfo* getSdsHisRecordByUser(const char* user);
    //添加删除短彩信   ----add by wangshiwei
    UserSdsinfo* getSdsHisRecordByDcid(const char* dcid);

    AllUserSdsInfoMap_T* getAllSdsHisRecord();

    UserLst_T* getSdsUserlst();

    int delSdsHisRecordByUser(const char* user);
    bool delSdsHisRecordByMsgid(const char* msgid, std::string dcid,int del_box_flag);
    void ModifySdsHisRecordByMsgid(const char* msgid,const std::string dcid, std::string flag);
/**
  \brief delete all short messages and MMS
*/
    void delAllSdsHisRecord();

	bool DeciperForExport(msg_box_t box);

	void doBackup(const char* oripath, const char* ackpath);
};

//typedef ACE_Unmanaged_Singleton<ProvisionManager, ACE_Null_Mutex> ProvisionManager_T;

class SDK_IMPORT_EXPORT SdsInfoManageritf_T
{
public:
  /// Global access point to the Singleton.
  static SdsInfoManageritf *instance (void);
};

#endif // SDSINFO_MANAGER_H
