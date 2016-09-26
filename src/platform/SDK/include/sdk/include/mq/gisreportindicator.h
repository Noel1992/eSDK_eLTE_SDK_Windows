/*---------------------------------------------------------
Copyright 2013,TD-Tech. Co., Ltd.
Filename:    gisreportindicator.h
Author:      wu yu wei
Created:     2013/01/24 16:36
Description: GIS Report Indicator
-------------------------------------------------------------*/
#pragma warning(disable: 4251)
#ifndef GISREPORTINDICATOR_H
#define GISREPORTINDICATOR_H

#include <string>
#include <sdk_export.h>
#include <map>

typedef enum ack_rsp
{
    ACK_SUCCESS,    /** SDS server return success */
    ACK_FAIL        /** SDS server return failure */
}ack_rsp_t;


typedef enum reason_code
{
    PERIOD_RPT,     /** report periodicaly */
    EMERGENCY,      /** report on emergency begin */
    EMERGENCY_END,  /** report on emergency end */
    OTHER = 90
}reason_code_t;

typedef enum rpt_state
{
    NORMAL_STATE,       /** normal report state */
    SEARCH_FAIL,        /** no Satellite found */
    OTHER_EXCEPTION,    /** other exceptions */
    UNREGISTER          /** unregistered from network */
}rpt_state_t;

typedef std::map<int, ack_rsp> AckResultMap;

/**
  \brief GIS report indicator, terminal will report its GIS location periodically or on some special events
  \note GIS location report and GIS subscription ACK are notified with the same indicator, please use isAck() to check the type
*/
class SDK_IMPORT_EXPORT GISReportIndicator
{
public:
    GISReportIndicator(std::string sender, std::string msgTp);
    GISReportIndicator(GISReportIndicator& copy);
    virtual ~GISReportIndicator();
    std::string datalog_str();

    void setSender(std::string sender);         /** report terminal */
    void setTime(std::string time);             /** report time */
    void setAltitude(std::string altitude);     /** Altitude */
    void setLongitude(std::string longitude);   /** Longitude */
    void setLatitude(std::string latitude);     /** Latitude */
    void setDirection(std::string direction);   /** direction, reserved */
    void setAckRsp(AckResultMap & ackMap);      /** subscribe acknowledgement, such as "1001:0;1002:1;1003:0" */

    void setReasonCode(reason_code_t rc);       /** report reason, please see the enum for details */
    void setRptState(rpt_state_t ust);          /** GIS report status, please see the enum for details */
    bool isAck();                               /** whether this is an ACK for GIS subscription */

    AckResultMap& getAckRsp();
    std::string getLongitude();
    std::string getLatitude();
    std::string getAltitude();
    std::string getDirection();
    std::string getTime();
    std::string getSender();
    reason_code_t getReasonCode();
    rpt_state_t getRptState();
    std::string getMsgType();

    void setSpeed(std::string speed);
    std::string getSpeed();                /** report speed*/

private:
    std::string _sender;
    std::string _latitude;
    std::string _longitude;
    std::string _altitude;
    std::string _direction;
    std::string _time;
    std::string _msgTp;
    AckResultMap _rspMap;
    reason_code_t  _reasoncd;
    rpt_state_t _ust;
    std::string _speed;
};


#endif // GISREPORTINDICATOR_H
