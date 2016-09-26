#ifndef GW_MESSAGESERVICE_H
#define GW_MESSAGESERVICE_H

#include "sdk/include/sdk_export.h"

namespace LTE
{
  namespace TRUNK {
    namespace SDK {
        class SDK_IMPORT_EXPORT MessageService
        {
        public:
            /**
            \brief Send a short message or multi_media message from GW user to a LTE mobile or a LTE group
                \param from : a SDK_GW user
                \param to: a mobile terminal ID, or a LTE Group ID
                \param sdsInfo: a JSON string depicting the message, such as
                    {
                        "dest":"1001", //Receiver number
                        "content":"asdasdsadsadasdasdadsdsad"
                        "msgid":"1387782270184"//Short message ID specified by the sender. It is an optional parameter
                    }

                \note for Multi_Media message, please add a "attach" property, such as "attach": "10101.mp3";
                      Currently, only one attachment is supported. The path name is utf-8 character string.
                      If a user uploads the attachment, only the file name needs to be specified. Otherwise, the absolute path must be specified.

                \return : please refer to dc_module_error.h in C:\\Program Files\\SDK\\include\\dc
                \since: TTR3.0
            */
            Op_Return_t MESSAGE_SEND(const char*  from, const char*  to, const char*  sdsInfo);
        };

        class SDK_IMPORT_EXPORT MessageService_T
        {
        public:
            /**
              \brief Global access point to the MessageService.
              \return : the singleton object for MessageService
            */
          static MessageService *instance (void);
        };
    }
  }
}

#endif // GW_MESSAGESERVICE_H
