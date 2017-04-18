#ifndef GW_GISREPORTSERVICE_H
#define GW_GISREPORTSERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
    namespace TRUNK {
        namespace SDK {
            class SDK_IMPORT_EXPORT GISReportService
            {
            public:
                /**
                \brief Subscribe/Unsubscribe GPS reporting of a mobile terminal
                \param from_user : The Disptach Console ID who subscribes
                \param to: The mobile terminal ID
                \param gisInfo: a JSON string depicting the subscribed/unsubscribed mobile terminals, such as
                {
                "subtype":"sub", "uelist": [
                { "isdn":"1001" },
                { "isdn":"1002"}
                ]
                }
                \note "subtype" can be "sub" or "unsub"
                \return : please refer to dc_module_error.h in C:\Program Files\SDK\include\dc
                \since: TTR3.0
                */
                Op_Return_t GIS_SUB(const char*  from_user, const char*  to, const char*  gisInfo);

                /**
                \brief Customize GIS report configuration for a specific mobile terminal
                \param dcid : The Disptach Console ID which trigger the change
                \param mobileid: The mobile terminal ID
                \param gisCfg: a JSON string depicting the new GIS configuration, such as
                {
                "userid":"8001",
                "reportperiod":"10",
                "reportdistance":"1"
                }
                \note: "reportperiod" is in unit of second, shall be one vaule in {0,1,2,5,10,15,30,60}
                \note: "reportdistance" is in unit of meter, shall be a value between 0 and 10000
                \return : please refer to dc_module_error.h in C:\Program Files\SDK\include\dc
                \since: TTR4.0
                */
                Op_Return_t GIS_CFG(const char*  dcid, const char*  mobileid, const char*  gisCfg);
            };

            class SDK_IMPORT_EXPORT GISReportService_T
            {
            public:
                /**
                \brief Global access point to the GISReportService.
                \return : the singleton object for GISReportService
                */
                static GISReportService *instance (void);
            };
        }
    }
}

#endif // GW_GISREPORTSERVICE_H
