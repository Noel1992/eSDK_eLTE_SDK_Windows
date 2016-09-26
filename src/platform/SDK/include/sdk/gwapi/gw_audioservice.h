#ifndef GW_AUDIOSERVICE_H
#define GW_AUDIOSERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
  namespace TRUNK {
    namespace SDK {
        class SDK_IMPORT_EXPORT AudioService
        {
        public:
            /**
            \brief start a telephone-interconnect call
                \param from: the originator starting the call, which is normally a user outside LTE system and the telephone number start with "0"
                \param to: the receiver for this call, which is normally a LTE terminal or DC and is a short number
                \param originatorInfo: Not in use. Set this parameter to NULL.

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PA_DIAL(const char* from, const char* to, const char*  orginatorInfo);

            /**
            \brief Cancel an audio call
                \param call_id: ID of the call which is to be cancelled

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PA_CANCEL(CALLID_T call_id);

            /**
            \brief Accept an audio call
                \param call_id: ID of the call which is to be accepted

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PA_RECV(CALLID_T call_id, const char*  receiverInfo);

            /**
            \brief Reject an audio call
                \param call_id: ID of the call which is to be rejected
                \param cause: reject cause, normally a value of SIP code 4xx
                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PA_REJECT(CALLID_T call_id, int cause);

            /**
            \brief Hangup an audio call
                \param call_id: ID of the call which is to be ended

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PA_HANGUP(CALLID_T call_id);

            /**
            \brief Send a SIP 180 Ring message from SDK_GW to LTE terminal
                \param call_id: the call ID for this message
                \param status: a 18x status value, so far only support 180

                \note SDK_GW will NOT send 180ring by itself, instead 3rd Gateway shall invoke this API to tell SDK_GW sending 180ring

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t P2PA_RINGBACK(CALLID_T call_id, int status);
            /**
                \brief Start a Half-duplex telephone-interconnect call
                \param from: the originator starting the Half-duplex call
                \param to: the receiver for this call, which is normally a LTE terminal or DC and is a short number
                \param orginatorInfo: Not in use. Set this parameter to NULL.
                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t P2PA_HALFDPX_DIAL(const char* from, const char* to, const char* orginatorInfo);
            /**
                \brief Release a Half-duplex telephone-interconnect call
                \param from: the originator releases the Half-duplex call
                \param to: the receiver for this call, which is normally a LTE terminal or DC and is a short number
                \param orginatorInfo: Not in use. Set this parameter to NULL.
                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t P2PA_HALFDPX_RELEASE(const char* from, const char* to, const char* orginatorInfo);
        };

        class SDK_IMPORT_EXPORT AudioService_T
        {
        public:
          /// Global access point to the Singleton.
          static AudioService *instance (void);
        };
    }
  }
}
#endif // GW_AUDIOSERVICE_H
