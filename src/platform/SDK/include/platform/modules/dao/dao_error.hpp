/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      dao_error.hpp
Author:        XiaoLongguang
Created:       2013/06/18 10:07
Description:
*********************************************************************/
#ifndef  UBP_PLATFORM_MODULES_DAO_DAO_ERROR_HPP_
#define  UBP_PLATFORM_MODULES_DAO_DAO_ERROR_HPP_

#include "platform/modules/error/ubp_error.hpp"

#define DAOSEGMENT 3799

///////////////////////////////////////////////////////////////////////
//platform：-1000 ~ - 9999 
//  |--MQ     : -1000 ~ -1099
//  |--SCA    : -1100 ~ -1199
//  |--FRM    : -1200 ~ -1299
//  |--Thread : -1300 ~ -1399
//  |--DAO    : -1400 ~ -2999
///////////////////////////////////////////////////////////////////////

//DAO : -1400 ~ -2999
//  |--MYSQL  : -1400 ~ -2799
//  |--DAO    : -2800 ~ -2999
//MYSQL原始错误码区间1000~2399，减去3799为-1400~-2799
static const ACE_INT32 ERR_UBP_DAO_DUP_ENTRY = -2737;      //1062：字段值重复，入库失败 1062-3799=-2737
static const ACE_INT32 ERR_UBP_DAO_NO_REF_ROW = -2583;     //1216：外键约束检查失败，更新子表记录失败 1216-3799=-2583
static const ACE_INT32 ERR_UBP_DAO_ROW_IS_REF = -2582;     //1217：外键约束检查失败，删除或修改主表记录失败 1217-3799=-2582
static const ACE_INT32 ERR_UBP_DAO_NO_TABLE = -2653;       //1146：表不存在 1146-3799=-2653
static const ACE_INT32 ERR_UBP_DAO_NO_FIELD = -2745;       //1054：列不存在 1054-3799=-2745
static const ACE_INT32 ERR_UBP_DAO_NO_REF_ROW2 = -2348;     //1451：外键约束检查失败，不能删除或更新父行 1451-3799=-2348
static const ACE_INT32 ERR_UBP_DAO_ROW_IS_REF2 = -2347;     //1452：外键约束检查失败，不能添加或更新子行 1452-3799=-2347

static const ACE_INT32 ERR_UBP_DAO_INIT = -2800;           //初始化
static const ACE_INT32 ERR_UBP_DAO_NULL_PTR = -2801;       //null_pointer--"NULL pointer"
static const ACE_INT32 ERR_UBP_DAO_IN_TRANS = -2802;       //already_in_transaction--"transaction already in progress in this thread"
static const ACE_INT32 ERR_UBP_DAO_NOTIN_TRANS = -2803;    //not_in_transaction--"operation can only be performed in transaction"
static const ACE_INT32 ERR_UBP_DAO_TRANS_FIN = -2804;      //transaction_already_finalized--"transaction already committed or rolled back"
static const ACE_INT32 ERR_UBP_DAO_IN_SESS = -2805;        //already_in_session--"session already in effect in this thread"
static const ACE_INT32 ERR_UBP_DAO_NOTIN_SESS = -2806;     //not_in_session--"session not in effect in this thread"
static const ACE_INT32 ERR_UBP_DAO_SESS_REQ = -2807;       //session_required--"session required to load this object relationship"
static const ACE_INT32 ERR_UBP_DAO_RECOVERABLE = -2808;    //recoverable--
static const ACE_INT32 ERR_UBP_DAO_DEADLOCK = -2809;       //deadlock--"transaction aborted due to deadlock"
static const ACE_INT32 ERR_UBP_DAO_CON_LOST = -2810;       //connection_lost--"connection to database lost"
static const ACE_INT32 ERR_UBP_DAO_TIMEOUT = -2811;        //timeout--"database operation timeout"
static const ACE_INT32 ERR_UBP_DAO_OBJ_NOTEXIST = -2812;   //object_not_persistent--"object not persistent"
static const ACE_INT32 ERR_UBP_DAO_OBJ_EXIST = -2813;      //object_already_persistent--"object already persistent"
static const ACE_INT32 ERR_UBP_DAO_OBJ_CHANGED = -2814;    //object_changed--"object changed concurrently"
static const ACE_INT32 ERR_UBP_DAO_RES_NOTCACH = -2815;    //result_not_cached--"query result is not cached"
static const ACE_INT32 ERR_UBP_DAO_DATA_EXC = -2816;       //database_exception
static const ACE_INT32 ERR_UBP_DAO_ABS_CLASS = -2817;      //abstract_class--"database operation on instance of abstract class"
static const ACE_INT32 ERR_UBP_DAO_NOTYPE_INFO = -2818;    //no_type_info--"no type information"
static const ACE_INT32 ERR_UBP_DAO_UNKNOWN_SCHEMA = -2819; //unknown_schema--"unknown database schema



#endif //UBP_PLATFORM_MODULES_DAO_DAO_ERROR_HPP_
