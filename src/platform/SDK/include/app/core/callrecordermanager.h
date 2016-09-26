/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    callrecordmanager.hpp
Author:      zhangjiansheng
Created:     2012/07/20 10:00
Description:
*********************************************************************/
#ifndef CALLRECORDMANAGER_H
#define CALLRECORDMANAGER_H

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "common.h"
#include "util/sync.h"
#include <map>
#include <list>
#include <memory>
#include "sdk/include/sdk_export.h"
#include "sdk/util/xml_manager.h"
#include <json/json.h>
#include "logger.h"

namespace Json{
	class Value;
}

#define  MAX_WAITING_CALL  10
#define  MAX_HIS_RECORD_NUM  2000
class CallInfo;
class CallRecordManager;
class CallRecordInfo;

template <class TYPE, class ACE_LOCK> class ACE_Singleton;
class ACE_Null_Mutex;

//typedef ACE_Singleton<CallRecordManager, ACE_Null_Mutex> CallRecordManager_T;

typedef std::map<std::string, CallRecordInfo*> CallRecordMap;//呼叫记录ID与呼叫记录的映射关系

typedef std::map<int, std::string> RcordIDMap;//当前呼叫的callid和呼叫记录ID的映射关系

typedef std::map<std::string, CallRecordInfo *>::iterator RecordMapIterator;
typedef std::map<int, std::string>::iterator RIDMapIterator;

//业务类型：
const std::string RECORD_SERVICE_TYPE_P2P	= "P2P Audio";
const std::string RECORD_SERVICE_TYPE_PTT       = "PTT Audio";
const std::string RECORD_SERVICE_TYPE_VIDEO  	= "Video";

//呼叫类型
const std::string RECORD_CALL_TYPE_NORMAL       = "Normal";
const std::string RECORD_CALL_TYPE_EMERGENCY	= "Emergency";

//呼叫结果
const std::string RECORD_RESULT_TYPE_MISS       = "Missed";
const std::string RECORD_RESULT_TYPE_ANSWER	 = "Answered";
const std::string RECORD_RESULT_TYPE_DIAL	 = "Dialed";

//呼叫方向
const std::string RECORD_DIRECTION_TYPE_IN	 = "In";
const std::string RECORD_DIRECTION_TYPE_OUT	 = "Out";
//缺省时长
const std::string RECORD_DEFAULT_DURATION	 = "00:00:00";

//所有
const std::string FILTER_ALL	 = "All";

typedef enum {
    LOCAL_ANSWER = 0,   //本端接听
    PEER_ANSWER,        //对端接听
    HANGUP              //挂断
} action_t;


/**
  define CallRecord Detail object
*/
class SDK_IMPORT_EXPORT CallRecordInfo
{
    public:
        std::string call_record_id;  //呼叫记录ID
        std::string from;            //主叫
        std::string to;              //被叫
        std::string time;            //呼叫发起或接收时间
        std::string begin_time;      //业务开始时间
        std::string end_time;        //业务结束时间
        std::string servive_type;            //业务类型
        std::string call_type;               //呼叫类型：0,普通呼叫; 1,紧急呼叫
        std::string call_result;     //呼叫结果
        std::string duration;        //通话时长;
        std::string direct;          //呼叫方向：0,接收; 1,发送
        CallRecordInfo ();
        CallRecordInfo (CallRecordInfo * callrecord);
		std::string datalog_str();
		bool serialize(Json::Value &log);
};

/**
  define CallRecordManager abstract interface
*/
class CallRecordManagerItf
{

public:
        CallRecordManagerItf(){;}
        virtual ~CallRecordManagerItf(){;}

        virtual void init(){;}
        virtual bool LoadHisRecordfromXml(){return true;}

        /* 根据callinfo, 创建CallRecord信息 */
	virtual void createCallRecord(CallInfo * call,std::string dir){;}

	virtual void updateCallRecord(int cid, action_t action){;}

	virtual CallRecordInfo* getRecordbycid(int cid){return NULL;}
};

/**
  define one CallRecordManager implementation using XML for DC
  later on we may have different implementations for different scenarios
*/
class SDK_IMPORT_EXPORT CallRecordManager : public CallRecordManagerItf
{
    friend class ACE_Singleton<CallRecordManager, ACE_Null_Mutex>;

public:
        CallRecordManager();
        ~CallRecordManager();
        void init();
        bool LoadHisRecordfromXml();

        /* 根据callinfo, 创建CallRecord信息 */
        void createCallRecord(CallInfo * call,std::string dir);

        void updateCallRecord(int cid, action_t action);

        CallRecordInfo* getRecordbycid(int cid);
        CallRecordMap* getAllHisRecord();
        CallRecordMap* getHisRecordbyfilter(std::string calltype,std::string user = FILTER_ALL);
        void delSdsHisRecordBycallid(std::string callid);

protected:
        void FreeRecordDoc();
        void reset();
        void saveCallRecord(CallRecordInfo *record,bool isupdate);
        void UpdateCallRecord(CallRecordInfo* pCallRecordInfo);
        DOMDocument * CreateDOMDoc();

        bool isRecordInNode(const DOMNode* xCode,const char* callid);
        void delRecordFromXml(std::string callid);
        void delRecordFromMap(std::string callid);
        void delAllHisRecord();

        bool is_record_overflow();
        void remove_record_first();
        void re_init_tree();
//        CallRecordInfo * getHisRecordByUser(const char* user);
private:
        CallRecordInfo* getRecordFromXml(const DOMNode* xCode);

        bool encryptCallXML();
        bool reEncryptCallXML();

        /* 操作锁*/
        utils::RW_Thread_Mutex _recordmutex;

        CallRecordMap m_recordMap;

        RcordIDMap m_ridMap;
        XercesDOMParser* m_DOMXmlParser;
        DOMDocument *m_RecordDoc;
        std::string m_recordfile;
        std::string m_owner;

};

class SDK_IMPORT_EXPORT CallRecordManager_T
{
public:
	static CallRecordManager* instance(void);

private:
	static CallRecordManager* m_instance;
	static utils::RW_Thread_Mutex _mutex;

};


#endif // CALLRECORDMANAGER_H
