/********************************************************************
Copyright 2013,TD-Tech. Co., Ltd.
Filename:    bcc_error.hpp
Author:      Yang Yuanyuan
Created:     2013/05/16 10:07
Description:
*********************************************************************/
#ifndef  UBP_BCC_BCC_ERROR_HPP_
#define  UBP_BCC_BCC_ERROR_HPP_

#include "platform/modules/error/ubp_error.hpp"

///////////////////////////////////////////////////////////////////////
//bcc：-20000 ~ -29999
//|--ZMQ     :-20000 ~ -XXXX

//For example : 
//static const ACE_INT32 ERR_UBP_MQ_XXX1 = -20000;
///////////////////////////////////////////////////////////////////////

//ZMQ:-29999 ~ -XXXX
#define ERR_BCC_ZMQ_NO_LICENSE                -20000                        //later insert/dispatch/discreet listen and so:License is not enough
#define ERR_BCC_ZMQ_EMERGENCY_REJECT         (ERR_BCC_ZMQ_NO_LICENSE-1)     //Emergency calls reject dc break off or break in

//Begain for dispatch zmq error_code
#define ERR_BCC_ZMQ_TARGET_NOT_FOUND         (ERR_BCC_ZMQ_NO_LICENSE-12)    //Videodispatch user account not found 
#define ERR_BCC_ZMQ_TARGET_NOT_RIGISTERED    (ERR_BCC_ZMQ_NO_LICENSE-13)    //Videodispatch user not registered
#define ERR_BCC_SINGLE_DIALOG_EXISTED        (ERR_BCC_ZMQ_NO_LICENSE-14)    //single dialog has existed   
#define ERR_BCC_ZMQ_VPN_NOT_ALLOWED          (ERR_BCC_ZMQ_NO_LICENSE-15)    //vpn source and dispath user's vpn not allowed
#define ERR_BCC_ZMQ_DISPATCH_FULL            (ERR_BCC_ZMQ_NO_LICENSE-16)    //Videodispatch is reached max number

//Begain for rec zmq error_code
#define ERR_BCC_ZMQ_RECSTART_TYPE_NO_MATCH   (ERR_BCC_ZMQ_NO_LICENSE-24)    //Rec type not match session type
#define ERR_BCC_ZMQ_RECSTART_SESSION_ABSENT  (ERR_BCC_ZMQ_NO_LICENSE-25)    //Rec session no existed
#define ERR_BCC_ZMQ_RECSTART_TASK_EXISTED    (ERR_BCC_ZMQ_NO_LICENSE-26)    //Rec task has existed
#define ERR_BCC_RECSTART_INNER               (ERR_BCC_ZMQ_NO_LICENSE-27)    //Rec object no existed or others

//Begain for gmg zmq error_code
#define ERR_BCC_ZMQ_DMGDMGMEMBER_BEYOND_MAX_NUM    (ERR_BCC_ZMQ_NO_LICENSE-38)    //Number of dynamicgroup users beyond max
#define ERR_BCC_ZMQ_WRONG_DMG_NUM                  (ERR_BCC_ZMQ_NO_LICENSE-39)    //Wrong dynamic group num
#define ERR_BCC_ZMQ_DMG_UNSPECIFIED                (ERR_BCC_ZMQ_NO_LICENSE-40)    //unspecified reasons
#define ERR_BCC_ZMQ_DMG_BEYOND_MAX                 (ERR_BCC_ZMQ_NO_LICENSE-41)    //Number of dynamic group beyond max
#define ERR_BCC_ZMQ_DMG_TIMEROUT                   (ERR_BCC_ZMQ_NO_LICENSE-42)    //ECN'S rsp, useless in 4.0
#define ERR_BCC_ZMQ_DMGDMGMEMBER_BEYOND_MAX_GROUP  (ERR_BCC_ZMQ_NO_LICENSE-43)    //DynamicGroup member joind beyond limited max groups
#define ERR_BCC_ZMQ_DMGMEMBER_NOT_RIGISTERED       (ERR_BCC_ZMQ_NO_LICENSE-44)    //DynamicGroup member not registered
#define ERR_BCC_ZMQ_DMG_NO_MATCH_VERSION           (ERR_BCC_ZMQ_NO_LICENSE-45)    //current version doe not support dmg
#define ERR_BCC_ZMQ_DMG_ALREADY_EXISTED            (ERR_BCC_ZMQ_NO_LICENSE-46)    //the dmg has alredy been existed
#define ERR_BCC_ZMQ_DMG_NO_OTHER_MEMBER            (ERR_BCC_ZMQ_NO_LICENSE-47)    //the dmg has no other member except dc itself
#define ERR_BCC_ZMQ_DMG_ECN_HANDLE_FAIL            (ERR_BCC_ZMQ_NO_LICENSE-48)    //ECN's rsp, useless in 4.0
#define ERR_BCC_ZMQ_DMG_ALL_PTT_USER_FAILED        (ERR_BCC_ZMQ_NO_LICENSE-49)    //ECN's rsp, useless in 4.0
#define ERR_BCC_ZMQ_DMG_VERSION_NOT_SUPPORTED      (ERR_BCC_ZMQ_NO_LICENSE-50)    //ECN's rsp, useless in 4.0
#define ERR_BCC_ZMQ_DMG_DOWNLOAD_NOT_ALLOWED       (ERR_BCC_ZMQ_NO_LICENSE-51)    //ECN's rsp, useless in 4.0
#define ERR_BCC_ZMQ_DMG_WRONG_NUMBER               (ERR_BCC_ZMQ_NO_LICENSE-52)    //ECN's rsp, useless in 4.0

/*BEGIN:jinjupeng to review, SOR_TTR40_0031_SDR_0012_001_002, 2015-06-11 */
//#define ERR_BCC_ZMQ_DMG_UDCTIMEROUT                (ERR_BCC_ZMQ_NO_LICENSE-53)    //UDC's rsp time out! 
#define ERR_BCC_ZMQ_DMG_UDCERROR                   (ERR_BCC_ZMQ_NO_LICENSE-54)    //Other UDC's error
#define ERR_BCC_ZMQ_DMG_ISDNERROR                  (ERR_BCC_ZMQ_NO_LICENSE-55)    //User's isdn is wrong
#define ERR_BCC_ZMQ_DMG_UDCSYNCERROR               (ERR_BCC_ZMQ_NO_LICENSE-56)    //UDC sync data to MDC error
/*END:  jinjupeng to review, SOR_TTR40_0031_SDR_0012_001_002, 2015-06-11 */

//Begain for break off error_code
#define ERR_BCC_ZMQ_BREAKOFF_NOT_ADSCRIPTION        (ERR_BCC_ZMQ_NO_LICENSE-60)    //group is not belongs to self, can't breakoff
#define ERR_BCC_ZMQ_BREAK_IN_REJECT             (ERR_BCC_ZMQ_NO_LICENSE-62)    // AmbienceLitten calls reject dc  break in


#define ERR_BCC_ZMQ_ENCRYPT_NOT_ALLOWED             (ERR_BCC_ZMQ_NO_LICENSE-61)    //network handle encrypt

//patch group,result is always ERR_BCC_ZMQ_PATCH_GROUP_UNSPECIFIED,but specified group id has specified result
#define ERR_BCC_ZMQ_PATCH_GROUP_UNSPECIFIED                 (ERR_BCC_ZMQ_NO_LICENSE-62)    //patch create fail
#define ERR_BCC_ZMQ_PATCH_GROUP_MAX_NUMBER_REACHED          (ERR_BCC_ZMQ_NO_LICENSE-63)
#define ERR_BCC_ZMQ_PATCH_GROUP_INVALID_DC                  (ERR_BCC_ZMQ_NO_LICENSE-64)
#define ERR_BCC_ZMQ_PATCH_GROUP_INVALID_PGID                (ERR_BCC_ZMQ_NO_LICENSE-65)
#define ERR_BCC_ZMQ_PATCH_GROUP_MEMBER_MAX_NUMBER_REACHED   (ERR_BCC_ZMQ_NO_LICENSE-66)
#define ERR_BCC_ZMQ_PATCH_GROUP_MEMBER_INPUT_ZERO           (ERR_BCC_ZMQ_NO_LICENSE-67)
#define ERR_BCC_ZMQ_PATCH_GROUP_INVALID_PGINDEX             (ERR_BCC_ZMQ_NO_LICENSE-68)

//transfer failed, for instance transfer target is not regiter, refuse, in speaking...
#define ERR_BCC_ZMQ_TRANSFER_FAILED                     (ERR_BCC_ZMQ_NO_LICENSE-69)
#define ERR_BCC_ZMQ_TRANSCODE_NO_LICENSE                (ERR_BCC_ZMQ_NO_LICENSE-70)

//new err code for 4.0 
#define ERR_BCC_ZMQ_PATCH_GROUP_DN_GET_FAIL      (ERR_BCC_ZMQ_NO_LICENSE-72)

#define ERR_BCC_ZMQ_TEMP_GROUP_DN_GET_FAIL       (ERR_BCC_ZMQ_NO_LICENSE-102)

//UDC error global!
#define ERR_BCC_ZMQ_DMG_UDCTIMEROUT                 (ERR_BCC_ZMQ_NO_LICENSE-200)
#define ERR_BCC_ZMQ_DMG_UDCLOGINFAIL                (ERR_BCC_ZMQ_NO_LICENSE-201)
#define ERR_BCC_ZMQ_DMG_UDCINNERERR                 (ERR_BCC_ZMQ_NO_LICENSE-202)


#define ERR_BCC_ZMQ_PWD_UDCISDNFAIL                 (ERR_BCC_ZMQ_NO_LICENSE-401)
#define ERR_BCC_ZMQ_PWD_UDCOLDPWDERROR              (ERR_BCC_ZMQ_NO_LICENSE-402)
#define ERR_BCC_ZMQ_PWD_UDCNEWPWDILLEGAL            (ERR_BCC_ZMQ_NO_LICENSE-403)
#define ERR_BCC_ZMQ_PWD_UDCUNKOWN                   (ERR_BCC_ZMQ_NO_LICENSE-404)

//Add by chenchujie for DTS2016072809307
#define ERR_BCC_ZMQ_TMP_USER_JOIN_INVALID_ISDN      (ERR_BCC_ZMQ_NO_LICENSE-500)
//End of add by chenchujie for DTS2016072809307 

//Add by chenchujie for DTS2016110108657
#define ERR_BCC_ZMQ_ADS_ZONE_NOT_FOUND              (ERR_BCC_ZMQ_NO_LICENSE-600)
#define ERR_BCC_ZMQ_ADS_ZONE_NOT_CONNECTED          (ERR_BCC_ZMQ_NO_LICENSE-601)
//End of add by chenchujie for DTS2016110108657 

// member group id's specified result
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_OK                       0
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_ALREADY_PATCHED          1
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_NON_STATIC_GROUP         2
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_NOT_LOCAL_BCC            3
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_VPN_NOT_ALLOWED          4
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_DEL_NOT_FOUND            5
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_IS_EMERGENCY             6
#define ERR_BCC_ZMQ_PATCH_RESULT_GROUP_NOT_ALLOWED             7
//hpgw return error code authentication error
#define ERROR_BCC_HPGW_ZMQ_AUTH_FAILURE        10208

//TO DOs
#endif //UBP_BCC_BCC_ERROR_HPP_
