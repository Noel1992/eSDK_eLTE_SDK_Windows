/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    message_factory.hpp
  Author:      Wu Gang
  Created:     2012/07/24 14:41
  Description:

*********************************************************************/
#ifndef UBP_PLATFORM_MOUDLES_MQ_MESSAGE_FACTORY_HPP_
#define UBP_PLATFORM_MOUDLES_MQ_MESSAGE_FACTORY_HPP_

#include "mq_export.hpp"

#include "google/protobuf/descriptor.h"

//////////////////////////////////////////////////////////////////////////////
//IMessageFactory
//////////////////////////////////////////////////////////////////////////////
namespace ubp
{
namespace platform
{
namespace mq
{
class MQ_IMPORT_EXPORT IMessageFactory
{
  public:
    static google::protobuf::Message *CreateMessage(const std::string &msgId);

    static ::google::protobuf::Message *CreateMessageHeader();

    static ::google::protobuf::Message *CloneMessage(
      const ::google::protobuf::Message &msg);
};
}//namespace mq
}//namespace platform
}//namespace ubp

#endif //UBP_PLATFORM_MOUDLES_MQ_MESSAGE_FACTORY_HPP_