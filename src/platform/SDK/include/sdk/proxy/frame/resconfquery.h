/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    resconfdispatcher.hpp
Author:      Wu yu wei
Created:     2012/12/06 11:30
Description: 资源配置更新信号发生器
*********************************************************************/
#ifndef RESCONFQUERY_H
#define RESCONFQUERY_H

#include "boost/shared_ptr.hpp"
#include <map>
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "provisionmanager.h"
#include "sdk_export.h"

class ACE_Null_Mutex;

template <class TYPE, class ACE_LOCK> class ACE_Singleton;

////////智能指针接收SDK配置信息/////////
/** 用户信息 **/
typedef boost::shared_ptr<UserInfo> UserInfoRef;
/** 群组信息 **/
typedef boost::shared_ptr<GroupInfo> GroupInfoRef;
/** 用户群组关联信息 **/
typedef boost::shared_ptr<GrpUserInfo> GroupUserInfoRef;
/** Range信息 **/
typedef boost::shared_ptr<RangeInfo> RangeInfoRef;
/** 热线信息 **/
typedef boost::shared_ptr<Hotline> HotlineInfoRef;
/** 墙屏信息 **/
typedef boost::shared_ptr<VWallInfo> WallInfoRef;

/** VPN信息 **/
typedef boost::shared_ptr<VpnInfo> VpnInfoRef;
/** GisIpc信息 **/
typedef boost::shared_ptr<ProvGisIpc> GisIpcRef;

/** 定义缓存结构 **/
typedef std::vector<UserInfoRef> UserInfoVec;
typedef std::vector<UserInfoRef>::const_iterator UserInfoVecIterator;

typedef std::vector<GroupInfoRef> GroupInfoVec;
typedef std::vector<GroupInfoRef>::const_iterator GroupInfoVecIterator;


typedef std::vector <GroupUserInfoRef> GrpUserInfoVec;
typedef std::vector<GroupUserInfoRef>::const_iterator GrpUserInfoVecIterator;

typedef std::vector <WallInfoRef> WallInfoVec;
typedef std::vector<WallInfoRef>::const_iterator WallInfoVecIterator;

typedef std::vector <VpnInfoRef> VpnInfoVec;
typedef std::vector<VpnInfoRef>::const_iterator VpnInfoVecIterator;

typedef std::vector<GisIpcRef> GisIpcVec;
typedef std::vector<GisIpcRef>::const_iterator GisIpcVecIterator;


typedef boost::shared_ptr<ProvChangeIndicator> ProvChangeIndicatorRef;

typedef std::vector<GroupInfoRef> GroupInfoVec;
typedef std::vector<GroupInfoRef>::const_iterator GroupInfoVecIterator;

typedef boost::shared_ptr<BaseInfo> BaseInfoRef;
typedef std::vector <BaseInfoRef> BaseInfoRefVec;
typedef std::vector<BaseInfoRef>::const_iterator BaseInfoRefVecIterator;


class SDK_IMPORT_EXPORT ResConfQuery
{
     friend class ACE_Singleton<ResConfQuery, ACE_Null_Mutex>;
     //changeVisitor declared as friend class
     friend class ChangeVisitor;
	     
public:
    explicit ResConfQuery();

    /*从SDK接口查询封装群组信息，主要目标为GUI构造智能指针，进行自动销毁
    */
    std::auto_ptr<GroupInfoVec> getDcGroupsClone(MSISDN Dcid);

    /*从SDK接口查询封装用户信息，主要目标为GUI构造智能指针，进行自动销毁
    */
    std::auto_ptr<UserInfoVec> getDcUsersClone(MSISDN Dcid);

    /*从SDK接口查询封装用户信息，按类型查询，主要目标为GUI构造智能指针，进行自动销毁
    */
    std::auto_ptr<UserInfoVec> getDcUsersCloneByType(MSISDN Dcid, User_Category_t cat);


    /*从SDK接口查询封装用户群组关联对象信息，主要目标为GUI构造智能指针，进行自动销毁
    */
    std::auto_ptr<GrpUserInfoVec> getGroupUsersClone(MSISDN  grpid);

    /*
      从SDK查询封装摄像头列表，主要目标为GUI构造智能指针，自动销毁
      @param dcid: 本调度台的用户号;如果 Dcid==0， 则返回全网所有的Camera User
    */
    std::auto_ptr<UserInfoVec> getDcCameraUsersClone(MSISDN Dcid);

    /* 从SDK接口查询封装单个用户信息，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<UserInfo> getDcSingleUserClone(MSISDN resid);

    /* 从SDK接口查询封装单个VPN信息，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<VpnInfo> getDcVpnClone(MSISDN resid);

    /* 从SDK接口查询封装VPN信息，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<VpnInfoVec> getVpnsClone();

    /* 从SDK接口查询封装单个群组信息，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<GroupInfo> getDcSingleGroupClone(MSISDN grpid);

    /* 从SDK接口查询封装墙屏信息，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<WallInfoVec> getWallInfoClone();

    /* 从SDK接口查询封装某VPN下游离于组外的离散用户，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<UserInfoVec> getAloneDcUsersClone(MSISDN Dcid, int VpnId);

    std::auto_ptr<UserInfoVec> getDcCameraUsersClonebyVpnId(MSISDN Dcid,int VpnId);

    std::auto_ptr<UserInfoVec> getDcUsersCloneByTypeAndVpnId(MSISDN Dcid, int VpnId, User_Category_t cat);

    std::auto_ptr<GroupInfoVec> getDcGroupsClonebyVpnId(MSISDN Dcid,int VpnId);

    /* 从SDK接口查询封装GisIpc数据 */
    std::auto_ptr<GisIpcVec> getDcGisIpcClone(MSISDN Dcid);

    /* 从SDK接口查询封装外部用户，主要目标为GUI构造智能指针，自动销毁 */
    std::auto_ptr<UserInfoVec> getDcExternalUsersCloneByType(MSISDN dcid, User_Category_t cat);

protected:

};

typedef ACE_Unmanaged_Singleton<ResConfQuery, ACE_Null_Mutex> ResConfQuery_T;

#endif // RESCONFDISPATCHER_H
