/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    provchange_indicator.h
Author:      Kai Wang
Created:     2012/12/07 08:36
Description: 本对象负责处理配置变更时从APP到GUI的指示
ProvChangeIndicator 为基类
ProvChangeIndicator_XXX 定义每一个变更的具体类
DisplayManager::NotifyProvChange(ProvChangeIndicator*) 使用一个基类的多态指针
使用者通过 getChangeMO来知道具体变化类并 down_cast 多态指针到具体类型
*********************************************************************/

#ifndef PROVCHANGE_INDICATOR_H
#define PROVCHANGE_INDICATOR_H

#include "sdk_export.h"
#include "provisionmanager.h"
#include "ubp_globe.hpp"

/**
定义变更类型
**/

typedef enum {

    CHANGE_ADD =OP_ADD,
    CHANGE_MODIFY =OP_MODIFY,
    CHANGE_DELETE =OP_DELETE,
    CHANGE_QUERY =OP_QUERY,
    CHANGE_BATCHADD =OP_BATCH_ADD,
    CHANGE_BATCHMODIFY =OP_BATCH_MODIFY,
    CHANGE_BATCHDELETE=OP_BATCH_DELETE,
    CHANGE_BATCHIMPORT =OP_BATCH_IMPORT,
    CHANGE_BATCHIMPORTCSV =OP_BATCH_IMPORT_CSV,

    CHANGE_ENABLE =20,
    CHANGE_DISABLE =21,

    CHANGE_UNKNOWN = -1

} Change_Type_t;

typedef enum {
    MO_PTTUser =0,
    MO_FixedUser,
    MO_DCAttribute,
    MO_Group,
    MO_DMGroup,
    MO_GroupPTTUser =5,
    MO_GroupFixedUser,
    MO_DynamicGroupMember,
    MO_DMGPhotoGroupUser,
    MO_UBPNode,
    MO_UBPService =10,
    MO_NeighbourNode,
    MO_BccCfg,
    MO_CallInhibited,
    MO_VGWDEC,
    MO_RangeList =15,
    MO_DCRanges,
    MO_HOTLINECFG,
    MO_STMSGCFG,
    MO_DECUSER,
    MO_UNKNOWN =20,
    MO_VGWDEVICE,
    MO_GISCFG,
    MO_GISIPC,
    MO_VPN,
    MO_VPNRange =25,
    MO_EXTERNALGROUP,
    MO_UEAbility,
    MO_DCDEC,
    MO_PatchGroup,
    MO_PatchGroupMember =30,
    MO_TERMINALLINKAGECFG,
    MO_EMERGRPTIME,
    MO_ALONEUSER,
    MO_MZONE_USER,              /** user MO object when query multi-zone*/
    MO_MZONE_GROUP =35,         /** group MO object when query multi-zone*/

    MO_PTTLINKCAMERA, // CRN05317
    MO_ExternalISDN,             /** users or groups from other zones*/
    MO_VideoLossRetransMission,
    MO_IPCATTRIBUTE
} Change_MO_t;

class SDK_IMPORT_EXPORT ProvChangeIndicator
{
public:
    virtual ~ProvChangeIndicator();
    void setChangeType(Change_Type_t changetype) {_changetype = changetype;}
    void setChangeMO(Change_MO_t changemo){_changemo = changemo;}
    virtual std::string datalog_str();
    Change_Type_t getChangeType(){return _changetype;}
    Change_MO_t getChangeMO(){return _changemo;}
    virtual ProvChangeIndicator* clone(){return NULL;}

    //added from TTR3.0
    virtual const char* serialize_ind(Json::Value&)=0;// { return NULL;}    //serialize into text format
    virtual bool de_serialize(const char* ){return false;}
protected:
    void cloneBase(ProvChangeIndicator& pci)
    {
        _changetype = pci.getChangeType();
        _changemo = pci.getChangeMO() ;
    }

private:
    Change_Type_t _changetype;
    Change_MO_t   _changemo;
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_PTTUser
    :public ProvChangeIndicator, public UserInfo
{
public:
    ProvChangeIndicator_PTTUser(const UserInfo& userinfo);
    //std::string datalog_str();
    virtual ProvChangeIndicator_PTTUser* clone();

    //added from TTR3.0
    virtual const char* serialize_ind(Json::Value &value);    //serialize into text format
    virtual bool de_serialize(const char* text);
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_Group
    :public ProvChangeIndicator, public GroupInfo
{
public:
    ProvChangeIndicator_Group(const GroupInfo& groupinfo);
    //std::string datalog_str();

    virtual ProvChangeIndicator_Group* clone();

    //added from TTR3.0
    virtual const char* serialize_ind(Json::Value &value);    //serialize into text format
    virtual bool de_serialize(const char* text);
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_GrpUserInfo
    :public ProvChangeIndicator, public GrpUserInfo
{
public:
    ProvChangeIndicator_GrpUserInfo(const GrpUserInfo& grpuserinfo);
    //std::string datalog_str();

    ProvChangeIndicator_GrpUserInfo* clone();

    //added from TTR3.0
    virtual const char* serialize_ind(Json::Value &value);    //serialize into text format
};

typedef  ProvChangeIndicator_PTTUser ProvChangeIndicator_FixedUser;

class SDK_IMPORT_EXPORT ProvChangeIndicator_DCAttribute
    :public ProvChangeIndicator, public DcProperty
{
public:
    ProvChangeIndicator_DCAttribute(const DcProperty& grpuserinfo);
    //std::string datalog_str();
    virtual const char* serialize_ind(Json::Value &value);
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_RangeList
    :public ProvChangeIndicator, public RangeInfo
{
public:
    ProvChangeIndicator_RangeList(const RangeInfo& rangeinfo);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_DCRange
    :public ProvChangeIndicator
{
public:
    ProvChangeIndicator_DCRange(MSISDN dcid,int rangid);
    //std::string datalog_str();

    virtual const char* serialize_ind(Json::Value &value);
    MSISDN dcid;
    int rangid;
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_HotlineCfg
    :public ProvChangeIndicator, public Hotline
{
public:
    ProvChangeIndicator_HotlineCfg(const Hotline& msg);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_TerminalLinkageCfg  //add by lwj
    :public ProvChangeIndicator, public TerminalLinkage
{
public:

    ProvChangeIndicator_TerminalLinkageCfg(const TerminalLinkage& msg);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};
class SDK_IMPORT_EXPORT ProvChangeIndicator_StMsgCfg
    :public ProvChangeIndicator, public SdsStMsgCfg
{
public:
    ProvChangeIndicator_StMsgCfg(const SdsStMsgCfg& msg);

    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_VgwDec
    :public ProvChangeIndicator
{
public:
    ProvChangeIndicator_VgwDec(int devIndex_, int nodeId_, int chnNum_, std::string devAlias_,int devStatus_);

    int DevIndex; //'大屏ID'
    int NodeId; //'VGW节点ID'
    int ChnNum; //'分屏数'
    std::string DevAlias; //'大屏描述'
    int DevStatus;       //解码器状态 

    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_GisCfg
    :public ProvChangeIndicator, public ProvGisCfg
{
public:
    ProvChangeIndicator_GisCfg(const ProvGisCfg& msg);

    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_GisIpc
    :public ProvChangeIndicator, public ProvGisIpc
{
public:
    ProvChangeIndicator_GisIpc(const ProvGisIpc& msg);

    ProvChangeIndicator_GisIpc* clone();

    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_PhotoGrpUserInfo
    :public ProvChangeIndicator, public PhotoGrpUserInfo
{
public:
    ProvChangeIndicator_PhotoGrpUserInfo(const PhotoGrpUserInfo& grpuserinfo);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};


class SDK_IMPORT_EXPORT ProvChangeIndicator_VPN
    :public ProvChangeIndicator, public VpnInfo
{
public:
    ProvChangeIndicator_VPN(const VpnInfo& vpn);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_VPNRange
    :public ProvChangeIndicator, public VpnRangeInfo
{
public:
    ProvChangeIndicator_VPNRange(const VpnRangeInfo& rangeinfo);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};


class SDK_IMPORT_EXPORT ProvChangeIndicator_UEAbility
    :public ProvChangeIndicator, public ProvUEAblity
{
public:    
    ProvChangeIndicator_UEAbility(const ProvUEAblity& ueability);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};


class SDK_IMPORT_EXPORT ProvChangeIndicator_PatchGroup
    :public ProvChangeIndicator, public PatchGroupInfo
{
public:
    ProvChangeIndicator_PatchGroup();
    ProvChangeIndicator_PatchGroup(const PatchGroupInfo& msg);

    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_PatchGroupMember
    :public ProvChangeIndicator, public PatchGroupMemberInfo
{
public:
    ProvChangeIndicator_PatchGroupMember();
    ProvChangeIndicator_PatchGroupMember(const PatchGroupMemberInfo& msg);

    virtual const char* serialize_ind(Json::Value &value);

};


class SDK_IMPORT_EXPORT ProvChangeIndicator_EmergencyGrp
    :public ProvChangeIndicator, public ProvEmgrpmber
{
public:
    ProvChangeIndicator_EmergencyGrp(const ProvEmgrpmber& emgrptime);

    virtual const char* serialize_ind(Json::Value &value);
    //std::string datalog_str();
};

/**
\brief ProvisionChangeIndicator for a batch query or import
\param list: the objects in the batch, which is a vector of ProvisionChange_Indicator object
\note ordinarilly, the Change_Type_t for this indicator is CHANGE_QUERY or CHANGE_BATCHIMPORT
*/
class SDK_IMPORT_EXPORT ProvChangeIndicator_Batch
    :public ProvChangeIndicator, public BaseInfo
{
public:

    ProvChangeIndicator_Batch();
    virtual const char* serialize(Json::Value& ){return NULL;}
    virtual const char* serialize_ind(Json::Value &value);
    virtual bool de_serialize(const char* );

    void append(ProvChangeIndicator* one);

public:
    std::vector<ProvChangeIndicator*> list;
};

// CRN05317
class SDK_IMPORT_EXPORT ProvChangeIndicator_PTTLinkCamera
    :public ProvChangeIndicator, public ProvPTTLinkCamera
{
public:
    ProvChangeIndicator_PTTLinkCamera(const ProvPTTLinkCamera& pttlinkinfo);

    virtual const char* serialize_ind(Json::Value &value);
};

class SDK_IMPORT_EXPORT ProvChangeIndicator_External_Isdn
    :public ProvChangeIndicator, public External_Isdn
{
public:
    ProvChangeIndicator_External_Isdn(const External_Isdn& exisdn);
    virtual const char* serialize_ind(Json::Value &value);

};

class SDK_IMPORT_EXPORT ProvChangeIndicator_VideoLossRetransMission
    :public ProvChangeIndicator, public VideoLossRetransmissionInfo
{
public:
    ProvChangeIndicator_VideoLossRetransMission(const VideoLossRetransmissionInfo& exisdn);
    virtual const char* serialize_ind(Json::Value &value);

};
class SDK_IMPORT_EXPORT ProvChangeIndicator_IPCATTRIBUTE
    :public ProvChangeIndicator, public CameraAttribute
{
public:
    ProvChangeIndicator_IPCATTRIBUTE(const CameraAttribute& ipc);
    virtual const char* serialize_ind(Json::Value &value);
};

#endif // PROVCHANGE_INDICATOR_H
