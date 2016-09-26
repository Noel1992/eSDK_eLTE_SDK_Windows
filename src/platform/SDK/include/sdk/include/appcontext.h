/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    appcontext.h
Author:      Wu yu wei
Created:     2012/07/07 10:00
Description:
*********************************************************************/
#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H
#include <memory>
#include <string>
#include "sdk_export.h"


/**
  \brief TriggerStatusReport() options
*/
#define TRIGGER_STATUS_REPORT_RESTYPE_ALL         0            //trigger status report for all types resource
#define TRIGGER_STATUS_REPORT_RESTYPE_GROUP       1            //trigger status report for group resources
#define TRIGGER_STATUS_REPORT_RESTYPE_USER        2            //trigger status report for user resources
#define TRIGGER_STATUS_REPORT_RESTYPE_CAMERA      4            //trigger status report for camera resources

class DcState;

class DisplayManager;

class LicenseControl;

class ConnectionControl;

class MediaManager;

class GISReportManager;

class Peripheral_report_manager;

class DcMqContext;

struct eXosip_t;

class KeepAliveTimeoutHandler;

/**
   \brief setWorkMode() options
**/
typedef enum {

    APPMODE_DISPATCH_CONSOLE,           //SDK_DC
    APPMODE_VIDEO_GATEWAY,		//reserved
    APPMODE_LOGGING_SERVER,		//logging server
    APPMODE_LISTEN_SERVER,		//reserved
    APPMODE_DEC_GATEWAY,		//reserved
    APPMODE_APP_GATEWAY                 //SDK_GW
} App_Mode_t;


/**
  \brief biz_type options for different archiving folder
**/
typedef enum {

   MMS,  /** Message Path **/
   GIS,  /** GIS Configuration Path**/
   REC,   /** Local recording Path **/
   SUB,   /** Resource Subscription Path **/
   CERTW,   /** Certificate working Path **/
   CERTD    /** Certificate default deployed path*/
} Biz_t;

// added by JINHONG on 2016/03/31 for SDK
/**
     \If set, 3rd app SHOULD free pointer yourself, otherwise SDK will free pointer automatically.
    default the flag is false.
**/
typedef enum {
    MEM_SDK_MGR,    // SDK mode.
    MEM_UPPER_LAY_MGR   // upper layer mode.
} Mem_Mgr_Mode_t;

#define AUTO_RELEASE_SDK_MEM(ptr)   {\
    ApplicationContext *ac = ApplicationContext_T::instance();\
    if(ac->getMemMgrMode() == MEM_SDK_MGR && (ptr) != NULL)\
    {\
        delete ptr;\
    }\
}

class ApplicationContext_Imp;

class SDK_IMPORT_EXPORT ApplicationContext
{

public:   
    ApplicationContext();

    /**
      \brief get DcState object
    */
    virtual DcState * getConsoleState();

    /**
     \brief get ApplicationContext singleton object
    */
    static ApplicationContext* getInstance();
    
    /**
      \brief initialize ApplicationContext object
    */
    virtual int initApp();
    /**
      \brief exit ApplicationContext object
    */
    virtual int exitApp();


    /**
     \brief get DisplayManager callback interface
    */
    virtual DisplayManager * getDisplayManager();
    /**
     \brief set DisplayManager callback interface
    */
    virtual void setDisplayManager(DisplayManager*);

    /**
     \brief get LicenseControl callback interface
    */
    virtual LicenseControl * getLicenseControl();
    /**
     \brief set LicenseControl callback interface
    */
    virtual void setLicenseControl(LicenseControl* );

    /**
     \brief get ConnectionControl callback interface
    */
    virtual ConnectionControl * getConnectionControl();
    /**
     \brief set ConnectionControl callback interface
    */
    virtual void setConnectionControl(ConnectionControl* );

    /**
     \brief get MediaManager callback interface
    */
    virtual MediaManager * getMediaManager();
    /**
     \brief set MediaManager callback interface
    */
    virtual void setMediaManager(MediaManager* );

    /**
     \brief get GISReportManager callback interface
    */
    virtual GISReportManager * getReportManager();
    /**
     \brief set GISReportManager callback interface
    */
    virtual void setReportManager(GISReportManager * );

    /**
     \brief get Peripheral_report_manager callback interface
    */
    virtual Peripheral_report_manager * getPeripheralManager();
    /**
     \brief set Peripheral_report_manager callback interface
    */
    virtual void setPeripheralManager(Peripheral_report_manager * );

    /**
     \brief get the flag whether SDK will process RTP Media
    */
    virtual bool bypassBuildMedia();
    /**
     \brief get the flag whether SDK will process RTP Media
     \note The flag is applied to both audio and video RTP media
    */
    virtual void setBypassBuildMedia(bool );


    // added by JINHONG on 2016/03/31 for SDK
    /**
     \If set, 3rd app SHOULD free pointer yourself, otherwise SDK will free pointer automatically.
    default the flag is false.
    **/
    virtual void setMemMgrMode(Mem_Mgr_Mode_t mode);
    virtual Mem_Mgr_Mode_t getMemMgrMode();

    /**
     \brief set SDK work mode
    */
    virtual void setWorkMode(App_Mode_t );
    /**
     \brief get SDK work mode
    */
    virtual App_Mode_t getWorkMode();


    /**
     \brief trigger to start receive real-time status notification
    */
    virtual void triggerStatusReport(bool);

    virtual bool isStatusReportTriggered();


    /**
     \brief start running SDK
     \param serverIP: IP address of MDC server
     \return: non-zero means failure, zero means success; Your application shall always check this return value
     \note: this API shall be invoked after DC_LOGIN operation succeed
    */
    virtual int startRunning(std::string & serverIP);

    /**
     \brief stop running SDK
    */
    virtual int stopRunning();

    virtual eXosip_t *getExContext();
    virtual void setExContext(eXosip_t *);

    /**
     \brief get local SIP port
    */
    virtual int GetLocalSipPort() ;
    /**
     \brief set local SIP port
    */
    virtual void SetLocalSipPort(int );

    virtual DcMqContext * getDcMqContext();

    /*--------------------added from TTR2.1----------------*/

    /**
     \brief start running SDK
     \param serverIP: IP address of MDC server, a char* parameter for easy adaption to C#/JAVA
     \return: non-zero means failure, zero means success; Your application shall always check this return value
     \note: this API shall be invoked after DC_LOGIN operation succeed
    */
    virtual int startRunning(const char* serverip);
    /**
     \brief set the flag whether SDK will process video RTP
     \note This flag is only applied to video,
           this is for some situations when you want SDK help processing audio while video is processed by yourself
    */
    virtual void setBypassBuildVideo(bool );
    /**
     \brief get the flag whether SDK will process video RTP
    */
    virtual bool bypassBuildVideo();
   
        /*-------------------added from TTR3.0 -----------------*/
	

    virtual void setMix(bool);
    virtual bool getMix();

    /**
     \brief trigger to start receive real-time status notification
     \param flag: flag whether start receiving or not
     \param resource_type: resource type option
     \note: This API can be used to trigger status notification by resource types, such as user, group, or camera
    */
    virtual void triggerStatusReport(bool flag, unsigned int resource_type);
		

    virtual void setRemoteMode(bool);
    virtual bool isRemoteMode();

    /**
     \brief set the flag whether SDK will process audio or video RTP
     \param audio: flag whether audio RTP is bypassed (means NOT processed by SDK)
     \param video: flag whether video RTP is bypassed (means NOT processed by SDK)
     \note This API is used to set media processing flag separatly
           It can replace setBypassBuildMedia(bool) and setBypassBuildVideo(bool);
    */
    virtual void setBypassBuildMedia(bool audio, bool video);

    /*-------------------added from TTR4.0 -----------------*/
    /**
      \brief set/get the flag whether DC connects the eLTE by private network
    */
    bool getNATIndicator();
    void setNATIndicator(bool bnat);
    /**
      \brief set/get the flag whether DC connects the eLTE by public network
    */
    bool getPNATIndicator();
    void setPNATIndicator(bool bpnat);

    virtual std::string getBizSavePath(Biz_t btp);
private:

    static ApplicationContext_Imp* inner;

    static ApplicationContext* proxy;
};

//typedef ACE_Singleton<ApplicationContext, ACE_Null_Mutex> ApplicationContext_T;

class SDK_IMPORT_EXPORT ApplicationContext_T
{
public:
  /// Global access point to the Singleton.
  static ApplicationContext *instance (void);
};

#endif // APPLICATIONCONTEXT_H
