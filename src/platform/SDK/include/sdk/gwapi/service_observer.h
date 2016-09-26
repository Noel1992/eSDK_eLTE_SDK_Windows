#ifndef SERVICE_OBSERVER_H
#define SERVICE_OBSERVER_H

#include "sdk/include/sdk_export.h"
namespace LTE {
  namespace TRUNK {
    namespace SDK {
    /**
      \brief Notify Audio Call Event
      \param callid Call ID for this event
      \param eventid Event ID for this event
      \param jsonSignal Details for this event
    */
    class SDK_IMPORT_EXPORT AudioServiceObserver
    {
        public:
        virtual void notify(CALLID_T callid, int eventid,const char* jsonSignal)=0;
    };
    /**
      \brief Notify Video Call Event
      \param callid Call ID for this event
      \param eventid Event ID for this event
      \param jsonSignal Details for this event
    */
    class SDK_IMPORT_EXPORT VideoServiceObserver
    {
        public:
        virtual void notify(CALLID_T callid, int eventid,const char* jsonSignal)=0;
    };
    /**
      \brief Notify Short Message and Multi-Media Message Event
      \param jsonSignal Message details in JSON format, please refer developers guide for its definition

      \note both Received Message and Sending-Acknowlegement are notified through this API
            If the "opt" field in jsonMessage is set to "sendresult", the message sending result is notified
            If the "opt" field in jsonMessage is set to "recvmsg", the new message received is notified
    */
    class SDK_IMPORT_EXPORT MessageServiceObserver
    {
        public:
        virtual void notify(const char* jsonSignal)=0;
    };
    /**
      \brief Notify GIS Event
      \param jsonSignal GIS report details in JSON format, please refer developers guide for its definition

      \note both GIS location report and GIS subscription acknowlegement are notified through this API
            If the "opt" field in jsonMessage is set to "report", GPS locations are reported.
            If the "opt" field in jsonMessage is set to "sub" or "unsub", subscription results are notified
    */
    class SDK_IMPORT_EXPORT GisServiceObserver
    {
        public:
        virtual void notify(const char* jsonSignal)=0;
    };

    /**
      \brief Notify Group Call Event
      \param callid Call ID for this event
      \param eventid Event ID for this event
      \param jsonSignal Details for this event
    */
    class SDK_IMPORT_EXPORT GroupServiceObserver
    {
        public:
        virtual void notify(CALLID_T callid, int eventid,const char* jsonSignal)=0;
    };

    /**
      \brief Notify Provision Query Result
      \param cmd Command ID for this query
      \param jsonResult Result for this query
      \note Query request shall be invoked through QueryManager::query_async(const char* command,const char* jsonQuery)
            And result is notified through this callback.
            Please refer developers guide for more details
    */
    class SDK_IMPORT_EXPORT QueryServiceObserver
    {
        public:
        virtual void notify(const char* cmd,const char* jsonResult)=0;
    };

    /**
      \brief Notify module status
      \param modulename Name of the module which raise this notification
      \param jsonResult detail of this notification
      \note please refer developers guide for more details about module notifications
    */
    class SDK_IMPORT_EXPORT ModuleStatusObserver
    {
        public:
        virtual void notify(const char* modulename,const char* jsonResult)=0;
    };
    /**
      \brief Notify resource status
      \param resid ID for the resource which raise this event
      \param jsonResult Details of this event
      \note please refer developers guide for more details about resource status notifications
    */
    class SDK_IMPORT_EXPORT ResourceStatusObserver
    {
        public:
        virtual void notify(MSISDN resid,const char* jsonResult)=0;
    };

    }
  }
}
#endif // SERVICE_OBSERVER_H
