/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      encryption_error.hpp
Author:        XiaoLongguang
Created:       2013/06/18 10:07
Description:
*********************************************************************/
#ifndef  UBP_PLATFORM_MODULES_ENCRYPTION_ERROR_HPP_
#define  UBP_PLATFORM_MODULES_ENCRYPTION_ERROR_HPP_

#define UBP_ENCRYPTION_SUCCESS 0
#define UBP_ENCRYPTION_ERROR 1

#define HSS_WKEY_DB_LEN       16

#define VOS_OK 0

#define HSM_SUCCESS 0

#define VOS_ERR -1

#define AES_CFB_ENCRYPT 1
#define AES_CFB_DECRYPT 0

#define AES_CFB_IV_LEN 16
#define AES_IV_STR_LEN 32
#define AES_KEY_LEN_128 128

#define HSM_SIPSTR_DB_LEN 36
#define HSM_SIPSTR_WITH_IVEC_DB_LEN 68
#define HSM_SIPSTR_UI_LEN 18

//#define O_KEY_LEN 16
//#define U_KEY_LEN 16
#define SALT_VALUE_LEN 16
#define PKCS5_ITER_COUNT 5000

//ECB加解密的最大长度
#define ECB_STR_MAX_LEN 32

/*鉴权参数相关定义*/
#define HSS_ESCN_OP_STRING_LEN 32
#define HSS_ESCN_AMF_STRING_LEN 4

/*Add By tangxuesong 鉴权密钥定义*/
#define HSS_ESCN_KAUTH_STRING_LEN    32

/*错误APNNI格式的数目*/
#define HSS_IVALID_APNNI_HEAD 4
#define HSS_IVALID_APNNI_TAIL 2

#define HSS_HUSIM_IMSI_LEN     15
#define HSS_HUSIM_KI_LEN       32
#define HSS_NO_RECORD            0

#define HSS_ERR_DATA_LENGTH 0XFFFF

//add by liuzhenxing for DGNA
#define HSS_PERIOD_CHECK_TIMER     100//暂定为100
#define HSS_GET_SIPLINK_STATE_TIMER 101
#define HSS_PERIOD_CHECK_TIME_LENGTH 300*1000//300秒 即5分钟
#define HSS_GET_SIPLINK_OK 1
#define HSS_GET_SIPLINK_ERR 2
#define HSS_MAX_GET_SIPLINK_NUM 3

//TTR2.0 新增宏定义
#define HSS_STATIC_GROUP_TYPE 0
#define HSS_DYN_GROUP_TYPE    1

#define HSM_MAX_SYNC_NO 0x7FFFFFFF

#define HSS_WKEY_FILE_NAME "Tffs:/HSSWK.DAT"  
#define HSS_WEKY_TEMP_FILE_NAME "Tffs:/HSSWKTEMP.DAT"  
#define HSS_WEKY_BKP_FILE_NAME "Tffs2:/HSSBKPWK.DAT"  
#define HSS_WKEY_LEN       32
#define HSS_WKEY_DB_LEN       16

#define ENC_MAX_STR_LEN 512
#define ENC_OK 0
#define ENC_ERR -1
#define ENC_STR_UI_LEN 512
#define ENC_SUCCESS 0
#define AES_KEY_LEN_128 128
#define AES_CFB_DECRYPT 0
#define AES_CFB_ENCRYPT 1
#define ENC_NULL_PTR NULL

#define SHA256_SALT_LENGTH 32

typedef unsigned char VOS_UINT8;
typedef ACE_UINT32 VOS_UINT32;
typedef char VOS_CHAR;
typedef ACE_INT32 VOS_INT32;


#endif //UBP_PLATFORM_MODULES_ENCRYPTION_ERROR_HPP_
