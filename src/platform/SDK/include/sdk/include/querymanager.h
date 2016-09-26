#ifndef QUERYMANAGER_H
#define QUERYMANAGER_H


#include <stdint.h>
#include <list>
#include <string>

#include "sdk/gwapi/service_observer.h"
#include "sdk/include/sdk_export.h"


class SDK_IMPORT_EXPORT  QueryManager
{    

public:
    /**
      \brief init QueryManager
      \param dcid ID for DC
      \param serverIP IP address for MDC server
      \param obs QueryServiceObserver, can be set either in this function or in set_observer();
      \param mrsIP IP address for MRS server, shall be provided if MRS is deployed separately from MDC
     */
    void init(MSISDN dcid,const char* serverIP,LTE::TRUNK::SDK::QueryServiceObserver* obs=NULL, const char* mrsIP=NULL);

    /**
      \brief set observer object for QueryManager
      \note query result will be notified through QueryServiceObserver::notify()
     */
    void set_observer(LTE::TRUNK::SDK::QueryServiceObserver* obs);
    /**
        \brief Query Provisions from configuration database
        \param command: query command name,shall be defined in constdef.h
        \param jsonQuery: query conditions in the style of json string

        \note  the query will be executed in async mode, result will be notified via QueryServiceObserver::notify
     */
    int query_async(const char* command,const char* jsonQuery);

public:
    QueryManager();

};

class SDK_IMPORT_EXPORT QueryManager_T
{
public:
  /// Global access point to the Singleton.
  static QueryManager *instance (void);
};

#endif // QUERYMANAGER_H
