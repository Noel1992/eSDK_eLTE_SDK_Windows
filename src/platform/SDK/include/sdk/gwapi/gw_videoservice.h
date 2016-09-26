#ifndef GW_VIDEOSERVICE_H
#define GW_VIDEOSERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
  namespace TRUNK {
    namespace SDK {
        class SDK_IMPORT_EXPORT VideoService
        {
        public:
            /**
            \brief start a peer-to-peer video call
                \param from: the originator which starts the peer-to-peer video call
                \param to: the receiver for this peer-to-peer video call
                \param originatorInfo:
                        {
                        "fmt": "D1", //video format, can be D1, CIF, QCIF, etc
                        "mute": "0",   //Always Set this parameter to 0
                        "confirm": "1",  //Always Set this parameter to 1
                        "camera": "1"  	// Always Set this parameter to 1
                        }


                \note   "fmt" value depends on LTE terminal model, and will be negotiated between source and destination mobiles;
                        After invoke this API, the SDK_GW reports the ID of an ongoing call and call status over the reverse event channel.

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PV_DIAL(const char* from, const char* to, const char*  orginatorInfo);

            /**
            \brief start a video surveillance service, which is normally an one-way video transmission
                \param from: the originator which starts the video surveillance
                \param to: the video source which shall transmit video to the originator
                \param originatorInfo:
                        {

                        "fmt": "D1",  //Optional video format for LTE terminals. It does not apply to fixed cameras
                        "mute": "0",  //Whether video is mute. 0: not mute, 1: mute
                        "confirm": "1", //Reception mode, 0: automatic reception, 1: manual reception. It does not apply to fixed cameras
                        "camera": "1"	//0: front camera, 1: rear camera. It does not apply to fixed cameras
                        }


                \note   "fmt" value depends on LTE terminal model, and will be negotiated between source and destination mobiles;
                        After invoke this API, the SDK_GW reports the ID of an ongoing call and call status over the reverse event channel.

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PV_MONITOR(const char* from, const char* to, const char*  orginatorInfo);

            /**
            \brief Cancel a video call or video surveillance request
                \param call_id: ID of the call which is to be cancelled

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PV_CANCEL(CALLID_T call_id);

            /**
            \brief Pan-Tile-Zoom control of a LTE wireless camera
                \param call_id: ID of the call which is to be controlled
                \param ptzControl: Please refer developers guide for more details about PTZ control command
                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \note: NOT all LTE wireless cameras support PTZ control, please check with the vendor first
                \since: TTR3.0
            */
            Op_Return_t P2PV_PTZ(CALLID_T call_id,const char* ptzControl);

            /**
            \brief Accept a video call or video surveillance request
                \param call_id: ID of the call which is to be accepted
                \param receiverInfo: Parameter about the video service which will be the negotiated result, in json format below:
                        {
                        "fmt": "1",  //0-CIF, 1-QCIF, 2-D1, 3-720P, 4-1080P
                        "mute": "0",  //Whether video is mute. 0: not mute, 1: mute
                        }

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PV_RECV(CALLID_T call_id, const char*  receiverInfo);

            /**
            \brief Reject a video call or video surveillance request
                \param call_id: ID of the call which is to be rejected
                \param cause: reject cause, normally a value of SIP code 4xx
                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PV_REJECT(CALLID_T call_id, int cause);

            /**
            \brief Hangup a video call or video surveillance service
                \param call_id: ID of the call which is to be ended

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc

                \since: TTR3.0
            */
            Op_Return_t P2PV_HANGUP(CALLID_T call_id);

            /**
            \brief start a video dispatch
                \param dcid: the DC which start this operation
                \param videosrc: the ID for the video source
                \param destinationList: the list of video receivers

                \note Before video dispatch, the DC shall be monitoring the video source

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t P2PV_DISPATCH(const char* dcid, const char* videosrc, const char*  destinationList);

            /**
            \brief Delete some receivers from a video dispatching service
                \param dcid: the DC which start this operation
                \param videosrc: the ID for the video source
                \param destination: the list of video receivers which shall be deleted

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */

            Op_Return_t P2PV_DISPATCH_DELETE(const char* dcid, const char* videosrc, const char*  destination);

            /**
            \brief Send a SIP 180 Ring message from SDK_GW to LTE terminal
                \param call_id: the call ID for this message
                \param status: a 18x status value, so far only support 180

                \note SDK_GW will NOT send 180ring by itself, instead 3rd Gateway shall invoke this API to tell SDK_GW sending 180ring

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR4.0
            */
            Op_Return_t P2PV_RINGBACK(CALLID_T call_id, int status);
        };

        class SDK_IMPORT_EXPORT VideoService_T
        {
        public:
          /// Global access point to the Singleton.
          static VideoService *instance (void);
        };
    }
  }
}

#endif // GW_VIDEOSERVICE_H
