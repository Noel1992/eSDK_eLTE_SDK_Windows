#ifndef GW_GROUPSERVICE_H
#define GW_GROUPSERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
    namespace TRUNK {
        namespace SDK {
            class SDK_IMPORT_EXPORT GroupService
            {
            public:
                /**
                \brief Start a PTT call or request speaking in an active PTT call
                \param from: ID of the proxy user in this PTT group
                \param grpid: ID of the PTT group
                \param originatorInfo: the real SDK_GW user which starts this request, in json format as below

                {"applyid":"xxxx"}

                \note Ordinarily SDK_GW users are considered as external resources and not provisioned in LTE system,
                instead one proxy user is provisioned in LTE system for each inter-connecting group

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
                */
                Op_Return_t PTTG_DIAL(const char* from, const char* grpid, const char*  originatorInfo);

                /**
                \brief Release the floor in an active PTT call
                \param from: ID of the proxy user in this PTT group
                \param grpid: ID of the PTT group

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
                */
                Op_Return_t PTTG_RELEASE(const char* from, const char* grpid);

                /**
                \brief Reject a PTT call
                \param calll_id: ID of the PTT group call

                \note Ordinarily SDK_GW don't need invoke this API to reject a PTT call request,
                only in some special case, such as resource is limited and SDK_GW has no available resources to handle this request

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
                */
                Op_Return_t PTTG_REJECT(CALLID_T call_id, int cause);

                /**
                \brief Leave a PTT call
                \param calll_id: ID of the PTT group call

                \note Ordinarilly SDK_GW don't need invoke this API to leave an active PTT call,
                only in some special case, such as resource is limited and SDK_GW need prempt this group call for some high-priority services

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
                */
                Op_Return_t PTTG_HANGUP(CALLID_T call_id);

                /**
                \brief Close a PTT call
                \param from: ID of the terminal which starts the PTT call
                \param grpid: ID of the PTT group

                \note the current PTT call on the "grpid" shall be started by the "from" user,
                if this API is not invoked, the PTT call will be closed by systerm after PTT-Idle timer expires

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
                */
                Op_Return_t PTTG_CLOSE(const char* from, const char* grpid);
            };

            class SDK_IMPORT_EXPORT GroupService_T
            {
            public:
                /// Global access point to the Singleton.
                static GroupService *instance (void);
            };
        }
    }
}


#endif // GW_GROUPSERVICE_H
