// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifndef ELTE_RM_SDK_EXPORTS
#define ELTE_RM_SDK_EXPORTS
#endif

typedef enum
{
	/*-------------------������ϢԪ�����ڵ����������Ƶ�������p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //���״̬
	CALLERINFO =1, //������
	CALLEEINFO =2, //������

	/*--------------------������ϢԪ�������������grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, //���״̬
	GRPCALLSPERKER =4, //���������
	FLOORSTATUS =5,    //��Ȩ״ָ̬ʾ

	/*--------------------������ϢԪ�����ڽ������У� ��emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //��������ָʾ
	EMERGENCYCALLER =7,  //����������

	/*--------------------������ϢԪ��������Դ״̬����,��resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //��ԴID
	DEVICEID =9,               //��Ƶ����ʱ�Ĵ��ں�
	RESSELECTSTATUS =10,        //��Դѡ��״ָ̬ʾ�� ���ڶ�GUI����
	RESASSIGNSTATUS =11,        //��Դָ��״ָ̬ʾ�� ������̨�Ƿ�������Դ��
	RESTXSTATUS =12,            //��Դռ��״ָ̬ʾ
	RESLOGGINGSTATUS =13,       //��Դ¼��״ָ̬ʾ
	RESDLSTATUS =14,            //��Դ����״ָ̬ʾ
	RESREGSTATUS_PROXY =15,     //��Դע��״ָ̬ʾ, ����ע��(���������û����ն˼�Ⱥע��)
	RESREGSTATUS_DIRECT =16,    //��Դע��״ָ̬ʾ, ֱ��ע��(�ն�ҵ��ע�ᣬ����Ƶ)
	ATTACHINGGROUP =17,         //��Դ��������

	/*--------------------������ϢԪ�������û�״̬����;��userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//�û�״ָ̬ʾ�� һ����BCC֪ͨ
	USERGPSSTATUS =19,       //�û�GPS״ָ̬ʾ
	USERDLSTATUS =20,            //�û�����״ָ̬ʾ
	USERDGNASTATUS =21,      //�û���̬����״̬
	VIDEODISPATCHSTATUS =22,     //��Ƶ�ַ�״̬
	MUTESTATUS =23,          //����״̬
	GRPPATCHSTATUS =24  //�ɽӲ���״̬
}BundleKey;

/**
   ������Դ״ָ̬ʾ���ݽṹ
**/
typedef  BundleKey resource_status_t;

typedef enum {
    RESOURCE_STATUS_UNKNOWN = 4000,
    STATUS_SELECTED = 4001,    //��Դѡ��״̬
    STATUS_UNSELECTED = 4002,  //��Դȥѡ��״̬

    STATUS_ASSIGNED = 4003,    //��Դָ��״̬-
    STATUS_DEASSIGNED = 4004,  //��Դȥָ��״̬-

    STATUS_LISTENING = 4005,   //��Դ������״̬
    STATUS_LISTENEND = 4006,   //��Դ��������״̬

    STATUS_LOGGING = 4007,      //��Դ¼����״̬
    STATUS_NOLOGGING = 4008,    //��Դδ����¼��

    STATUS_INPATCH = 4009,      //��Դ���ɽ�
    STATUS_OUTPATCH = 4010,     //��Դ����ɽ�

    STATUS_REGOK = 4011,        //��Դע��
    STATUS_REGFAIL = 4012,      //��Դȥע��
    STATUS_FORBIDEN = 4013,     //��Դδ��Ȩ 500
    STATUS_NOT_FOUND = 4014,    //��Դ������ 404
    STATUS_TEMP_UNAVAILABLE = 4015, //��Դ��ʱ������ 480
    STATUS_RES_CONFILCT = 4016, //��Դ��ͻ 450
    STATUS_UNAUTHORIZED = 4017, //δ��Ȩ 401
    STATUS_LICENSE_LIMIT = 4018, //License���� 503
    STATUS_PASSWORD_WRONG = 4019, //������� 403

    //�����ĸ�״̬��˳�����Ҫ�� ��Ϊ isValidTransition������Ҫ�ο�˳��
    STATUS_INVITING = 4020,    //the resource is initializing a call
    STATUS_RINGING = 4021,      //the resource is ringing
    STATUS_TXMTING = 4022,     //the resource is talking/transimitting in a call
    STATUS_TXMTIDLE = 4023,    //the resource is idle

    STATUS_DGNAOK = 4024,       //dynamic regrouping OK
    STATUS_DGNAFAIL = 4025,      //dynamic regrouping fail
    STATUS_DGNA_UEFAIL = 4026,   //dynamic regrouping one UE fail
    STATUS_GROUP_ACTIVATED = 4027,
    STATUS_GROUP_DEACTIVATED = 4028,
    STATUS_MUTE = 4029,        //sound mute
    STATUS_UNMUTE = 4030,       //sound unmute
    STATUS_GRPPATCH_CREATEOK = 4031,  //�ɽ��鴴��OK
    STATUS_GRPPATCH_CREATEFAIL = 4032, //�ɽ��鴴��ʧ��
    STATUS_GRPPATCH_CANCELOK = 4033, //�ɽ���ȡ��OK
    STATUS_GRPPATCH_CANCELFAIL = 4034, //�ɽ���ȡ��ʧ��
    STATUS_GRPPATCH_ADDOK = 4035, //�ɽ�����ӳ�ԱOK
    STATUS_GRPPATCH_ADDFAIL = 4036, //�ɽ�����ӳ�ԱFAIL
    STATUS_GRPPATCH_DELOK = 4037, //�ɽ���ɾ����ԱOK
    STATUS_GRPPATCH_DELFAIL = 4038,  //�ɽ���ɾ����ԱFAIL

    STATUS_ACCOUNT_LOCKED = 4060 // �˺ű�����

} res_status_value_t;

typedef enum {
	//��ѯ
	SEQ_CONNECT_REQUEST,         // ��������
	SEQ_LOGIN,                   // �û���¼
	SEQ_LOGOUT,                  // �û��ǳ�



	SEQ_GETDCUSERS = 25,         // ��ѯ����̨�����û�
	SEQ_GETDCGROUPS,             // ��ѯ����̨������
	SEQ_GETGROUPUSERS,           // ��ѯ�����û�
	SEQ_GETUSERINFO,             // ��ѯ�û���Ϣ
	SEQ_GETDCINFO,               // ��ѯ����̨��Ϣ
	SEQ_GETGROUPINFO,            // ��ѯ����Ϣ



	SEQ_STARTREALPLAY = 50,      // ��ʼ��Ƶ�ش�
	SEQ_STOPREALPLAY,           // ֹͣ��Ƶ�ش�
	SEQ_PTZCONTROL,              // ��̨����









} excute_seq_value_t;

// TODO: reference additional headers your program requires here
