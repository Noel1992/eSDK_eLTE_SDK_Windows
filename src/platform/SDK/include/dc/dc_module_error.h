/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    dc_module_error.hpp
Author:      zhangjiansheng
Created:     2013/05/20 10:08
Description:
*********************************************************************/
#ifndef  UBP_DC_MODULES_ERROR_H
#define  UBP_DC_MODULES_ERROR_H

#include "platform/modules/error/ubp_error.hpp"

///////////////////////////////////////////////////////////////////////
//DC：-40000 ~ -49999
//  |--SDK    : -40000 ~ -40999
//  |--APP    : -41000 ~ -41999
//  |--GUI    : -42000 ~ -42999
//For example :
//static const ACE_INT32 ERR_UBP_DC_APP_XXX1 = -41000;
///////////////////////////////////////////////////////////////////////

//DC_SDK:-40000 ~ -40999
/**
    \40000
    \40000
*/
static const ACE_INT32 ERR_UBP_DC_SDK_BASE = -40000 ;
/**
    \The operation was not accepted,the operation state is not correct,the lack of necessary prerequisite. 40001
    \操作不被接受，操作状态不正确，缺少必要的前提 40001
*/
static const ACE_INT32 ERR_UBP_DC_SDK_STATE_MISMATCH = ERR_UBP_DC_SDK_BASE-1; 
/**
    \The operation was not accepted ,because there is no resource. 40002
    \操作不被接受，因为没有该资源 40002
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NO_SUCHRESOURCE = ERR_UBP_DC_SDK_BASE-2;
/**
    \The operation was not accepted,because the corresponding resource does not  support this operation . 40003
    \操作不被接受，因为对应资源不支持该操作能力 40003
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NO_CAPABILITY = ERR_UBP_DC_SDK_BASE-3; 	    
/**
    \The operation was not accepted,because it is beyond the ability to handle APP . 40004
    \操作不被接受，因为超出APP处理能力 40004
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NO_CAPACITY = ERR_UBP_DC_SDK_BASE-4; 
/**
    \The operation was not accepted,because the user has no authority to do the operation on the corresponding resource. 40005
    \操作不被接受，因为用户在对应资源上没有权限做该操作 40005
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NO_PERMISSION = ERR_UBP_DC_SDK_BASE-5;
/**
    \The operation was not accepted,because the business is limited. 40006
    \操作不被接受, 业务并发受限 40006
*/
static const ACE_INT32 ERR_UBP_DC_SDK_PARALLEL_CONFLICT = ERR_UBP_DC_SDK_BASE-6;
/**
    \The operation was not accepted,because IP address is not legal. 40007
    \操作不被接受, IP地址不合法 40007
*/
static const ACE_INT32 ERR_UBP_DC_SDK_IP_REPEAT  = ERR_UBP_DC_SDK_BASE-7;
/**
    \The operation was not accepted,because IP address is not legal. 40008
    \操作不被接受, IP地址不合法 40008
*/
static const ACE_INT32 ERR_UBP_DC_SDK_IP_INVALID  = ERR_UBP_DC_SDK_BASE-8;       
/**
    \The operation was not accepted,perform operation result output. 40009
    \操作不被接受,执行结果输出  40009
*/
static const ACE_INT32 ERR_UBP_DC_SDK_OP_OK_QUEUED  = ERR_UBP_DC_SDK_BASE-9;
/**
    \release PTT error. 40010
    \执行组呼释放错误  40010
*/
static const ACE_INT32 ERR_UBP_DC_SDK_PTT_RELEASE_ERROR  = ERR_UBP_DC_SDK_BASE-10; 
/**
    \The operation was not accepted,because groups were not subscribed. 40011
    \操作不被接受,组没有被订阅  40011
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NOT_SUBSCRIBED  = ERR_UBP_DC_SDK_BASE-11;
/**
    \Media surface failed,please check the MIC insert. 40012
    \媒体面失败，检查MIC是否插入  40012
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MEDIAPLANE_ERROR = ERR_UBP_DC_SDK_BASE-12;
/**
    \Port allocation failure,no port allocation. 40013
    \端口分配失败，无可用端口供分配  40013
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NO_FREE_PORT_ERROR = ERR_UBP_DC_SDK_BASE-13;

/**
 * Cannot find user home.40094
 * 找不到归属地（用户在ZONE号段外）
 */
static const ACE_INT32 ERR_UBP_DC_SDK_ADS_ZONE_NOT_FOUND = ERR_UBP_DC_SDK_BASE-94;

/**
 * Cannot find user home.40094
 * 找到了用户的归属地 但与归属地MDC之间断链
 */
static const ACE_INT32 ERR_UBP_DC_SDK_ADS_ZONE_NOT_CONNECTED = ERR_UBP_DC_SDK_BASE-95;

/**
    \Server response timeout. 40096
    \服务器端响应超时  40096
*/
static const ACE_INT32 ERR_UBP_DC_SDK_REMOTE_TIMEOUT = ERR_UBP_DC_SDK_BASE-96;
/**
    \Server return error,the operation is MDC refused. 40097
    \服务器端返回错误, 操作被调度机拒绝  40097
*/
static const ACE_INT32 ERR_UBP_DC_SDK_REMOTE_FAILURE = ERR_UBP_DC_SDK_BASE-97; 
/**
    \The operation was not accepted,because reason unknown . 40098
    \操作不被接受, 原因不明 40098
*/
static const ACE_INT32 ERR_UBP_DC_SDK_GENERAL = ERR_UBP_DC_SDK_BASE-98;  	        
/**
    \The operation was not accepted,because may be implemented in futre. 40099
    \操作未实现， 可能在未来版本实现 40099
*/
static const ACE_INT32 ERR_UBP_DC_SDK_NOT_IMPLEMENTED = ERR_UBP_DC_SDK_BASE-99;    
/**
    \OSIP signaling handle error. 40100
    \OSIP信令处理内部错误 40100
*/
static const ACE_INT32 ERR_UBP_DC_SDK_OSIP_INTERNAL = ERR_UBP_DC_SDK_BASE-100;    
/**
    \Resverd. 40101
    \MS2媒体处理内部错误 40101
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MS2_INTERNAL = ERR_UBP_DC_SDK_BASE-101;    
/**
    \Zmq error. 40102
    \ZMQ命令处理内部错误 40102
*/
static const ACE_INT32 ERR_UBP_DC_SDK_ZMQ_INTERNAL = ERR_UBP_DC_SDK_BASE-102;    
/**
    \Timer for circulate diaplay failed. 40103
    \定时器处理内部错误 40103
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TIMER_INTERNAL = ERR_UBP_DC_SDK_BASE-103;    
/**
    \Parameter error. 40104
    \参数错误 40104
*/
static const ACE_INT32 ERR_UBP_DC_SDK_PARAMETER_ERROR = ERR_UBP_DC_SDK_BASE-104;    
/**
    \LICENSE excess system limit. 40105
    \LICENSE超出限制  40105
*/
static const ACE_INT32 ERR_UBP_DC_SDK_LICENSE_LIMIT = ERR_UBP_DC_SDK_BASE-105;   
/**
    \Operation already existed. 40106
    \已经存在，重复分发  40106
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TARGET_EXISTED = ERR_UBP_DC_SDK_BASE-106;   
/**
    \User not registered. 40107
    \ 该用户不存在  40107
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TARGET_NOT_FOUND = ERR_UBP_DC_SDK_BASE-107;   
/**
    \User not found. 40108
    \ 该用户没有注册  40108
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TARGET_NOT_REGISTERED = ERR_UBP_DC_SDK_BASE-108;   
/**
    \Timer scheduling failed. 40109
    \ 定时器调度失败  40109
*/
static const ACE_INT32 ERR_UBP_DC_SDK_SCHEDULE_FAIL = ERR_UBP_DC_SDK_BASE-109;    
/**
    \Failed to transfer the call. 40110
    \ 调度机转接失败  40110
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TRANSFER_FAILED = ERR_UBP_DC_SDK_BASE-110;   
/**
    \Number of dynamicgroup users beyong max. 40111
    \用户数量超出最大限制  40111
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_BEYOND_MAX_NUM = ERR_UBP_DC_SDK_BASE-111;
/**
    \Wrong dynamic group num. 40112
    \动态组创建时使用了错误的动态组号码  40112
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_WRONG_DMG_NUM = ERR_UBP_DC_SDK_BASE-112; 
/**
    \Number of dynamic group beyond max. 40113
    \动态组的成员超出范围  40113
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_BEYOND_MAX = ERR_UBP_DC_SDK_BASE-113;  
/**
    \Dynamic group creation timeout. 40114
    \动态组创建超时  40114
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_TIMEROUT = ERR_UBP_DC_SDK_BASE-114;         
/**
    \DynamicGroup member joined beyond limited max groups. 40115
    \动态组中组成员超出范围限制  40115
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_BEYOND_MAX_GROUP = ERR_UBP_DC_SDK_BASE-115; 
/**
    \DynamicGroup member not registered. 40116
    \动态组中有成员没有注册  40116
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_DMGMEMBER_NOT_RIGISTERED = ERR_UBP_DC_SDK_BASE-116;   
/**
    \Version mismatch. 40117
    \版本不匹配  40117
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_NO_MATCH_VERSION = ERR_UBP_DC_SDK_BASE-117;
/**
    \Dynamicgroup already existed. 40118
    \动态组已经存在  40118
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_DMG_ALREADY_EXISTED = ERR_UBP_DC_SDK_BASE-118; 
/**
    \No any valid user inthis dynamic group , please check wheher they are all external users or external groups. 40119
    \没有任何有效的用户在动态组内，请检测是否包含外部组或者外部用户  40119
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_NO_OTHER_MEMBER = ERR_UBP_DC_SDK_BASE-119;
/**
    \Network report error when handling DGNA. 40120
    \在创建动态组的时候网络返回错误  40120
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_ECN_HANDLE_FAIL = ERR_UBP_DC_SDK_BASE-120;
/**
    \Target is being listened,reject dc break in. 40121
    \目标正在通话中，拒绝调度台抢话  40121
*/
static const ACE_INT32 ERR_UBP_DC_SDK_ZMQ_BREAK_IN_REJECT = ERR_UBP_DC_SDK_BASE-121;
/**
    \Dispatch number from this video source excess max system limit. 40122
    \该视频源转发的数量已经超过系统限制    40122
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MAX_LIMIT = ERR_UBP_DC_SDK_BASE-122;
/**
    \Network handle assigning dynamic group  fail all ptt users failed. 40123
    \所有PTT用户不合法  40123
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_ALL_PTT_USER_FAILED = ERR_UBP_DC_SDK_BASE-123;
/**
    \Network handle assigning dynamic group fail version not supported. 40124
    \版本不支持  40124
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_VERSION_NOT_SUPPORTED = ERR_UBP_DC_SDK_BASE-124;
/**
    \Network handle assigning dynamic group fail download not allowed. 40125
    \不允许下载  40125
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_DOWNLOAD_NOT_ALLOWED = ERR_UBP_DC_SDK_BASE-125;
/**
    \Network handle assigning dynamic group fail wrong number. 40126
    \号码错误  40126
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_WRONG_NUMBER = ERR_UBP_DC_SDK_BASE-126;
/**
    \Create patchgroup numbers beyond limited max numbers. 40131
    \调度员超过最大创建派接数  40131
*/
static const ACE_INT32 ERR_UBP_DC_SDK_PATCH_BEYOND_MAX_NUM = ERR_UBP_DC_SDK_BASE-131;    
/**
    \Emergency services do not support this operation. 40132
    \紧急呼叫不支持强拆  40132
*/
static const ACE_INT32 ERR_UBP_DC_SDK_ZMQ_EMERGENCY_REJECT = ERR_UBP_DC_SDK_BASE-132; 
/**
    \No MIC, No Breakin allowed. 40133
    \不支持强拆操作  40133
*/
static const ACE_INT32 ERR_UBP_DC_SDK_BREAKIN_REJECT = ERR_UBP_DC_SDK_BASE-133;   
/**
    \Local ip is valid or network is disconnected. 40134
    \本地IP地址无效  40134
*/
static const ACE_INT32 ERR_UBP_DC_SDK_LocalIP_FAILURE = ERR_UBP_DC_SDK_BASE-134;   
/**
    \reservation. 40135
    \保留 40135
*/
static const ACE_INT32 ERR_UBP_DC_SDK_VERCHECK_FAILURE = ERR_UBP_DC_SDK_BASE-135; 
///**
//    \DynamicGroup operation unknown error number. 40136
//    \动态组操作未知错误码
//*/
//static const ACE_INT32 ERR_UBP_DC_SDK_DGNA_WRONG_NUMBER = ERR_UBP_DC_SDK_BASE-136;
/**
    \DynamicGroup operation has no response or is still establishing, please check later. 40137
    \后台服务无响应或动态组还在建立中，请稍后查看
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DMG_UDCTIMEROUT = ERR_UBP_DC_SDK_BASE-137;

/**
    \DynamicGroup operation UDC error. 40138
    \动态组操作UDC错误
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DMG_UDCERROR = ERR_UBP_DC_SDK_BASE-138;
/**
    \DynamicGroup operation ISDN error.40139
    \动态组操作无效的索引号码
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DMG_ISDNERROR = ERR_UBP_DC_SDK_BASE-139;
/**
    \DynamicGroup operation UDC synchronize error.40140
    \动态组操作UDC同步失败
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DMG_UDCSYNCERROR = ERR_UBP_DC_SDK_BASE-140;
///**
//    \DynamicGroup operation UDC timeout.40141
//    \动态组操作UDC超时
//*/
//static const ACE_INT32 ERR_UBP_DC_SDK_DMG_UDCTIMEROUT = ERR_UBP_DC_SDK_BASE-141;
/**
    \operation UDC login failure.40142
    \动态组OR修改密码操作UDC认证失败
*/
static const ACE_INT32 ERR_UBP_DC_SDK_UDCLOGINFAIL = ERR_UBP_DC_SDK_BASE-142;
/**
    \operation UDC inner error.40143
    \动态组OR修改密码操作UDC内部错误
*/
static const ACE_INT32 ERR_UBP_DC_SDK_UDCINNERERR = ERR_UBP_DC_SDK_BASE-143;
/**
    \PatchGroup id get failure.40144
    \派接组号码获取失败
*/
static const ACE_INT32 ERR_UBP_DC_SDK__PATCH_GROUP_DN_GET_FAIL = ERR_UBP_DC_SDK_BASE-144;
/**
    \TemporaryGroup id get failure.40145
    \临时组号码获取失败
*/
static const ACE_INT32 ERR_UBP_DC_SDK_TEMP_GROUP_DN_GET_FAIL = ERR_UBP_DC_SDK_BASE-145;

/**
  \Modify password get failure:old password is error. 40146
  \修改密码失败：旧密码验证失败
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MPWD_OLDPWDFAIL = ERR_UBP_DC_SDK_BASE - 146;
/**
  \Modify password get failure:new password is illegal . 40147
  \修改密码失败：新密码不合法
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MPWD_NEWPWDILLEGAL = ERR_UBP_DC_SDK_BASE - 147;
/**
  \Modify password get failure: dcid is invalid . 40148
  \修改密码失败：用户号无效
*/
static const ACE_INT32 ERR_UBP_DC_SDK_MPWD_DCIDINVALID = ERR_UBP_DC_SDK_BASE - 148;
/**
  \Modify password get failure: unknown
  \修改密码失败：未知错误
*/
static const ACE_INT32 ERROR_UBP_DC_SDK_MPWD_UNKNOWNFAIL = ERR_UBP_DC_SDK_BASE - 149;

static const ACE_INT32 ERR_UBP_DC_SDK_PARAMETER_BEYOND_SCOPE = ERR_UBP_DC_SDK_BASE - 150;
/**
  \Dispatch Video or Transfer audio get failure: Source Vpn and dispath user's vpn not allowed
  \视频分发或转接失败：vpn不支持
*/
static const ACE_INT32 ERR_UBP_DC_SDK_DISPATCH_VPN_NOT_ALLOWED = ERR_UBP_DC_SDK_BASE - 151;

/**
  \Creating PatchGroup reach system max number.
  \创建派接组达到系统最大数量,100
*/
static const ACE_INT32 ERR_UBP_DC_SDK_PATCH_GROUP_MAX_NUMBER_REACHED = ERR_UBP_DC_SDK_BASE - 152;

/**
  \Only gateway-user can be added in group temporarily.
  \临时用户当前只支持网关用户
*/
static const ACE_INT32 ERR_UBP_DC_SDK_USER_JOIN_INVALID_ISDN = ERR_UBP_DC_SDK_BASE - 153;

//DC_APP:-41000 ~ -41999
static const ACE_INT32 ERR_UBP_DC_APP_BASE = -41000;  //init asyn socket error



//DC_GUI:-42000 ~ -42999
static const ACE_INT32 ERR_UBP_DC_GUI_BASE = -42000;  //init asyn socket error



#endif //UBP_DC_MODULES_ERROR_H
