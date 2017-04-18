/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    provision_manager.h
Author:      Wang Kai
Created:     2012/12/04 14:00
Description: ProvisionManager API
*********************************************************************/
#pragma warning(disable: 4251)
#ifndef PROVISION_MANAGER_H
#define PROVISION_MANAGER_H

#include <list>
#include <map>
#include <vector>
#include <string>
#include "sdk/include/sdk_export.h"
#include "sdk/include/provision_helper.h"

namespace Json{
    class Value;
}

//forward declarations:
class ACE_Null_Mutex;
class ProvListener;
class UserStatusIndicator;
class GrpDspInfo;
class ProvChangeIndicator;
class DisplayManager;
typedef GrpDspInfo GroupStatusIndicator;
class LicenseListener;


class SDK_IMPORT_EXPORT ProvisionManager
{
    friend class ProvisonManager_T;

public:

    /**
    \brief get ProvisionManager service object
    \note please don't invoke this API, use ProvisionManager_T::instance() instead
    */
    static ProvisionManager* getInstance();

    /**
    \brief ProvisionManager initialize，will download provisioning data from server
    \param serverIP： IP address for eMDC server
    \param dcid： ID of DC
    */
    void init(std::string serverIP,MSISDN dcid);
    /**
    \brief init eMRS
    \param mrsServerIP: IP address for eMRS server
    \note : if eMRS and eMDC are deployed separately, you need invoke this API before querying recording files
    */
    int initMRS(std::string mrsServerIP);

    void exit();

    void setDisplayManager(DisplayManager* dm);
    DisplayManager* getDisplayManager();
    void setProvListener(ProvListener* pl);
    ProvListener* getProvListener();

    /**
    \brief get hotline configurations for this DC, the caller is responsible to destroy the clone data structure
    \param dcid： DCID for the invoking DC
    \return DcHotlines: vectors of hotline for this DC
    */
    DcHotlines* getDcHotlinesClone(MSISDN Dcid);

    Prov_Result_t addDcHotline(MSISDN dcid,const Hotline& hotline );
    Prov_Result_t delDcHotline(MSISDN dcid, const Hotline& hotline);

    bool isInDcRange(MSISDN userid, MSISDN dcid);
    bool isInOnlyDcRange(MSISDN userid, MSISDN dcid);

    /**
    \brief get managed groups for this DC, the caller is responsible to destroy the clone data structure
    \param dcid： DCID for the invoking DC
    \return DcGroups: vector of groups managed by this DC
    */
    DcGroups* getDcGroupsClone(MSISDN Dcid);
    /**
    \brief get managed groups for this DC by group type, the caller is responsible to destroy the clone data structure
    \param dcid： DCID for the invoking DC
    \param cat: category of group object
    \return DcGroups: vector of groups managed by this DC
    */
    DcGroups* getDcGroupsByType(MSISDN Dcid, Group_Category_t cat);


    /**
    \brief get managed users for this DC , the caller is responsible to destroy the clone data structure
    \param dcid： DCID for the invoking DC
    \return DcUsers: vector of users managed by this DC
    */
    DcUsers* getDcUsersClone(MSISDN Dcid);
    DcUsers* getDcUsersCloneByType(MSISDN Dcid, User_Category_t cat);

    /**
    \brief get users in a group, the caller is responsible to destroy the clone data structure
    \param grpid： group id
    \return GrpUserlist: vector of users in a group
    */
    GrpUserList* getGroupUsersClone(MSISDN  grpid);

    GrpUserList* getDynamicGroupPhotoUsersClone(MSISDN  dynamicgrpid, MSISDN photogrpid);
    /**
    \brief get detail information for a group
    \param grpid: group ID
    \return GroupInfo: details about a group, NULL if the group doesn't exist
    */
    GroupInfo* getGroupInfoClone(MSISDN  grpid);

    /**
    \brief get detail information for a user
    \param userid: user ID
    \return UserInfo: details about a user, NULL if the user doesn't exist
    */
    UserInfo*  getUserInfoClone(MSISDN userid);

    /**
    \brief get detail information for a DC (Dispatch Console)
    \param dcid: DC id
    \return DcProperty: details about a DC, NULL if the DC doesn't exist
    */
    DcProperty* getDcProperty(MSISDN Dcid);

    int getDcUserPriority(MSISDN Dcid);

    VpnInfo* getDcVpnINfo(MSISDN Dcid);

    bool isGroupMember(MSISDN userid, MSISDN grpid);


    /**
    \brief get status message defintion for a status code
    \param code: status code
    \return SdsStMsgCfg: details about a status message, NULL if it doesn't exist
    */
    SdsStMsgCfg* getStatusMsgClone(/*unsigned*/ int code);

    /**
    \brief get video wall list (video output channels)
    \return VWallInfoList: a list of video walls managed by this DC, return NULL if no video walls
    */
    VWallInfoList* getVWallsClone();


    void notifyProvisionChange(ProvChangeIndicator* pci);
    void notifyProvisionAllResync();

    Prov_Result_t GetGisLic(const std::string &aCtrlItm,std::string &Itmvalue);
    LicenseListener *getListener();
    void setListener(LicenseListener *Listener);

    /**
    \brief recording files from eMRS
    \param condition: query condition, please see RECQueryInfo definition for more details
    \return UserRecFileInfoList: list of recording files, NULL if no recording files are found with this condition
    */
    UserRecFileInfoList* getUserRecFileInfoList( RECQueryInfo* condition);

    /**
    \brief query ability for a mobile terminal
    \param ueid: ID of the mobile terminal
    \return ProvUEAblity: ability of this UE, reported when UE power up
    */
    ProvUEAblity* getUEAbilityClone(MSISDN ueid);

    /*-----------------------TTR3.0----------------------- */

    /**
    \brief initMRS with dcid, security and privilege checking
    */
    int initMRS(std::string mrsServerIP, std::string localIP, MSISDN dcid, std::string pwd);

    /**
    \brief query patch groups those contains this DC
    \param dcid: ID for this DC
    \return PatchGroupsList: list of patch groups containing this DC as a member
    */
    PatchGroupsList * getPatchGroupsClone(MSISDN dcId);

    /**
    \brief query members in a patch group
    */
    PatchGroupMembers * getGroupMemberByPatchId(MSISDN patchId);
    /**
    \brief query patch Group according one of its member
    \note: one member can be in one group at a time
    */
    PatchGroupInfo * getPatchInfoCloneByMemberId(MSISDN memberId);
    /**
    \brief query patch group detail according patch group id
    */
    PatchGroupInfo *  getPatchGroupById(MSISDN patchId);

    /*-----------------------TTR4.0----------------------- */
    /**
    \brief query GIS report configuration for a mobile terminal
    \param userid: ID for this mobile Terminal
    \return ProvGisCfg: GIS report config specified for this Terminal
    \note: if return NULL, it means this terminal is using a default GIS report config
    */
    ProvGisCfg* queryUserSpecificGISCfg(MSISDN userid);

    /**
    \brief query GIS subscriptions for this DC
    \param dcid: ID of DC
    \return GisQuerySubList: a list of terminal those will report GIS location to this DC
    \note: if return NULL, this DC has not subscribed any terminals for GIS reporting
    */
    GisQuerySubList* queryGISSubscription(MSISDN dcid);


    /**
    \brief recording tasks from eMRS,when got tasks you can query files by it
    \param condition: query condition, please see RECQueryTaskInfo definition for more details
    \return UserRecTaskInfoList: list of recording tasks, NULL if no recording tasks are found with this condition
    */
    UserRecTaskInfoList* qryUserRecTaskInfoList( RECQueryTaskInfo* condition);

    /**
    \brief recording files from eMRS,you can not use this until you have got tasks by the function of 'qryUserRecTaskInfoList'
    \param condition: query condition, please see RECQueryFileInTask definition for more details
    \return UserRecFileInTaskInfoList: list of recording files, NULL if no recording files are found with this condition
    */
    UserRecFileInTaskInfoList* qryUserRecFileInTaskInfoList( RECQueryFileInTask* condition);

    /**
    \brief recording PTT files from eMRS
    \param condition: query condition, please see RECQueryPttInfo definition for more details
    \return UserRecPttInfoList: list of recording PTT files, NULL if no recording PTT files are found with this condition
    */
    UserRecPttInfoList* qryUserRecPttInfoList(RECQueryPttInfo* condition);

    /**
    \brief recording terminal files from eMRS
    \param condition: query condition, please see RECQueryUEInfo definition for more details
    \return UserRecUEInfoList: list of recording terminal files, NULL if no recording terminal files are found with this condition
    */
    UserRecUEInfoList* qryUserRecUEInfoList(RECQueryUEInfo *condition);

    /**
    \brief init eMRS infomation for DConsole
    \return ONLY number zero means OK, else failed
    \note: before query record files, you need to login eMRS. AND it was reserved for INTERNAL use only
    */
    MSISDN initMRS4DC(std::string mrsServerIP);

    /**
    \brief before you invoke 'initMRS4DC',you may need to know the 'mrsServerIP',then you can get it by 'getUBPNodeInfoList'
    \return UBPNodeInfoList: list of UBPNode infomation, please see 'UBPNodeInfoList' definition for more details
    \note: it was reserved for INTERNAL use only
    */
    UBPNodeInfoList* getUBPNodeInfoList();

    /**
    \brief if you invoke 'getUserRecFileInfoList' API,and want to change the offset or filenum, you can do it by this
    \param offset: start querying position
    \param filenum: wish to query the numbers of files
    */
    void setPagesConfigure4DC(MSISDN offset,MSISDN filenum);

    /**
    \brief get members in a dynamic group, filter by member type
    \param dmgrpid： dynamic group id
    \return GrpUserlist: vector of members in the dynamic group
    \note   you can filter by member type to get group list or user list in the dynamic group
    */
    GrpUserList* getDynamicGroupMembers(MSISDN  dmgrpid);

    /**
    \brief get users proxy register informations
    \param from: user isdn begin
    \param to: user isdn end
    \return UserRegisterList: vector of users register informations
    \note: if set from and to with -1,then eSDK will query all users proxy register informations.
    */
    UserRegisterList* getUserRegisterInfo(MSISDN from, MSISDN to);

    //终端绑定摄像头信息查询
    ProvPTTLinkCamera* getPttLinkCameraClone(std::string pttisdn);

    /**
    \brief when creating a patchGroup, you can invoke this API to get the current sequence.
    \return @int PatchGroup sequence
    */
    MSISDN getCurrentPatchSeq();

    /**
    \brief get patchGroup informations by group id
    */
    PatchGroupInfo * getPatchGroupCloneById(MSISDN pid);

    bool isPatchGroup(MSISDN grpid);
    /**
    \brief get the value whcih can tell you whether camera support viedo loss retransmission or not
    */
    VideoLossRetransmissionInfoList* getVideoLossRetransMissionInfo();

    /**
    \brief by this you can get the cameras' shape infomation
    \param dcid: ID of DC
    \return CameraAttributeList: vector of 'CameraAttribute'
    */
    CameraAttributeList* getDcCameraAttributeCloneByDcID(MSISDN dcid);
    /**
    \brief by this you can get a camera's shape
    \param cameraid: ID of camera
    \return type of 'CameraShape_t'
    */
    CameraShape_t getDcCameraAttributeByCameraId(MSISDN cameraid);

    /**
    \brief Check the success or failure information of the SDK load key tables
    \return 32 bit value, user should compare the value with LOADDATA_OK, equal means success, or failed
    \note:
    KeyTable[T_KEYTBL_NUM] ={T_DCATTRIBUTES,T_RANGELIST,T_DCRANGES,T_VPNINFO,T_VPNRANGE,T_GROUP,T_PTTUSER,T_FIXEDUSER,T_GROUPPTTUSER,T_GROUPFIXEDUSER};
    it was reserved for INTERNAL use only!
    */

    unsigned int getLoadTablesInfo();
    /**
    \brief Check this group whether still exist in ExternalISDN Table.
    \param group isdn
    \return true or false
    \note: It was reserved for INTERNAL use only!
    Call this API when receiving MO_Group notifyProvisionChange.If return true, please ignore this notify.
    */

    bool isExistExternalISDNDatas(const int& groupid);

public:
    ProvisionManager();

private:
    DcUsers* getDcUsersCloneByTypeAndVpnId(MSISDN Dcid, int VpnId, User_Category_t cat);
    DcGroups* getDcGroupsByTypeAndVpnId(MSISDN Dcid, int VpnId, Group_Category_t cat);
};

//typedef ACE_Unmanaged_Singleton<ProvisionManager, ACE_Null_Mutex> ProvisionManager_T;

class SDK_IMPORT_EXPORT ProvisionManager_T
{
public:
    /// Global access point to the Singleton.
    static ProvisionManager *instance (void);
};

#endif // PROVISION_MANAGER_H
