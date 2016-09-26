
#ifndef STATUSOBSERVER_H
#define STATUSOBSERVER_H

#include "sdk_export.h"

class GrpDspInfo;
class ResourceStatusIndicator;
class P2pcallStatusIndicator;
class P2pvideocallStatusIndicator;
class ProvChangeIndicator;
class SdsMessageIndicator;
class DCModuleStatusIndicator;

class SDK_IMPORT_EXPORT DisplayManager
{
public:    
    //virtual ~DisplayManager(){};

    /**
    \brief When a group call occurs, the SDK calls back this function to send a status event to a third-party application
    \param grpInfo: Pointer to a group status event, generally it includes a status value and details for the status
    */
    virtual void notifyGroupStatus(GrpDspInfo * grpInfo)=0;

    /**
    \brief When the status of a resource changes, the SDK calls back this function to send a status event to a third-party application
    \param resourceStatus: Pointer to a resource status event, generally it includes a status value and details for the status
    */
    virtual void notifyResourceStatus(ResourceStatusIndicator * resourceStatus)=0;

    /**
    \brief When a P2P call occurs, the SDK calls back this function to send a status event to a third-party application
    \param callStatus: Pointer to a P2P call status event, generally it includes a status value and details for the status
    */
    virtual void notifyP2pcallStatus(P2pcallStatusIndicator * callStatus)=0;

    /**
    \brief When a video call or video upload request is initiated, the SDK calls back this function to send a status event to a third-party application
    \param callStatus: Pointer to a video call status event, generally it includes a status value and details for the status
    */
    virtual void notifyP2pvideocallStatus(P2pvideocallStatusIndicator * callStatus)=0;

    /**
    \brief When a service dispatch occurs on a resource, include the successful or failed registration status of the PTT user and fixed user, the SDK calls back this function to send a resource status event.
    \note Dispatch services include P2P voice calls, PTP video calls, video upload, video distribution, and video-on-wall
    */
    virtual void notifyUserStatus(ResourceStatusIndicator * resourceStatus)=0;

    /**
    \brief Status messages are a special type of short message. The SDK calls back the function to notify a third-party application that a status message is received
    \note please use SdsMessageIndicator::getStatusCode() to get the code of this status message
    */
    virtual void notifyUserSdsStatus(SdsMessageIndicator * sdsmsg)=0;

    /**
    \brief Notify connecting status for different modules ,such as SIP, SDS, etc
    */
    virtual void notifyModuleStatus(DCModuleStatusIndicator * moduleStatus)=0;

    /**
    \brief Upon receiving a text/multimedia message, the dispatching console calls back this function to send an event to a third-party application
    */
    virtual void notifySMS(SdsMessageIndicator * sdsmsg)=0;

    /**
    \brief When a configuration change occurs, the SDK calls back this function to notify a third-party application of the object on which the change occurs and the type of change (addition/deletion/modification).
    \param ind: a ProvChangeIndicator object, generally it includes change_type and change_MO
    \note The ProvChangeIndicator* parameter is a polymorphic pointer (base class pointer). Based on the value of change_MO in ProvChangeIndicator*, the SDK generates a derived class and passes in the derived class pointer.
    */
    virtual void notifyProvisionChange(ProvChangeIndicator* ind)=0;

    /**
    \brief The SDK automatically downloads configuration data during a startup. After the downloading, the SDK calls back this function to send an event to a third-party application.
    \note The third-party application can call the ProvisionManager interface only after receiving this event. Otherwise, the returned value is NULL
    */
    virtual void notifyProvisionAllResync(ProvChangeIndicator* )=0;

    /**
    \brief reserved
    */
    virtual void notifyStatusAllResync(ResourceStatusIndicator* )=0;

};

class SDK_IMPORT_EXPORT ProvListener
{
public:

    virtual void notifyProvisionChange(ProvChangeIndicator* ) =0;

    virtual void notifyProvisionAllResync(ProvChangeIndicator* ) =0;

};

#endif // STATUSOBSERVER_H
