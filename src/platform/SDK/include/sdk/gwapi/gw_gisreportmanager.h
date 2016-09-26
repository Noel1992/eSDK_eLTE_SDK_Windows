#ifndef GW_GISREPORTMANAGER_H
#define GW_GISREPORTMANAGER_H

#include "sdk/proxy/ntfy/notifyhandler_itf.h"
#include "sdk/include/gisreportmanager.h"
#include "sdk/gwapi/service_observer.h"
#include "sdk/include/sdk_export.h"
using namespace LTE::TRUNK::SDK;


class SDK_IMPORT_EXPORT Gw_GISReportManager : public GISReportManager
{
public:
    Gw_GISReportManager();
    /**
      \brief After GIS_SUB is called, the latest location information of the terminal is reported first, then location information of the terminal or exception events are regularly reported
    */
    virtual void notifyGISReport(GISReportIndicator * gisInd);
    /**
      \brief After the GIS_SUB operation is called, the eMDC610 returns a subscription status list. 0 indicates that the subscription is successful, and 1 indicates that the subscription fails.
    */
    virtual void notifyGISSubRsp(GISReportIndicator * gisInd);

    virtual void setObserver(GisServiceObserver* it){GisObserver = it;}

private:

	GisServiceObserver* GisObserver;
		
};

#endif // GW_GISREPORTMANAGER_H
