/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include "StdAfx.h"
#include "XMLProcess.h"
#include "eLTE_Log.h"
#include "eLTE_Tool.h"
#include "eLTE_Xml.h"

XMLProcess::XMLProcess(void)
{
}

XMLProcess::~XMLProcess(void)
{
}

// �������Mute_UL_parameter
eLTE_RET XMLProcess::XmlParseMuteULParameter(const std::string& xmlStr,	// ���
		std::string& strCallType)			// ����
{
	/************************************************************************
	--- XML ��ʽ ---
	<Content>
		<MuteParam>
			<CallType>"0"�����"1"�����"2"��Ƶ�ش�</CallType>
		</MuteParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("MuteParam"))
	{
		LOG_RUN_ERROR("FindElem MuteParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CallType", srcValue, elemValue, uiMaxLen);
	strCallType = elemValue;

	return eLTE_ERR_SUCCESS;
}


// �������Video_UL_parameter
eLTE_RET XMLProcess::XmlParseVideoULParameter(const std::string& xmlStr,	// ���
		std::string& strVideoFormat, std::string& strCameraType,			// ����
		std::string& strUserConfirmType, std::string& strMuteType)			// ����
{
	/************************************************************************
	--- XML ��ʽ ---
	<Content>
		<VideoParam>
			<VideoFormat>D1��CIF��QCIF��720P��1080P</VideoFormat>
			<CameraType>��0����ʾǰ�ã���1����ʾ���á�</CameraType>
			<UserConfirmType>��0����ʾ����Ҫ�û�ȷ�ϣ���1����ʾ��Ҫ�û�ȷ�ϡ�</UserConfirmType>
			<MuteType>��0����ʾ��Ҫ��������1����ʾ����Ҫ����</MuteType>
		</VideoParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("VideoParam"))
	{
		LOG_RUN_ERROR("FindElem VideoParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "VideoFormat", srcValue, elemValue, uiMaxLen);
	strVideoFormat = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "CameraType", srcValue, elemValue, uiMaxLen);
	strCameraType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "UserConfirmType", srcValue, elemValue, uiMaxLen);
	strUserConfirmType = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "MuteType", srcValue, elemValue, uiMaxLen);
	strMuteType = elemValue;

	return eLTE_ERR_SUCCESS;
}

// ���������Ƶ���Ų���
eLTE_RET XMLProcess::XmlParsePlayVideoParam(const std::string& xmlStrLocal, const std::string& xmlStrRemote,	// ���
		std::string& strLocalIP, unsigned int& uiLocalVideoPort, unsigned int& uiLocalAudioPort,				// ����
		std::string& strRemoteIP, unsigned int& uiRemoteVideoPort, unsigned int& uiRemoteAudioPort)				// ����
{
	/************************************************************************
	--- XML ��ʽ ---
	<Content>
		<LocalMediaAddr>
			<LocalIP></LocalIP>
			<VideoPort></VideoPort>
			<AudioPort></AudioPort>
		</LocalMediaAddr>
	</Content>
	<Content>
		<RemoteMediaAddr>
			<RemoteIP></RemoteIP>
			<VideoPort></VideoPort>
			<AudioPort></AudioPort>
		</RemoteMediaAddr>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM2(xmlStrLocal, xmlStrRemote);

	// ����������Ƶ����
	CXml xmlLocal;
	if (!xmlLocal.Parse(xmlStrLocal.c_str()))
	{
		LOG_RUN_ERROR("parse xmlStrLocal failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlLocal.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlLocal.IntoElem();
	if (!xmlLocal.FindElem("LocalMediaAddr"))
	{
		LOG_RUN_ERROR("FindElem LocalMediaAddr failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlLocal.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "LocalIP", srcValue, elemValue, uiMaxLen);
	strLocalIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "VideoPort", srcValue, elemValue, uiMaxLen);
	uiLocalVideoPort = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlLocal, "AudioPort", srcValue, elemValue, uiMaxLen);
	uiLocalAudioPort = eLTE_Tool::String2UInt(elemValue);

	// ����Զ����Ƶ����
	CXml xmlRemote;
	if (!xmlRemote.Parse(xmlStrRemote.c_str()))
	{
		LOG_RUN_ERROR("parse xmlStrRemote failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlRemote.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlRemote.IntoElem();
	if (!xmlRemote.FindElem("RemoteMediaAddr"))
	{
		LOG_RUN_ERROR("FindElem RemoteMediaAddr failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlRemote.IntoElem();

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "RemoteIP", srcValue, elemValue, uiMaxLen);
	strRemoteIP = elemValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "VideoPort", srcValue, elemValue, uiMaxLen);
	uiRemoteVideoPort = eLTE_Tool::String2UInt(elemValue);

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);
	GET_XML_ELEM_VALUE_CHAR(xmlRemote, "AudioPort", srcValue, elemValue, uiMaxLen);
	uiRemoteAudioPort = eLTE_Tool::String2UInt(elemValue);

	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringPlayerEvent(int iEventId, const PlayerEventParamInfo* pInfo,
	CString& xmlStr)
{
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;

	switch (iEventId)
	{
	case eLTE_PLAYER_REVERSE:		// OCX �������л���ͷ
		{
			/************************************************************************
			--- XML ��ʽ ---
			<Content>
				<VideoFormat></VideoFormat>
				<CameraType></CameraType>
				<UserConfirmType></UserConfirmType>
				<MuteType></MuteType>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("VideoFormat");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(pInfo->strFmtValue.c_str());
			(void)xml.AddElem("CameraType");
			(void)xml.SetElemValue(pInfo->strCameraType.c_str());
			(void)xml.AddElem("UserConfirmType");
			(void)xml.SetElemValue(pInfo->strUserCfmType.c_str());
			(void)xml.AddElem("MuteType");
			(void)xml.SetElemValue(pInfo->strMuteType.c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_SNAPSHOT:		// OCX ������ץͼ
		{
			/************************************************************************
			--- XML ��ʽ ---
			<Content>
				<SnapshotPath></SnapshotPath>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("SnapshotPath");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(pInfo->strSnapshotPath.c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_POSITION:		// OCX ����������䶯
		{
			/************************************************************************
			--- XML ��ʽ ---
			<Content>
				<Left></Left>
				<Top></Top>
				<Width></Width>
				<Height></Height>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("Left");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iLeft).c_str());
			(void)xml.AddElem("Top");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iTop).c_str());
			(void)xml.AddElem("Width");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iWidth).c_str());
			(void)xml.AddElem("Height");
			(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iHeight).c_str());
			xml.OutOfElem();
		}
		break;
	case eLTE_PLAYER_TITLE:			// OCX ���������ڱ���
		{
			/************************************************************************
			--- XML ��ʽ ---
			<Content>
				<TitleText></TitleText>
			</Content>
			************************************************************************/

			(void)xml.AddElem("Content");
			(void)xml.AddChildElem("TitleText");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(pInfo->strTitle.c_str());
			xml.OutOfElem();
		}
		break;
	default:
		{
			LOG_RUN_ERROR("Invalid eventId.");
			return eLTE_ERR_FAILED;
		}
	}

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	//INFO_PARAM1(xmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyGisReport(const GisReportInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML ��ʽ ---
			<Content>
			<ResourceID>�ϱ���</ResourceID>
			<Time>���һ���ϱ�ʱ��</Time>
			<Altitude>�߶�</Altitude>
			<Latitude>γ��</Latitude>
			<Longtitude>����</Longtitude>
			<TriggerCode>�����ϱ�(0)���ض��¼���1���������У�</TriggerCode>
			<ReportStatus>GPS��������(0)��GPS����ʧ��(1)�������쳣(2)</ReportStatus>
			</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSender.c_str());
	(void)xml.AddElem("Time");
	(void)xml.SetElemValue(pInfo->strTime.c_str());
	(void)xml.AddElem("Altitude");
	(void)xml.SetElemValue(pInfo->strAltitude.c_str());
	(void)xml.AddElem("Latitude");
	(void)xml.SetElemValue(pInfo->strLatitude.c_str());
	(void)xml.AddElem("Longtitude");
	(void)xml.SetElemValue(pInfo->strLongtitude.c_str());
	(void)xml.AddElem("TriggerCode");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iTriggerCode).c_str());
	(void)xml.AddElem("ReportStatus");
	(void)xml.SetElemValue(eLTE_Tool::Int2String(pInfo->iReportState).c_str());
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifySDSStatus(const SdsMessageInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML ��ʽ ---
			<Content>
			<SdsMsgId>��ϢID</SdsMsgId>
			<SdsRetCode>��Ϣ����״̬</SdsRetCode>
			</Content>
	************************************************************************/
	LOG_TRACE();
	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("SdsMsgId");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSubject.c_str());

	(void)xml.AddElem("SdsRetCode");
	(void)xml.SetElemValue(pInfo->strRetCode.c_str());

	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifySDSReport(const std::string& strSrc,
	CString& xmlStr)
{
	/************************************************************************
			--- XML ��ʽ ---
			<Content>
			<SdsType>��Ϣ����</SdsType>
			<SdsContent>��Ϣ����</SdsContent>
			<SdsFrom>������</SdsFrom>
			<SdsMmsFileNameList>
				<SdsMmsFileName>�����ļ����б�</SdsMmsFileName>
				<SdsMmsFileName>�����ļ����б�</SdsMmsFileName>
			</SdsMmsFileNameList>
			<SdsSubject>��Ϣ����</SdsSubject>
			<SdsDate>��Ϣ��������</SdsDate>
			<SdsTime>��Ϣ����ʱ��</SdsTime>
			</Content>
	************************************************************************/
	LOG_TRACE();

	CXml xmlParse;

	if (!xmlParse.Parse(strSrc.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("SdsType"))
	{
		LOG_RUN_ERROR("FindElem CallStatus failed.");
		return eLTE_ERR_FAILED;
	}
	std::string strSdsType = xmlParse.GetElemValue();

	std::string strSdsContent = "";
	if(xmlParse.FindElem("SdsContent"))
	{
		strSdsContent = xmlParse.GetElemValue();
	}

	if (!xmlParse.FindElem("SdsFrom"))
	{
		LOG_RUN_ERROR("FindElem SdsFrom failed.");
		return eLTE_ERR_FAILED;
	}
	std::string strSdsFrom = xmlParse.GetElemValue();

	std::list<std::string> fileNameList;
	//����
	if (xmlParse.FindElem("SdsMmsFileNameList") && xmlParse.IntoElem())
	{
		do 
		{																		
			std::string srcValue = xmlParse.GetElemValue();
			if(!srcValue.empty())
			{
				fileNameList.push_back(srcValue);
			}
		} while (xmlParse.NextElem());
		xmlParse.OutOfElem();
	}

	if (!xmlParse.FindElem("SdsSubject"))
	{
		LOG_RUN_ERROR("FindElem SdsSubject failed.");
		return eLTE_ERR_FAILED;
	}
	std::string strSdsSubject = xmlParse.GetElemValue();


	if (!xmlParse.FindElem("SdsDate"))
	{
		LOG_RUN_ERROR("FindElem SdsDate failed.");
		return eLTE_ERR_FAILED;
	}
	std::string strSdsDate = xmlParse.GetElemValue();

	if (!xmlParse.FindElem("SdsTime"))
	{
		LOG_RUN_ERROR("FindElem SdsTime failed.");
		return eLTE_ERR_FAILED;
	}
	std::string strSdsTime = xmlParse.GetElemValue();


	//ƴ��xml
	xmlStr.Append(_T("<Content>"));

	xmlStr.Append(_T("<SdsType>"));
	xmlStr.Append(eLTE_Tool::ANSIToUnicode(strSdsType).c_str());
	xmlStr.Append(_T("</SdsType>"));

	if(!strSdsContent.empty())
	{
		xmlStr.Append(_T("<SdsContent>"));
		xmlStr.Append(eLTE_Tool::UTF8ToUnicode(strSdsContent).c_str());
		xmlStr.Append(_T("</SdsContent>"));
	}

	xmlStr.Append(_T("<SdsFrom>"));
	xmlStr.Append(eLTE_Tool::ANSIToUnicode(strSdsFrom).c_str());
	xmlStr.Append(_T("</SdsFrom>"));

	if(0 < fileNameList.size())
	{
		xmlStr.Append(_T("<SdsMmsFileNameList>"));
		std::list<std::string>::iterator it = fileNameList.begin();

		for (; it != fileNameList.end(); ++it)
		{
			xmlStr.Append(_T("<SdsMmsFileName>"));
			xmlStr.Append(eLTE_Tool::UTF8ToUnicode(*it).c_str());
			xmlStr.Append(_T("</SdsMmsFileName>"));
		}

		xmlStr.Append(_T("</SdsMmsFileNameList>"));
	}

	xmlStr.Append(_T("<SdsSubject>"));
	xmlStr.Append(eLTE_Tool::UTF8ToUnicode(strSdsSubject).c_str());
	xmlStr.Append(_T("</SdsSubject>"));

	
	xmlStr.Append(_T("<SdsDirection>"));
	xmlStr.Append(_T("false"));
	xmlStr.Append(_T("</SdsDirection>"));

	xmlStr.Append(_T("<SdsDate>"));
	xmlStr.Append(eLTE_Tool::ANSIToUnicode(strSdsDate).c_str());
	xmlStr.Append(_T("</SdsDate>"));

	xmlStr.Append(_T("<SdsTime>"));
	xmlStr.Append(eLTE_Tool::ANSIToUnicode(strSdsTime).c_str());
	xmlStr.Append(_T("</SdsTime>"));
	xmlStr.Append(_T("</Content>"));

	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::SetXmlStringEventNotifyGisStatus(const GisReportInfo* pInfo,
	CString& xmlStr)
{
	/************************************************************************
			--- XML ��ʽ ---
			<Content>
			<ResourceID>�ϱ���</ResourceID>
			<AckStatusList>
			<AckStatus>�ն˺ţ�����״̬���������£�
			1001:0��</AckStatus>
			</AckStatusList>
			</Content>
	************************************************************************/
	LOG_TRACE();

	if (NULL == pInfo)
	{
		LOG_RUN_ERROR("pInfo is null.");
		return eLTE_ERR_FAILED;
	}

	CXml xml;
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResourceID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pInfo->strSender.c_str());
	(void)xml.AddElem("AckStatusList");

	AckStateMap::iterator itor_b = (const_cast<GisReportInfo*>(pInfo))->ackStateMap.begin();
	AckStateMap::iterator itor_e = (const_cast<GisReportInfo*>(pInfo))->ackStateMap.end();
	std::string strAckStatus;
	for(; itor_b != itor_e; ++itor_b)
	{
		if(itor_b == pInfo->ackStateMap.begin())
		{
			(void)xml.AddChildElem("AckStatus");
			(void)xml.IntoElem();
		}
		else
		{
			(void)xml.AddElem("AckStatus");
		}
		strAckStatus.clear();
		strAckStatus = "";
		strAckStatus.append(eLTE_Tool::Int2String(itor_b->first).c_str());
		strAckStatus.append(":");
		strAckStatus.append(eLTE_Tool::Int2String(itor_b->second).c_str());
		(void)xml.SetElemValue(strAckStatus.c_str());
	}
	xml.OutOfElem();
	xml.OutOfElem();

	IVS_UINT32 uiLen = 0;
	const char* pXmlStr = xml.GetXMLStream(uiLen);
	if (NULL == pXmlStr)
	{
		LOG_RUN_ERROR("pXmlStr is null.");
		return eLTE_ERR_FAILED;
	}
	xmlStr = eLTE_Tool::ANSIToUnicode(pXmlStr).c_str();

	INFO_PARAM1(pXmlStr);
	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::XmlParseGISSubscribeParameter(const std::string& xmlStr,     // ���
	std::string& strSubType, std::string& strResourceList, std::string& strSubscriber)   // ����
{
	/************************************************************************
	--- XML ��ʽ ---
	<Content>
		<GISParam>
			<SubType>7/8</SubType>
			<ResourceList>�����ԴID�÷ֺŷָ������200������1001;1002;1003</ResourceList>
			<Subscriber>Ŀǰ�ɲ���</Subscriber>
		</GISParam>
	</Content>
	************************************************************************/ 
	
	LOG_TRACE();
	INFO_PARAM1(xmlStr);

	CXml xmlParse;
	if (!xmlParse.Parse(xmlStr.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("GISParam"))
	{
		LOG_RUN_ERROR("FindElem GISParam failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();

	const unsigned int XML_VAR_LENGTH = 20;
	char elemValue[XML_VAR_LENGTH] = {0};
	const char* srcValue;
	unsigned int uiMaxLen = XML_VAR_LENGTH;

	GET_XML_ELEM_VALUE_CHAR(xmlParse, "SubType", srcValue, elemValue, uiMaxLen);
	strSubType = elemValue;

	if (!xmlParse.FindElem("ResourceList"))													
	{																			
		LOG_RUN_ERROR("FindElem ResourceList failed.");								
		return eLTE_ERR_XML_FIND_ELEM;											
	}																			
	srcValue = xmlParse.GetElemValue();												
	if (NULL == srcValue)														
	{																			
		LOG_RUN_ERROR("GetElemValue ResourceList failed.");							
		return eLTE_ERR_XML_GET_ELEM_VALUE;										
	}
	strResourceList = srcValue;

	uiMaxLen = XML_VAR_LENGTH;
	memset(elemValue, 0, sizeof(char)*XML_VAR_LENGTH);	
	GET_XML_ELEM_VALUE_CHAR(xmlParse, "Subscriber", srcValue, elemValue, uiMaxLen);
	strSubscriber = elemValue;

	return eLTE_ERR_SUCCESS;
}

eLTE_RET XMLProcess::ParseXmlStringEventNotifyP2pvideocallStatus(NotifyP2pvideocallStatusInfo& pInfo, const std::string& strXml)
{
	LOG_TRACE();
	INFO_PARAM1(strXml);

	CXml xmlParse;
	if (!xmlParse.Parse(strXml.c_str()))
	{
		LOG_RUN_ERROR("parse xml failed.");
		return eLTE_ERR_FAILED;
	}
	if (!xmlParse.FindElem("Content"))
	{
		LOG_RUN_ERROR("FindElem Content failed.");
		return eLTE_ERR_FAILED;
	}
	(void)xmlParse.IntoElem();
	if (!xmlParse.FindElem("CallStatus"))
	{
		LOG_RUN_ERROR("FindElem CallStatus failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iCallStatus = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("Callee"))
	{
		LOG_RUN_ERROR("FindElem Callee failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iCallee = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("Caller"))
	{
		LOG_RUN_ERROR("FindElem Caller failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iCaller = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("LocalAudioPort"))
	{
		LOG_RUN_ERROR("FindElem LocalAudioPort failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iLocalAudioPort = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("LocalVideoPort"))
	{
		LOG_RUN_ERROR("FindElem LocalVideoPort failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iLocalVideoPort = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("RemoteAudioPort"))
	{
		LOG_RUN_ERROR("FindElem RemoteAudioPort failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iRemoteAudioPort = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	if (!xmlParse.FindElem("RemoteVideoPort"))
	{
		LOG_RUN_ERROR("FindElem RemoteVideoPort failed.");
		return eLTE_ERR_FAILED;
	}
	pInfo.iRemoteVideoPort = eLTE_Tool::String2Int(xmlParse.GetElemValue());

	return eLTE_ERR_SUCCESS;
}
