
#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include "sdk_export.h"

class GISReportIndicator;

class SDK_IMPORT_EXPORT GISReportManager
{
public:

    //virtual ~GISReportManager(){};

    /**
      \brief After GIS_SUB is called, the latest location information of the terminal is reported first, then location information of the terminal or exception events are regularly reported
    */
    virtual void notifyGISReport(GISReportIndicator *) = 0;

    /**
      \brief After the GIS_SUB operation is called, the eMDC610 returns a subscription status list. 0 indicates that the subscription is successful, and 1 indicates that the subscription fails.
    */
    virtual void notifyGISSubRsp(GISReportIndicator * ) = 0;

private:

};


#endif // REPORTMANAGER_H
