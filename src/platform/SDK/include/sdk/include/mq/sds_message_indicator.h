/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    sds_message_indicator.h
Author:      zhangjiansheng
Created:     2012/12/12 16:36
Description: Short Data Service message indicator
*********************************************************************/

#ifndef SDS_MESSAGE_INDICATOR_H
#define SDS_MESSAGE_INDICATOR_H

//#include <string>
//#include "ace/Basic_Types.h"
#include "uiparameter.h"
//#include <list>
namespace Json {
    class Value;
}
typedef std::list<std::string> RecvfileList;	/** Receiver list */

/**
  \brief SDS message indicator for short message or multi-media message
  \note Both incoming messages and outgoing message acknowledgements are notified with the same indicator type,
        please check getSdsDirection()==false to get 'incoming text/multimedia messages'.
*/
class SDK_IMPORT_EXPORT SdsMessageIndicator
{
public:
    SdsMessageIndicator(std::string from);
    SdsMessageIndicator(SdsMessageIndicator& copy);
    virtual ~SdsMessageIndicator();
    std::string datalog_str();

    void setSdsType(std::string type);
    void setSdsContent(std::string content);
    void setSdsFrom(std::string from);
    void setSdsTo(std::string to);
    void setSdsFilename(RecvfileList &mmsfilename);
    void setSdsSubject(std::string subject);
    void setTime(std::string date,std::string time);
    void setStatusCode(unsigned int code);
    void setMsgState(MSG_STATE_t msgstate);
    void setSdsDirection(bool direction);
    void setRetCode(std::string retcode);
    void setEmergencyStatusGpId(std::string groupid);
    void setEmergencyStatusUEId(std::string terminalid);
    void setGroupId(std::string groupid);
    //report gis direction,speed
    void setGISDirection(std::string gisdirection);
	void setGISSpeed(std::string gisspeed);

    std::string getSdsType();
    std::string getSdsContent();
    std::string getSdsFrom();
    std::string getSdsTo();
    RecvfileList getSdsFilename();
    std::string getSdsSubject();
    std::string getSdsDate();
    std::string getSdsTime();
    std::string getEmergencyStatusGpId();
    std::string getEmergencyStatusUEId();
    std::string getGroupId();
    unsigned int getStatusCode();
    MSG_STATE_t getMsgState();
    bool getSdsDirection();
    std::string getRetCode();
    std::string getGISDirection();
	std::string getGISSpeed();

    const char* serialize(Json::Value& value);
private:
    std::string _type;			/** Message type, please see uiparameter.h for detailed list */
    std::string _content;		/** Message content */
    std::string _from;			/** Message from (JID) */
    RecvfileList _mmsfilename;	        /** Attach file name (MMS) */
    std::string _subject;	        /** Message subject */
    std::string _date;			/** Date received */
    std::string _time;			/** Time received */
    unsigned int _statusCode;           /** statusCode for StatusMsg, 0 is reserved for emergency **/
    MSG_STATE_t _msgstate;
    std::string _to;                    /** Message to **/
    bool _direction;                    /** SDS receive:false ; SDS send:true **/
    std::string _retcode;
    std::string _emerg_groupid;         /** Group_Call Isdn for Emergency Status **/
    std::string _emerg_ueid;            /** UE Isdn for Emergency Status **/
    std::string _groupid;               /** GroupId for GroupChat**/
    std::string _gis_direction;         /** report gis direction, 0-360**/
	std::string _gis_speed;				/** report gis speed, km/h **/
};


#endif // SDS_MESSAGE_INDICATOR_H
