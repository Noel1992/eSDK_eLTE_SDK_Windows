/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:    workkey.hpp
  Author:        ZhouYuanFu
  Created:       2014/12/9 17:11
  Description:

*********************************************************************/

#ifndef UBP_PLATFORM_WORKKEY_HPP_
#define UBP_PLATFORM_WORKKEY_HPP_

#include <string>
#include <boost/thread/recursive_mutex.hpp>

#include "ace/Basic_Types.h"
#include "workkey_def.hpp"

using namespace std;
const string MOC_WORKKEY = "WorkKey";
const string ATTR_WKEY = "WKey";

namespace ubp
{
namespace platform
{
namespace workkey
{

class UBP_WORKKEY_IMPORT_EXPORT CWorkKey
{
  public:
    CWorkKey();
    ~CWorkKey();

    //工作密钥相关接口，业务模块不能使用
    void RollbackWKey();
    void SwitchWKey();
    bool GetCurrentWKey(string& outWKey);
    bool GetNewWKey(string& outWKey);
    ACE_INT32 DecryptWithWkey(const string& strIn, string& strOut);
    ACE_INT32 EncryptWithWkey(const string& strIn, string& strOut);

    ACE_INT32 LocalEncrypt(const string& strIn, string& strOut, const std::string &in_encryption_ver="new");
    ACE_INT32 LocalDecrypt(const string& strIn, string& strOut, const std::string &in_encryption_ver="new");

	//asymmetric encryption SHA
	std::string GetSaltofSHA256( const std::string & ciphertext );
	std::string SHA256WithSalt( const std::string & plaintext );
	std::string SHA256WithSalt( const std::string & plaintext, const std::string & salt_str );
  private:
    ACE_INT32 DelWKey(const string& status);
    ACE_INT32 ModWKey();
    bool GetWKey(const string& status, string& outWKey);
  private:
    std::string workkey_;
    boost::recursive_mutex mutex_workkey_;
};

}//namespace query
}//namespace platform
}//namespace ubp

#endif // UBP_PLATFORM_WORKKEY_HPP_