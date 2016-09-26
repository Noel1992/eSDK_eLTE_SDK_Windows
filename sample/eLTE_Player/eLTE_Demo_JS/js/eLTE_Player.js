var ocx;
var isLoad = false;
var isLogin = false;

function SetLogPath()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetLogPath($("#logPath").val());
	}
}

function SetLogLevel()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetLogLevel($("#logLevel").val());
	}
}

function SetLogoPath()
{
	if (ocx)
	{
		ocx.ELTE_OCX_UploadLogo($("#logoPath").val());
	}
}

function GetVersion()
{
	if (ocx)
	{
		var version;
		var strResult = ocx.ELTE_OCX_GetVersion(1);

		var xmlDoc = $.parseXML(strResult);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("Version").text();
		$("#version").val(result);
	}
}

function Load()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetBypassBuildMedia(0);
		
		SetLogPath();
		SetLogLevel();
		var logoPath=document.getElementById('logoPath');
		if(logoPath.value!="")
		{
			SetLogoPath();
		}
		
		var resultXml = ocx.ELTE_OCX_Load(1);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_Load:" +result);
		
		isLoad = true;
	}
}

function Exit()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_UnLoad();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_UnLoad:" +result);
		
		isLoad = false;
	}
}

function Login()
{
	if (ocx)
	{
		var userName = $("#userName").val();
		var password = $("#password").val();
		var serverIP = $("#serverIP").val();
		var localIP = $("#localIP").val();
		var sipPort = $("#sipPort").val();
		
		var resultXml = ocx.ELTE_OCX_Login(userName, password, serverIP, localIP, sipPort);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_Login:" +result);
		
		isLogin = true;
	}
}

function Logout()
{
	if (ocx)
	{
		var userName = $("#userName").val();
		var resultXml = ocx.ELTE_OCX_Logout(userName);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_Logout:" +result);
	
		isLogin = false;
	}
}

function TriggerStatusReport()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_TriggerStatusReport($("#triggerStatusReportSelect").val());
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_TriggerStatusReport:" +result);
	}
}

function GetDcUsers()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetDcUsers($("#userName").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetDcUsers:" +result);
	}
}

function GetGroupUsers()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetGroupUsers($("#groupIdForQuery").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetGroupUsers:" +result);
	}
}

function GetGroupInfo()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetGroupInfo($("#groupIdForQuery").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetGroupInfo:" +result);
	}
}

function GetDCInfo()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetDcInfo($("#userName").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetDcInfo:" +result);
	}
}

function GetUserInfo()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetUserInfo($("#userIdForQuery").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetUserInfo:" +result);
	}
}

function GetDcGroups()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetDcGroups($("#userName").val());
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetDcGroups:" +result);
		$("#resultcode").val(resultXml);
	}
}

function StartRealPlay()
{
	if (ocx)
	{
		var resId = $("#resourceId").val();
		var videoFormat = $("#videoFormat").val();
		var cameraType = $("#cameraType").val();
		var userConfirm = $("#userConfirm").val();
		var muteType = $("#muteType").val();
		
		var videoParam = "<Content>";
		videoParam +=    "<VideoParam>";
		videoParam +=    "<VideoFormat>";
		videoParam +=    videoFormat;
		videoParam +=    "</VideoFormat>";
		videoParam +=    "<CameraType>";
		videoParam +=    cameraType;
		videoParam +=    "</CameraType>";
		videoParam +=    "<UserConfirmType>";
		videoParam +=    userConfirm;
		videoParam +=    "</UserConfirmType>";
		videoParam +=    "<MuteType>";
		videoParam +=    muteType;
		videoParam +=    "</MuteType>";
		videoParam +=    "</VideoParam>";
		videoParam +=    "</Content>";
		
		var resultXml = ocx.ELTE_OCX_StartRealPlay(resId, videoParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StartRealPlay:" +result);
	}
}

function StartVideoDispatch()
{
	if(ocx)
	{
		var dstviewerlist = $("#dstviewerlist").val();
		var dispatchParam = "<Content>";
		dispatchParam +=    "<Fmtvalue>";
		dispatchParam +=    $("#videoFormat").val();
		dispatchParam +=    "</Fmtvalue>";
		dispatchParam +=    "<DispatchNum>";
		dispatchParam +=    $("#resourceId").val();
		dispatchParam +=    "</DispatchNum>";
		dispatchParam +=    "<Dstviewerlist>";
		//dispatchParam +=    ;
		//
		
		var strs= new Array(); 
		strDstviewer=dstviewerlist.split(","); 
		for (i=0;i<strDstviewer.length;i++ ) 
		{
			dispatchParam +=    "<Dstviewer>";
			dispatchParam +=    strDstviewer[i];
			dispatchParam +=    "</Dstviewer>";
		}
		dispatchParam +=    "</Dstviewerlist>";
		dispatchParam +=    "<Channel>";
		dispatchParam +=    "</Channel>"
		dispatchParam +=    "</Content>";
		
		var resId = $("#resourceId").val();
		var resultXml = ocx.ELTE_OCX_StartVideoDispatch(resId, dispatchParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StartVideoDispatch:" +result);
	}
}

function StopVideoDispatch()
{
	if(ocx)
	{
		var dispatchParam = "<Content>";
		dispatchParam +=    "<UserId>";
		dispatchParam +=    $("#stopDispatchResId").val();
		dispatchParam +=    "</UserId>";
		dispatchParam +=    "</Content>";
		
		var resId = $("#resourceId").val();
		var resultXml = ocx.ELTE_OCX_StopVideoDispatch(resId, dispatchParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StopVideoDispatch:" +result);
	}
}

function sleep(d){
  
  var t = Data.now();
  
  while(1)
  {
	if(Date.now() - t > d)
	{break;}
  }
}

function RecvVideoPlay()
{
	if(ocx)
	{
		var resId = $("#resourceId").val();
		var resultXml = ocx.ELTE_OCX_RecvVideoPlay(resId, $("#muteType").val());
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_RecvVideoPlay:" +result);
		
		//为了与界面按钮状态保持一致这里需要静音
		//var param = "<Content><MuteParam><CallType>2</CallType></MuteParam></Content>";
		
		//sleep(2000);
		
		//为了与界面按钮状态保持一致这里需要静音
		//resultXml = ocx.ELTE_OCX_VolMute(resId, param);
		
		//PlaySound();
		setTimeout('StopSound()', 10);
	}
}

function StopRealPlay()
{
	if (ocx)
	{
		var resId = $("#resourceId").val();
		
		var resultXml = ocx.ELTE_OCX_StopRealPlay(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StopRealPlay:" +result);
	}
}

function StopCurrentRealPlay(resId)
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_StopRealPlay(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StopRealPlay:" +result);
	}
}

function GISSubscribe()
{
	if (ocx)
	{
		var resId = $("#gisid").val();
		
		var strGisParam = "<Content>";
		strGisParam +=    "<GISParam>";
		strGisParam +=    "<SubType>";
		strGisParam +=    "7";
		strGisParam +=    "</SubType>";
		strGisParam +=    "<ResourceList>";
		strGisParam +=    resId;
		strGisParam +=    "</ResourceList>";
		strGisParam +=    "<Subscriber>";
		strGisParam +=    "</Subscriber>";
		strGisParam +=    "</GISParam>";
		strGisParam +=    "</Content>";
		
		var resultXml = ocx.ELTE_OCX_GISSubscribe("0", strGisParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GISSubscribe:" +result);
	}
}

function GISUnSubscribe()
{
	if (ocx)
	{
		var resId = $("#gisid").val();
		
		var strGisParam = "<Content>";
		strGisParam +=    "<GISParam>";
		strGisParam +=    "<SubType>";
		strGisParam +=    "8";
		strGisParam +=    "</SubType>";
		strGisParam +=    "<ResourceList>";
		strGisParam +=    resId;
		strGisParam +=    "</ResourceList>";
		strGisParam +=    "<Subscriber>";
		strGisParam +=    "</Subscriber>";
		strGisParam +=    "</GISParam>";
		strGisParam +=    "</Content>";
		
		var resultXml = ocx.ELTE_OCX_GISSubscribe("0", strGisParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GISSubscribe:" +result);
	}
}

function CreateDGNA()
{
	if(ocx)
	{
		var strDGNAParam = "<Content>";
		strDGNAParam += "<GroupID>";
		strDGNAParam += $("#DgroupId").val();
		strDGNAParam += "</GroupID>";
		strDGNAParam += "<DcID>";
		strDGNAParam += $("#userName").val();
		strDGNAParam += "</DcID>";
		strDGNAParam += "<Alias>";
		strDGNAParam += $("#alias").val();
		strDGNAParam += "</Alias>";
		strDGNAParam += "<Priority>";
		strDGNAParam += $("#priority").val();
		strDGNAParam += "</Priority>";
		strDGNAParam += "<MaxPeriod>";
		strDGNAParam += $("#maxPeriod").val();
		strDGNAParam += "</MaxPeriod>";
		strDGNAParam += "<GroupList>";
		
		var strGroupList= new Array(); 
		var groupList = $("#groupList").val();
		strGroupList=groupList.split(";"); 
		for (i=0;i<8 && i < strGroupList.length;i++ ) 
		{
			strDGNAParam +=    "<GroupID>";
			strDGNAParam +=    strGroupList[i];
			strDGNAParam +=    "</GroupID>";
		}
		strDGNAParam += "</GroupList>";
		
		strDGNAParam += "<UserList>";
		groupList = $("#userList").val();
		strGroupList=groupList.split(";"); 
		for (i=0;i<8 && i < strGroupList.length;i++ ) 
		{
			strDGNAParam +=    "<UserID>";
			strDGNAParam +=    strGroupList[i];
			strDGNAParam +=    "</UserID>";
		}
		strDGNAParam += "</UserList>";
		strDGNAParam += "</Content>";
		
		var resultXml = ocx.ELTE_OCX_CreateDynamicGroup(strDGNAParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_CreateDynamicGroup:" +result);
	}
}

function CreateTEMPGRP()
{
	if(ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetNextTempGroupID();
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		groupId = xmlDoc.find("NextTempGroupID").text();
		$("#DgroupId").val(groupId);
		
		var strDGNAParam = "<Content>";
		strDGNAParam += "<GroupID>";
		strDGNAParam += groupId;
		strDGNAParam += "</GroupID>";
		strDGNAParam += "<DcID>";
		strDGNAParam += $("#userName").val();
		strDGNAParam += "</DcID>";
		strDGNAParam += "<Alias>";
		strDGNAParam += $("#alias").val();
		strDGNAParam += "</Alias>";
		strDGNAParam += "<Priority>";
		//strDGNAParam += $("#priority").val();
		strDGNAParam += "</Priority>";
		strDGNAParam += "<MaxPeriod>";
		//strDGNAParam += $("#maxPeriod").val();
		strDGNAParam += "</MaxPeriod>";
		strDGNAParam += "<GroupList>";
		
		var strGroupList= new Array(); 
		var groupList = $("#groupList").val();
		strGroupList=groupList.split(";"); 
		for (i=0;i<8;i++ ) 
		{
			strDGNAParam +=    "<GroupID>";
			strDGNAParam +=    strGroupList[i];
			strDGNAParam +=    "</GroupID>";
		}
		strDGNAParam += "</GroupList>";
		
		strDGNAParam += "<UserList>";
		groupList = $("#userList").val();
		strGroupList=groupList.split(";"); 
		for (i=0;i<8;i++ ) 
		{
			strDGNAParam +=    "<UserID>";
			strDGNAParam +=    strGroupList[i];
			strDGNAParam +=    "</UserID>";
		}
		strDGNAParam += "</UserList>";
		strDGNAParam += "</Content>";
		
		resultXml = ocx.ELTE_OCX_CreateTempGroup(strDGNAParam);
		
		xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_CreateTempGroup:" +result);
	}
}

function CancelDGNA()
{
	if(ocx)
	{
		var resultXml = ocx.ELTE_OCX_CancelDynamicGroup($("#DgroupId").val());
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_CancelDynamicGroup:" +result);
	}
}

function SDSSendMessage()
{
	if(ocx)
	{
		var strSDSParam = "<Content>";
		strSDSParam +=    "<SDSType>";
		var attachMent=document.getElementById('attachMent');
		if(attachMent.value!="")
		{
			strSDSParam +=    "0004";
		}
		else
		{
			strSDSParam +=    "0001";
		}
		strSDSParam +=    "</SDSType>";
		strSDSParam +=    "<MsgBody>";
		strSDSParam +=    $("#content").val()
		strSDSParam +=    "</MsgBody>";
		strSDSParam +=    "<Receiver>";
		strSDSParam +=    $("#receiver").val()
		strSDSParam +=    "</Receiver>";
		strSDSParam +=    "<AttachFileList>";
		if(attachMent.value!="")
		{
			strSDSParam += "<AttachFile>";
			strSDSParam += attachMent.value;
			strSDSParam += "</AttachFile>";
		}
		strSDSParam +=    "</AttachFileList>";
		strSDSParam +=    "</Content>";
		
		var resultXml = ocx.ELTE_OCX_SDSSendMessage($("#userName").val(), strSDSParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_SDSSendMessage:" +result);
	}
}

function ReverseRealPlay()
{
	if (ocx)
	{
		var resId = $("#resourceId").val();
		var videoFormat = $("#videoFormat").val();
		var cameraType = $("#cameraType").val();
		var userConfirm = $("#userConfirm").val();
		var muteType = $("#muteType").val();
		
		if (cameraType == "0")
		{
			cameraType = "1";
		}
		else
		{
			cameraType = "0";
		}
		
		var videoParam = "<Content>";
		videoParam +=    "<VideoParam>";
		videoParam +=    "<VideoFormat>";
		videoParam +=    videoFormat;
		videoParam +=    "</VideoFormat>";
		videoParam +=    "<CameraType>";
		videoParam +=    cameraType;
		videoParam +=    "</CameraType>";
		videoParam +=    "<UserConfirmType>";
		videoParam +=    userConfirm;
		videoParam +=    "</UserConfirmType>";
		videoParam +=    "<MuteType>";
		videoParam +=    muteType;
		videoParam +=    "</MuteType>";
		videoParam +=    "</VideoParam>";
		videoParam +=    "</Content>";
		
		var resultXml = ocx.ELTE_OCX_ReverseRealPlay(resId, videoParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_ReverseRealPlay:" +result);
		//PlaySound();
	}
}

function ShowRealPlay()
{
	if (ocx)
	{		
		var resId = $("#resourceId").val();
		var localVideoPort = $("#localVideoPort").val();
		var localAudioPort = $("#localAudioPort").val();
		var remoteVideoPort = $("#remoteVideoPort").val();
		var remoteAudioPort = $("#remoteAudioPort").val();
		var localIP = $("#localIP").val();
		var serverIP = $("#serverIP").val();
		
		var localParam = "<Content>";
		localParam    += "<LocalMediaAddr>";
		localParam    += "<LocalIP>";
		localParam    += localIP;
		localParam    += "</LocalIP>";
		localParam    += "<VideoPort>";
		localParam    += localVideoPort;
		localParam    += "</VideoPort>";
		localParam    += "<AudioPort>";
		localParam    += localAudioPort;
		localParam    += "</AudioPort>";
		localParam    += "</LocalMediaAddr>";
		localParam    += "</Content>";
		
		var remoteParam = "<Content>";
		remoteParam    += "<RemoteMediaAddr>";
		remoteParam    += "<RemoteIP>";
		remoteParam    += serverIP;
		remoteParam    += "</RemoteIP>";
		remoteParam    += "<VideoPort>";
		remoteParam    += remoteVideoPort;
		remoteParam    += "</VideoPort>";
		remoteParam    += "<AudioPort>";
		remoteParam    += remoteAudioPort;
		remoteParam    += "</AudioPort>";
		remoteParam    += "</RemoteMediaAddr>";
		remoteParam    += "</Content>";
		
		var resultXml = ocx.ELTE_OCX_ShowRealPlay(resId, localParam, remoteParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_ShowRealPlay:" +result);
		
		SetVideoPos();
		
		//resultXml = ocx.ELTE_OCX_VolUnMute(resId, param);
		//PlaySound();
	}
}

function HideRealPlay()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_HideRealPlay();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_HideRealPlay:" +result);
	}
}

function Snapshot()
{
	if (ocx)
	{
		var resId = $("#resourceId").val();
		
		var resultXml = ocx.ELTE_OCX_Snapshot(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_Snapshot:" +result);
	}
}

function PlaySound()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_PlaySound();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_PlaySound:" +result);
	}
}

function StopSound()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_StopSound();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StopSound:" +result);
	}
}

function FullScreen()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_FullScreenDisplay();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_FullScreenDisplay:" +result);
	}
}

function NormalScreen()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_NormalScreenDisplay();
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_NormalScreenDisplay:" +result);
	}
}

function SetVideoPos()
{
	if (ocx)
	{
		var left = $("#left").val();
		var top = $("#top").val();
		var width = $("#width").val();
		var height = $("#height").val();
		
		var resultXml = ocx.ELTE_OCX_SetVideoWindowPos(left, top, width, height);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_SetVideoWindowPos:" +result);
	}
}

function SetTitleText()
{
	if (ocx)
	{
		var titleText = $("#titleText").val();
		
		var resultXml = ocx.ELTE_OCX_SetTitleText(titleText);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_SetTitleText:" +result);
	}
}

function ShowToolBar()
{
	if (ocx)
	{
		var select = $("#toolBar").val();
		
		var resultXml = ocx.ELTE_OCX_ShowToolbar(select);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_ShowToolbar:" +result);
	}
}

function SetLang()
{
	if (ocx)
	{
		var select = $("#language").val();
		
		var resultXml = ocx.ELTE_OCX_SetLanguage(select);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_SetLanguage:" +result);
	}
}

function P2PDial()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2PDial(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2PDial:" +result);
	}
}

function P2PReceive()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2PRecv(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2PRecv:" +result);
	}
}

function P2PReject()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2PReject(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2PReject:" +result);
	}
}

function P2PHangup()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2PHangup(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2PHangup:" +result);
	}
}

function P2PBreakin()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2Pbreakin(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2Pbreakin:" +result);
	}
}

function P2PBreakoff()
{
	if (ocx)
	{
		var resId = $("#p2pResId").val();
		
		var resultXml = ocx.ELTE_OCX_P2Pbreakoff(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2Pbreakoff:" +result);
	}
}

function UnSubscribeGroup()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_UnSubscribeGroup(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_UnSubscribeGroup:" +result);
	}
}

function Environmentlisten() {
	if	(ocx) {
		var resId = $("#p2pResId").val();

		var resultXml = ocx.ELTE_OCX_StartEnvironmentListen(resId);

		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StartEnvironmentListen:" + result);
	}
}

function P2PMute()
{
	if (ocx)
	{
		
		var param = "<Content><MuteParam><CallType>0</CallType></MuteParam></Content>";
		
		var resultXml = ocx.ELTE_OCX_VolMute($("#p2pResId").val(), param);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_VolMute:" +result);
	}
}

function P2PUnMute()
{
	if (ocx)
	{
		
		var param = "<Content><MuteParam><CallType>0</CallType></MuteParam></Content>";
		
		var resultXml = ocx.ELTE_OCX_VolUnMute($("#p2pResId").val(), param);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_VolUnMute:" +result);
	}
}


function GetUserRECFileInfoList()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_ProvisionManagerInitMRS($("#serverIP").val());
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_ProvisionManagerInitMRS:" +result);
		
		
		var grpId = $("#queryUserRECFileInfoListXml").val();
		
		resultXml = ocx.ELTE_OCX_GetUserRECFileInfoList(grpId);
		$("#textArea").val(resultXml);
		xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetUserRECFileInfoList:" +result);
	}
}

function SubJoinGroup()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_SubJoinGroup(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_SubJoinGroup:" +result);
	}
}

function PTTMute()
{
	if (ocx)
	{
		var param = "<Content><MuteParam><CallType>1</CallType></MuteParam></Content>";
		
		var resultXml = ocx.ELTE_OCX_VolMute($("#groupId").val(), param);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("PTT ELTE_OCX_VolMute:" +result);
	}
}

function PTTUnMute()
{
	if (ocx)
	{
		var param = "<Content><MuteParam><CallType>1</CallType></MuteParam></Content>";
		
		var resultXml = ocx.ELTE_OCX_VolUnMute($("#groupId").val(), param);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("PTT ELTE_OCX_VolUnMute:" +result);
	}
}

function PTTDial()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_PTTDial(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_PTTDial:" +result);
	}
}

function PTTRelease()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_PTTRelease(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_PTTRelease:" +result);
	}
}

function PTTHangup()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_PTTHangup(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_PTTHangup:" +result);
	}
}

function PTTEmergency()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_PTTEmergency(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_PTTEmergency:" +result);
	}
}

function GroupBreakoff()
{
	if (ocx)
	{
		var grpId = $("#groupId").val();
		
		var resultXml = ocx.ELTE_OCX_GroupBreakoff(grpId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GroupBreakoff:" +result);
	}
}

//C60
function Startdiscreetlisten()
{
	if	(ocx)
	{
		var resId = $("#resourceId").val();
		
		var resultXml = ocx.ELTE_OCX_StartDiscreetListen(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StartDiscreetListen:" + result);
	}
}

function Stopdiscreetlisten()
{
	if	(ocx)
	{
		var resId = $("#resourceId").val();
		
		var resultXml = ocx.ELTE_OCX_StopDiscreetListen(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_StopDiscreetListen:" + result);
	}
}

function GetVWallIDlist()
{
	if	(ocx)
	{
		var resId = $("#resourceId").val();
		
		var resultXml = ocx.ELTE_OCX_GetDcVWallIDList();
		
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetDcVWallIDList:" + result);
	}
}

function VWallstart()
{
	if	(ocx)
	{
		var resId = $("#resourceId").val();
		var VWallID = $("#VWallID").val();
		var resParam = "<Content>";		
		resParam += "<ResourceID>";
		resParam += resId;
		resParam += "</ResourceID>";
		resParam += "<VideoParam>";
		resParam += "<DstObjId>";
		resParam += VWallID;
		resParam += "</DstObjId>";
		resParam += "<SrcObjId>";
		resParam += resId;
		resParam += "</SrcObjId>";
		resParam += "<StrFmt>";
		resParam += "</StrFmt>";
		resParam += "</VideoParam>";
		resParam += "</Content>";	
		
		var resultXml = ocx.ELTE_OCX_VWallStart(resId,resParam);

		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_VWallStart:" + result);
	}
}

function VWallstop()
{
	if	(ocx)
	{
		var resId = $("#resourceId").val();
		var VWallID = $("#VWallID").val();
		var resParam = "<Content>";	
		resParam += "<DstObjId>";
		resParam += VWallID;
		resParam += "</DstObjId>";
		resParam += "<SrcObjId>";
		resParam += resId;
		resParam += "</SrcObjId>";
		resParam += "</Content>";	
		
		var resultXml = ocx.ELTE_OCX_VWallStop(resId,resParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_VWallStop:" + result);
	}
}

function ELTE_OCX_Event(ulEventType,pEventDataXml)
{
	$("#eventType").val("ELTE_OCX_Event:" +ulEventType);
}

function ELTE_OCX_PlayerEvent(ulEventType,pEventDataXml)
{
	$("#eventType").val("ELTE_OCX_PlayerEvent:" +ulEventType);
}

function Init()
{
	ocx = document.getElementById("eLTE_PlayerOCX");
	
	GetVersion();
}

function Uninit()
{
	if (isLogin)
	{
		Logout();
	}
	
	if (isLoad)
	{
		Exit();
	}
}