var ocx;
var isLoad = false;
var isLogin = false;
var localIP="";
var serverIP="172.22.9.120";
var userName = "4119";	//user name
var password = "4119";	//password
var resourceID = "8895";
var arrayList;

function setResourceID()
{
	curLongtitude = 0;
    curLatitude = 0;
	resourceID = $("#gisid").val();

	//To determine if there is a dictionary of resources in the current resource box
    var newArr = ocxMap.filter(function(item){
            return item.resourceID === $("#gisid").val();
        });

    if (0 == newArr.length) {
    	//create a new array
		ocxMap.push({resourceID:resourceID, LonLatArr:null, Marker:null, ocxName:"", locateFlag:false, ClickEvtHandle:null});
		alert("setResourceID: add resourceID success!");
    }
    else
    {
    	alert("setResourceID: resourceID already exist!");	
    }    
}

function delResourceID()
{
	delmark();
	//Get  current resource box ID, and determines the current index in the dictionary
    var i = findMapByResID($("#gisid").val());
    if ((-1) == i) 
    {
    	alert("resource ID input error!");
    	return;
    }
    //Delete the resource corresponding to the ocx control
    delOcx(ocxMap[i].ocxName);
    //Clear the resource path specified in the current resource mine
    ocxMap[i].LonLatArr = [];
    delete ocxMap[i];
    alert("delResourceID: delete resourceID success!");
}

function SetLogPath()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetLogPath("D:\\eLTE_Player_log");
	}
}

function SetLogLevel()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetLogLevel(0);
	}
}

function SetLogoPath()
{
	if (ocx)
	{
		ocx.ELTE_OCX_UploadLogo("D:\\eLTE_Player_log");
	}
}

function GetVersion()
{
	if (ocx)
	{
		var strResult = ocx.ELTE_OCX_GetVersion(1);
		var xmlDoc = $.parseXML(strResult);
		xmlDoc = $(xmlDoc);
		var version = xmlDoc.find("Version").text();
	}
}

function Load()
{
	if (ocx)
	{
		ocx.ELTE_OCX_SetBypassBuildMedia(0);
		
		SetLogPath();
		SetLogLevel();
		
		var resultXml = ocx.ELTE_OCX_Load(1);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_Load:" +result);

		isLoad = true;
		if ("0" != result) {
			alert("ELTE_OCX_Load failed, error code: "+result);
		}
		
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
		if ("" == localIP) {
			alert("localIP is null! please refresh the page.");
		}

		var resultXml = ocx.ELTE_OCX_Login(userName, password, serverIP, localIP, "5060");
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
		//var userName = userName;
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
		var resultXml = ocx.ELTE_OCX_TriggerStatusReport("1");
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_TriggerStatusReport:" +result);
	}
}

function GetDCInfo()
{
	if (ocx)
	{
		var resultXml = ocx.ELTE_OCX_GetDcInfo(userName);
		$("#textArea").val(resultXml);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_GetDcInfo:" +result);
	}
}

function StartRealPlay(resID)
{
	if (ocx)
	{
		var resId = resID;
		var videoFormat = "D1";
		var cameraType = "0";
		var userConfirm = "0";
		var muteType = "0";
		
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

function sleep(d){
  var Date = new Date()
  var t = Date.now();
  
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
		var resId = resourceID;
		var resultXml = ocx.ELTE_OCX_RecvVideoPlay(resId, "0");
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_RecvVideoPlay:" +result);
		
		setTimeout('StopSound()', 10);
	}
}

function StopRealPlay()
{
	if (ocx)
	{
		var resId = resourceID;
		
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

function ReverseRealPlay()
{
	if (ocx)
	{
		var resId = resourceID;
		var videoFormat = "D1";
		var cameraType = "0";
		var userConfirm = "0";
		var muteType = "0";
		
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

function ShowRealPlay(localVideoPort,localAudioPort,remoteVideoPort,remoteAudioPort)
{
	if (curOcx)
	{		
		
		if ("" == localIP) {
			alert("localIP is null! please refresh the page.");
		}
		
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
		
		var resultXml = curOcx.ELTE_OCX_ShowRealPlay(resourceID, localParam, remoteParam);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_ShowRealPlay:" +result);
		
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
		var resultXml = ocx.ELTE_OCX_Snapshot(resourceID);
		
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

function SetVideoPos(ocxName,left,top)
{
	if (ocxName)
	{
		var width = "500";
		var height = "450";
		var resultXml = ocxName.ELTE_OCX_SetVideoWindowPos(left, top, width, height);
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
		var titleText = "Live Video";
		
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
		var select = "1";
		
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
		var select = "1";
		
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
		var resId = resourceID
		
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
		var resId = resourceID
		
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
		var resId = resourceID
		
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
		var resId = resourceID
		
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
		var resId = resourceID
		
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
		var resId = resourceID
		
		var resultXml = ocx.ELTE_OCX_P2Pbreakoff(resId);
		
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_P2Pbreakoff:" +result);
	}
}

function Environmentlisten() {
	if	(ocx) {
		var resId = resourceID;

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
		
		var resultXml = ocx.ELTE_OCX_VolMute(resourceID, param);
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
		
		var resultXml = ocx.ELTE_OCX_VolUnMute(resourceID, param);
		var xmlDoc = $.parseXML(resultXml);
		xmlDoc = $(xmlDoc);
		var result = xmlDoc.find("ResultCode").text();
		$("#resultcode").val("ELTE_OCX_VolUnMute:" +result);
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
	//Clear all tags
	clearDrivingRoute();
	if (isLogin)
	{
		Logout();
	}
	
	if (isLoad)
	{
		//unload	
		Exit();
	}
}