/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      encryption_assistant.hpp
Author:        XiaoLongguang
Created:       2012/10/29 09:17
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_MODULES_ENCRYPTION_ASSISTANT_HPP_
#define UBP_PLATFORM_MODULES_ENCRYPTION_ASSISTANT_HPP_

#include <string>
#include <vector>
#include <map>

#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "encryption_common.hpp"
#include "encryption_def.hpp"

namespace ubp
{
namespace platform
{
namespace encryption
{

class ENCRYPTION_IMPORT_EXPORT Encryption
  : public ubp::platform::frame::Module
{
  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    virtual ACE_INT32 Init(void)=0;
    virtual ACE_INT32 Fini(void)=0;
	
    //����һ��wkey,���Ϊ32�����ַ���,�ɼ�16����
    //virtual int GenAWkey(char * out_wkey)=0;

   //����wkey
    //in_wkey,16���ƿɼ��ַ���
    //out_wkey,16���ƿɼ��ַ���,ǰ32λ��iv,ivΪ16���ƿɼ��ַ���
    virtual ACE_INT32 HsmEncryptWkey(char *in_wkey, char *out_wkey)=0;
    //����wkey
    //in_wkey, 16���ƿɼ��ַ���,ǰ32λΪiv, ivΪ16���ƿɼ��ַ���
    //out_wkey, 16���ƿɼ��ַ���
    virtual ACE_INT32 HsmDecryptWkey(char *in_wkey, char *out_wkey)=0;
    //����
    //strIn,Ҫ���ܵ�����
    //in_key,��wkey,wkeyΪ16���ƿɼ��ַ���
    //out_str,���ܺ������,16���ƿɼ��ַ���,ǰ32λΪiv,ivΪ16���ƿɼ��ַ���
    virtual ACE_INT32 EncryptWithWkey(unsigned char * strIn, unsigned char *in_key, char out_str[], int in_length=0)=0;
    //����
    //strIn,Ҫ���ܵ�����,16���ƿɼ��ַ���,ǰ32λΪiv,ivΪ16���ƿɼ��ַ���
    //in_key,��wkey,wkeyΪ16���ƿɼ��ַ���
    //out_str,���ܺ������
    virtual ACE_INT32 DecryptWithWkey(unsigned char * strIn, unsigned char *in_key, char out_str[])=0;

    //��������ӽ���
    virtual ACE_INT32 LocalEncrypt(char *in_str, char *out_str)=0;
    //�µı��������ļ����ܷ�ʽ,iv��������
    //���õĺ�����ע�͵�,Ȼ����ɾ��
//    virtual ACE_INT32 LocalEncryptEx(char *in_str, char *out_str)=0;
//    virtual ACE_INT32 LocalEncryptDepWkey(char *in_str, char *out_str)=0;
    virtual ACE_INT32 LocalDecrypt(char *in_str, char *out_str)=0;
    virtual ACE_INT32 LocalEncryptOld(char *in_str, char *out_str)=0;
    virtual ACE_INT32 LocalDecryptOld(char *in_str, char *out_str)=0;
//    virtual ACE_INT32 LocalDecryptEx(char *in_str, char *out_str)=0;
//    virtual ACE_INT32 LocalDecryptDepWkey(char *in_str, char *out_str)=0;
    //asymmetric encryption SHA
	virtual std::string GetSaltofSHA256( const std::string & ciphertext )=0;
	virtual std::string SHA256WithSalt( const std::string & plaintext )=0;
	virtual std::string SHA256WithSalt( const std::string & plaintext, const std::string & salt_str )=0;

    virtual void test()=0;
};

}//namespace encryption
}//namespace platform
}//namespace ubp

#endif  //UBP_PLATFORM_MODULES_ENCRYPTION_ASSISTANT_HPP_
