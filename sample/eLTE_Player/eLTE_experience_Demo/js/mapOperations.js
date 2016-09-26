var map;            //map object 
var drivingRoute;   //Driving route planning object 
var obj;            //search result 
var startLngLat;    //Latitude and longitude of the starting point 
var endLngLat;      //End latitude and longitude 
var startTool;      //Starting point marking tool 
var endTool;    //End point marking tool 
var startIcon = "images/startmarker.png";  //Starting point icon 
var endIcon = "images/endmarker.png";      //Terminal Icon 
var infoWin = null;
var carMk;
var lnglat;
var currentPos; //Current window coordinates
var curLonLat;  //Current latitude and longitude
var myIcon = new TIcon("images/marker.png",new TSize(33,44),{anchor:new TPixel(22,44)});
var curLatitude=0;      //Current Latitude
var curLongtitude=0;    //Current Longtitude
var gisResId=0;     //Gis resource ID
var infoWin=null;    //Information form handle
var line=null;   //Planning route
var startMarker=null;    //Path start tag
var endMarker=null;
var ocxMap=new Array();    //resource dictionary
var curOcx = null;  //Current Ocx object

//Set novice navigation information
$(document).ready(function()
{
        var guide = [
            {ele:$("#start"),title:"Navigation STEP #1",text:"Fill in the latitude and longitude of the starting point"},
            {ele:$("#u50"),title:"Navigation STEP #2",text:"click this button then click left button on the map to set starting point"},
            {ele:$("#end"),title:"Navigation STEP #3",text:"Fill in the end of the latitude and longitude"},
            {ele:$("#u57"),title:"Navigation STEP #4",text:"click this button then click left button on the map to set end point"},
            {ele:$("#u68"),title:"Navigation STEP #5",text:"According to the starting point and the end of the planning path"},

            {ele:$("#gisid"),title:"eLTE operation STEP #1",text:"Fill in the terminal ID which need to be located"},
            {ele:$("#setResourceID"),title:"eLTE operation STEP #2",text:"Add the location of the terminal resource ID"},
            {ele:$("#delResourceID"),title:"eLTE operation STEP #3",text:"Delete the location information of the ID number in the input box"},
            {ele:$("#u24"),title:"eLTE operation STEP #4",text:"click，Then Start positioning the terminal"},
            {ele:$("#u3"),title:"eLTE operation STEP #5",text:"click，Then Stop positioning the terminal"},
            {ele:$("#u17"),title:"eLTE operation STEP #6",text:"Draw the current running track of current terminal"},
            {ele:$("#u10"),title:"eLTE operation STEP #7",text:"move to the position of current terminal"},
            {ele:$("#u35"),title:"Reset Option STEP #8",text:"Reset All"},

            {ele:$("#resultcode"),title:"eLTE operation STEP #9",text:"Show call API result here: function Name:Error Code"},
            {ele:$("#eventType"),title:"eLTE operation STEP #10",text:"Show callback message here"}
        ];
        $("#startGuide").click(function(){
            $.wlGuide(guide);
        });
});

function onLoad() 
{ 
    //Initialization map object 
    map=new TMap("mapDiv"); 
    //设置显示地图的中心点和级别 
    map.centerAndZoom(new TLngLat(120.72628,31.26507),16); 
    //Set the center and level of the map 
    map.enableHandleMouseScroll(); 
    //Allows double click on the map to enlarge 
    map.enableDoubleClickZoom(); 
    //Enable continuous zoom effect
    map.enableContinuousZoom(); 
    //Eagle eye map set
    var config = {
    anchor: "TMAP_ANCHOR_BOTTOM_RIGHT", //Set the eagle eye position,"TMAP_ANCHOR_TOP_LEFT"means top left，
                                        //"TMAP_ANCHOR_TOP_RIGHT"means right left,
                                        //"TMAP_ANCHOR_BOTTOM_LEFT"means Lower left,
                                        //"TMAP_ANCHOR_BOTTOM_RIGHT"means lower right，
                                        //"TMAP_ANCHOR_LEFT"means left，
                                        //"TMAP_ANCHOR_TOP"means top，
                                        //"TMAP_ANCHOR_RIGHT"means right，
                                        //"TMAP_ANCHOR_BOTTOM"means Below，
                                        //"TMAP_ANCHOR_OFFSET"Represents a custom location,default is"TMAP_ANCHOR_BOTTOM_RIGHT" 
        size  : new TSize(180,120),         //Hawkeye display size 
        isOpen : true                       //whether the eagle is open，true or false，default is false 
    }; 
    //Create OverviewMap object 
    overviewMap = new TOverviewMapControl(config); 
    //Add OverviewMap 
    map.addControl(overviewMap);
    //Zoom control configuration
    var config = { 
        type:"TMAP_NAVIGATION_CONTROL_LARGE",   //Zoom and pan display type 
        anchor:"TMAP_ANCHOR_TOP_LEFT",          //Zoom and pan control display location 
        offset:[0,0],                           //The offset value of the zoom and pan control 
        showZoomInfo:true                       //Whether to display the level of information，true or false。 
    }; 
    //Create a zoom and pan control object
    control=new TNavigationControl(config); 
    //Add zoom and pan control
    map.addControl(control); 
    //Create scale control object
    var scale = new TScaleControl();
    //Set scale color
    scale.setColor("#FF0A0A");
     //Add scale control
    map.addControl(scale);

     //Create a map type control object 
    var control = new TMapTypeControl({mapTypes:[TMAP_NORMAL_MAP, TMAP_SATELLITE_MAP, TMAP_HYBRID_MAP]}); 
    //Add a map type control to a map 
    map.addControl(control); 
    //Set default display satellite 
    map.setMapType(TMAP_NORMAL_MAP); 
     
    //Path planning start point 
    var config = { 
        icon : new TIcon(startIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    }; 
    startTool = new TMarkTool(map,config); 
    TEvent.addListener(startTool,"mouseup",mouseUpStartMaker); 
     
    //Path planning end point 
    var config = { 
        icon : new TIcon(endIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    }; 
    endTool = new TMarkTool(map,config); 
    TEvent.addListener(endTool,"mouseup",mouseUpEndMaker); 
     
    var config = { 
        //Driving strategy 
        policy:0,
        //Retrieves the completed callback function 
        onSearchComplete:searchResult   
    }; 
     
    //Create a public transport search object 
    drivingRoute = new TDrivingRoute(map,config);
    //Init eLTE
    setTimeout(Init(),1000);
    //load information and login
    setTimeout(Load(),1000);
    initData();
}
 
//Drive search 
function searchDrivingRoute() 
{ 
    //Empty map 
    //map.removeOverLay(line);
    map.clearOverLays();
     
    //Latitude and longitude of the starting point 
    var startVal = document.getElementById("start").value.split(","); 
    startLngLat = new TLngLat(startVal[0],startVal[1]); 
    //End latitude and longitude 
    var endVal = document.getElementById("end").value.split(","); 
    endLngLat = new TLngLat(endVal[0],endVal[1]); 
     
    //Set driving strategy 
    drivingRoute.setPolicy(getRadioValue()); 
    //Drive route search 
    drivingRoute.search(startLngLat,endLngLat);
} 
 
//Display bus search results 
function searchResult(result) 
{ 
    //Add start point 
    createStartMarker(); 
     
    obj = result; 
    var resultList = document.createElement("div"); 
    //Get a number of Solutions  
    var routes = result.getNumPlans(); 
    for(var i=0;i<routes;i++) 
    { 
        //Get a driving program results object 
        var plan = result.getPlan(i); 
        //Display driving circuit 
        createRoute(plan.getPath());
        //According to the provided coordinates of the point array set map view, 
        //the adjustment of the field of vision will be guaranteed to provide the coordinates of the point
       map.setViewport(plan.getPath()); 
    } 
} 
 
//Bus lines, pointsStr said the latitude and longitude of the string, 
//type said the line type 1, walk; 2, bus; 3, Metro; 4, the subway station transfer, 
//lnglat said the bus or subway icon to display the latitude and longitude
function createRoute(lnglats,lineColor) 
{ 
    //create Line object 
    line = new TPolyline(lnglats,{strokeColor:"#2C64A7", strokeWeight:5, strokeOpacity:0.9}); 
    //Add lines to the map 
    map.addOverLay(line); 
}

function clearDrivingRoute()
{
    map.clearOverLays();

    //Traversal to clear all the marker within the ocxMap
    for (var i in ocxMap) 
    {
        if (null != ocxMap[i].Marker) 
        {
            //unload and delete ocx controls
            delOcx(ocxMap[i].ocxName);
            ocxMap[i].ClickEvtHandle = TEvent.clearListeners(ocxMap[i].Marker,"click");
            ocxMap[i].ClickEvtHandle = null;
        }

    }
    //Empty all the data in the dictionary
    ocxMap = [];
    curLongtitude = 0;
    curLatitude = 0;    
}
 
//Add start point 
function createStartMarker() 
{ 
    //Add a starting point to the map
    var icon = new TIcon(startIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    startMarker = new TMarker(startLngLat,{icon:icon}); 
    map.addOverLay(startMarker); 
    //Add a termial point to the map
    var icon = new TIcon(endIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    endMarker = new TMarker(endLngLat,{icon:icon}); 
    map.addOverLay(endMarker); 
} 

function mouseUpStartMaker(point) 
{ 
    //Add a starting point to the map 
    var icon = new TIcon(startIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    startMarker = new TMarker(point,{icon:icon}); 
    map.addOverLay(startMarker); 
    //Close punctuation 
    startTool.close(); 
    //Set bus search starting point 
    document.getElementById("start").value = point.getLng()+","+point.getLat(); 
    map.removeOverLay(startTool);
} 
 
function mouseUpEndMaker(point) 
{ 
    //Add a starting point to the map 
    var icon = new TIcon(endIcon,new TSize(30,42),{anchor:new TPixel(21,42)}) 
    endMarker = new TMarker(point,{icon:icon}); 
    map.addOverLay(endMarker); 
    //Close punctuation 
    endTool.close(); 
    //Set bus terminal 
    document.getElementById("end").value = point.getLng()+","+point.getLat(); 
} 
 
//Drive route strategy 
function getRadioValue() 
{ 
    var obj = document.getElementsByName("planType"); 
    for(var i=0;i<obj.length;i++) 
    { 
        if(obj[i].checked)
        { 
            return obj[i].value; 
        } 
    } 
} 

//draw track
function drawtrack(){
    var line;
    var i = findMapByResID($("#gisid").val());
    if ((-1) == i) 
    {
        alert("Unknown ResourceID:"+$("#gisid").val());
        return;
    }
    //Array of points to obtain a set of points
    var pts = ocxMap[i].LonLatArr;
    //get the number of all points
    var paths = pts.length;
    if ((0 == pts[0].getLng())&&(0 == pts[0].getLat())) 
    {
        line = [pts[1],pts[1]]; 
    }
    else
    {
        line = [pts[0],pts[1]]; 
    }
    //create line object
    line = new TPolyline(line, {strokeColor:"red", strokeWeight:3, strokeOpacity:0.5}); 
    map.addOverLay(line);        

    setTimeout(function(){
        resetMkPoint(2,pts);
    },100);        
}

function resetMkPoint(i,pts){
    //carMk.setLngLat(pts[i]);
    if (undefined != pts[i]) 
    {
        line = [pts[i-1],pts[i]];
        //create Line object
        line = new TPolyline(line, {strokeColor:"red", strokeWeight:3, strokeOpacity:0.5}); 
        map.addOverLay(line);
        if(i < paths){
            setTimeout(function(){
                i++;
                resetMkPoint(i,pts);
                lnglat = pts[i];
            },50);
        }
        else
        {
           return;
        }
    }
    else
    {
        //Has been drawn to the end of the track
        return;
    }
}


function popup(resID)
{
        resourceID = resID;

        var i = findMapByResID(resourceID);
        if ((-1) == i) 
        {
            alert("Unknown ResourceID: "+resourceID);
            return;
        }
        //Grasp the most recent coordinate point information        
        //Create an information window object 
        infoWin=new TInfoWindow(ocxMap[i].LonLatArr[ocxMap[i].LonLatArr.length-1],new TPixel([0,-34])); 
        infoWin.setTitle("ResourceID："+resID);
        infoWin.setWidth(160);

        //Set the contents of the information window to display 
        var html = "<img src=\'images/call.png\' width=\'40px\' height=\'40px\' onclick=\'Startcall()\'>";
        html += "<img src=\'images/hangup.png\' width=\'40px\' height=\'40px\' onclick=\'HangUp()\'>";
        html += "<img src=\'images/video.png\' width=\'40px\' height=\'40px\' onclick=\'StartVideoPlay(eLTE_PlayerOCX_"+resID+","+resID+")\'>";
        
        infoWin.setLabel(html); 
        map.addOverLay(infoWin);
}

function StartGetPos()
{
    //Open location service
    GISSubscribe();

    var lon = parseFloat(curLongtitude);
    var lat = parseFloat(curLatitude);

    var pos = new TLngLat(lon, lat);

    //To determine if there is a dictionary of resources in the current resource box
    var newArr = ocxMap.filter(function(item){
            return item.resourceID === $("#gisid").val();
        });
    var i = findMapByResID($("#gisid").val());
    if (0 == newArr.length) 
    {
        alert("Unknown ResourceID,plaease Add ResourceID first");
        return;
    }
    if ((false == newArr[0].locateFlag)&&(1 == newArr.length)) {
        //First click on the positioning button
        ocxMap[i].locateFlag = true;
        carMk = new TMarker(pos,{icon:myIcon});
        map.addOverLay(carMk);
        
        var posArr = new Array();
        posArr.push(pos);
        //In accordance with the name of the terminal to increase the OCX control
        addOcx("eLTE_PlayerOCX_"+$("#gisid").val());
        
        if ((-1)!=i)
        {            
            ocxMap[i].LonLatArr = posArr;
            ocxMap[i].Marker = carMk;
            var ocxName = document.getElementById("eLTE_PlayerOCX_"+$("#gisid").val());
            ocxMap[i].ocxName = ocxName;
        }
        else
        {
            alert("please add the resourceID first!");
        }
        //ocxMap.push({resourceID: $("#gisid").val(),LonLatArr: posArr,Marker:carMk,ocxName:"eLTE_PlayerOCX_"+$("#gisid").val()});
        
    }
    else
    {
        //Repeat click on the positioning button
        carMk.setLngLat(pos);
        var i = findMapByResID($("#gisid").val());
        ocxMap[i].LonLatArr.push(pos);
        if (null == ocxMap[i].ClickEvtHandle)
        {
            ocxMap[i].ClickEvtHandle = TEvent.addListener(carMk,"click",function(p){
                //get current position
                currentPos = p;
                popup(ocxMap[i].resourceID);
            });
        }
    }
}

function delmark()
{
    var i = findMapByResID($("#gisid").val());
    if ((-1) == i) 
    {
        alert("Unknown ResourceID:"+$("#gisid").val());
        return;
    }
    //delete current marker
    map.removeOverLay(ocxMap[i].Marker); 
    ocxMap[i].locateFlag = false;
    //alert("delmark: delete marker:" + $("#gisid").val() + " success!");
}

function addOcx(ocxName)
{
    //add an named ocx
    if (null == document.getElementById(ocxName))
    {
        document.getElementById("OCXList").innerHTML += "<OBJECT id='"+ocxName+"' classid='CLSID:6AB35B6F-D5D4-4DA3-9503-1AD4E5E1E589'></OBJECT>";
    }
    /*else
    {
        alert("ocx: "+ocxName+" already exist!");
    }*/

}

function delOcx(ocxName)
{
    //delete an ocx
    if (null != document.getElementById(ocxName)) 
    {
        
        if (null != ocxName) 
        {
            ocxName.ELTE_OCX_UnLoad();
        }
        
        $("#"+ocxName).empty();
        $("#"+ocxName).remove();
    }
}

function resetOcx()
{
    document.getElementById("OCXList").innerHTML = "<OBJECT id='eLTE_PlayerOCX' classid='CLSID:6AB35B6F-D5D4-4DA3-9503-1AD4E5E1E589'></OBJECT>";
}


function StopGetPos()
{
    var i = findMapByResID($("#gisid").val());
    if ((-1) == i) 
    {
        alert("Unknown ResourceID:"+$("#gisid").val());
    }

    //停止定位业务  
    GISUnSubscribe();
    ocxMap[i].locateFlag = false;
}

function ToMyPos()
{
    if (("0" != curLongtitude)&&("0" != curLatitude))
    {
        var lon = parseFloat(curLongtitude);
        var lat = parseFloat(curLatitude);
        map.centerAndZoom(new TLngLat(lon, lat),16);
    }
    else
    {
        alert("locate error,please make sure that the device is in the open area and the signal is normal!");
    }
    
}

function StartVideoPlay(ocxName, resID)
{
    if (isLogin)
    {
        if (ocxName) 
        {
            ocxName.ELTE_OCX_Load(2);
            SetVideoPos(ocxName,currentPos.x + 60, currentPos.y - 60);
            StartRealPlay(resID); 
            curOcx = ocxName;
        }      
    }
    else
    {
      Load();
      initData();
    }
    //Close information form
    if (null != infoWin) 
    {
        infoWin.closeInfoWindow();
        infoWin = null;
    }
}

function Startcall()
{
    P2PDial();
    //Close information form
    if (null != infoWin) 
    {
        infoWin.closeInfoWindow();
        infoWin = null;
    }
}

function HangUp()
{
    P2PHangup();
    //Close information form
    if (null != infoWin) 
    {
        infoWin.closeInfoWindow();
        infoWin = null;
    }
}

function initData()
{
    curLatitude=0;
    curLongtitude=0;
    curLonLat = null;
}

function posMoving(lat, lon, resID)
{
    var i = findMapByResID(resID);
    if ((-1) != i) 
    {
        if (true == ocxMap[i].locateFlag) 
        {
            var flat = parseFloat(lat);
            var flon = parseFloat(lon);
            curLonLat = new TLngLat(lon, lat);
            carMk.setLngLat(curLonLat);

            //Add a new location to the array
            (ocxMap[i].LonLatArr).push(curLonLat);

            if (null == ocxMap[i].ClickEvtHandle) 
            {
                ocxMap[i].ClickEvtHandle = TEvent.addListener(ocxMap[i].Marker,"click",function(p){
                    //Gets the current screen coordinates
                    currentPos = p;
                    popup(ocxMap[i].resourceID);
                });
            }
                
        }
    }
    else
    {
       alert("Unknown resourceID："+resID);
       return; 
    }
    
}

function setstartpos()
{
    if (null != document.getElementById("start")) 
    {
        //map.removeOverLay(startTool);

    }

    startTool.open();
}

//Lookup traversal dictionary
function findMapByResID(resId)
{
    for (var i = 0; i < ocxMap.length; i++) {
            if (undefined != ocxMap[i]) 
            {
                if (resId == ocxMap[i].resourceID) {
                return i;
            }        
        }
    }
    return -1;
}