/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    no_copy.hpp
  Author:      Wu Gang
  Created:     2012/07/09 11:17
  Description:

*********************************************************************/

#ifndef UBP_PLATFORM_UTIL_NO_COPY_HPP_
#define UBP_PLATFORM_UTIL_NO_COPY_HPP_

namespace ubp
{
namespace platform
{
namespace util
{

class NonCopyable
{
  protected:
    NonCopyable(){};
    ~NonCopyable(){};
  private:  // emphasize the following members are private
    NonCopyable(const NonCopyable &);
    const NonCopyable &operator=(const NonCopyable &);
};

}//namespace util
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_UTIL_NO_COPY_HPP_