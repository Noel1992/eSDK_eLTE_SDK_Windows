#ifndef UBP_PLATFORM_AES_ENCRIPT_HPP_
#define UBP_PLATFORM_AES_ENCRIPT_HPP_

/********************************************************************
  Copyright 2012,TD-Tech. Co., Ltd.
  Filename:   
  Author:     
  Created:    
  Description:

*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <boost/exception/all.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  
#include <boost/lexical_cast.hpp>  
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <boost/thread/recursive_mutex.hpp>
//#include <boost/xpressive/xpressive.hpp>  

#include "platform/modules/trace/ubp_trace.hpp"

namespace ubp{
namespace platform{
namespace util{

int base64_encode(unsigned char *str,int str_len,char **encode,int &encode_len);
int base64_decode(char *str,int str_len,char **decode,int &decode_buffer_len);

class AesBase64
{
public:
    AesBase64();

    ~AesBase64();

    int encode(const std::string &_in, std::string &_out);
    int decode(const std::string &_in, std::string &_out);
    int Init();
    int Fini();

private:
    // "opaque" encryption, decryption ctx structures that libcrypto uses to record
    //   status of enc/dec operations 
    EVP_CIPHER_CTX en_;
    EVP_CIPHER_CTX de_;
    boost::recursive_mutex mutex_enc_;
};


}}}

#endif //UBP_PLATFORM_AES_ENCRIPT_HPP_

