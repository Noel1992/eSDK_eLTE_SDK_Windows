#ifndef GW_CADISERVICE_H
#define GW_CADISERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
  namespace TRUNK {
    namespace SDK {
        class SDK_IMPORT_EXPORT CADIService
        {
        public:
            /**
            \brief Create a dynamic group
                \param dcid : the ID of the creator DC
                \param DGNAInfo: a JSON string depicting the message, such as
                 {
                    "grpid":"69100",   //If the isdn number is not carried, the group ID is allocated by the system.
                    "alias":"dgna1", //Indicates the dynamic group name.
                    "priority":"1", //Indicates the priority.
                    "maxperiod":"60", //Indicates the maximum call duration. The unit is s.
                    "uelist":[
                    { "isdn":"1001" },
                    { "isdn":"1002" }
                    ],
                    "grouplist":[
                    { "isdn":"61000" },
                    { "isdn":"61001" }
                    ]
                 }

                \note

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t DGNA_NEW(const char*  dcid, const char*  DGNAInfo);


            /**
            \brief Modify a dynamic group
                \param dcid : the ID of the creator DC
                \param DGNAInfo: a JSON string depicting the message, such as


                \note

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t DGNA_MODIFY(const char*  dcid, const char*  DGNAInfo);


            /**
            \brief Cancel a dynamic group
                \param dcid : the ID of the creator DC
                \param grpid: the ID for the dynamic group which is to be deleted
                \note Only the creator of this dynamic group can delete it

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t DGNA_DELETE(const char*  dcid, const char*  grpid);
        };

        class SDK_IMPORT_EXPORT CADIService_T
        {
        public:
            /**
              \brief Global access point to the CADIService.
              \return : the singleton object for CADIService
            */
          static CADIService *instance (void);
        };
    }
  }
}

#endif // GW_MESSAGESERVICE_H
