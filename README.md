## eSDK\_eLTE\_SDK\_Windows  ##
华为eLTE宽带集群解决方案，基于最先进的LTE无线宽带技术，上行吞吐量可达50Mbps，下行吞吐量达100Mbps。一网支持多媒体集群语音、视频调度，高清无线视频监控，远程数据采集和移动办公业务。

该方案是业界首个宽带集群解决方案，同时支持语音和视频调度。其专业的语音性集群组呼时延<300ms,群组抢占时延<150ms，关键时刻值得信赖。同时支持的视频上传，以及分发。调度过程不仅能听到，而且能看到，做到“现场可见，指挥可达“。

该方案还切实地结合了行业特殊需求，定制了高集成度的小型化核心网设备和分布式基站，灵活部署，降低建网和维护成本；丰富的工业级终端设备，具有更高的可靠性，能够适应严酷的室外环境，帮助企业打造无处不在的宽带连接，构建现代化的信息平台。 

华为宽带集群**eSDK\_eLTE\_SDK\_Windows**提供基于Windows动态库标准化API接口和OCX控件，为您提供集群用户管理、群组管理、语音点呼、语音组呼、人工转接、缜密监听、环境监听、实时视频回传与分发、视频上墙、短彩信发送与接收以及集群终端GIS订阅与地理位置信息上报等管理调度功能。

## 版本更新 ##
eSDK eLTE最新版本v2.1.10

## 开发环境 ##

- 操作系统： Windows7专业版
- Microsoft Visual Studio：Visual Studio 2010专业版

## 文件指引 ##

- src文件夹：eSDK\_eLTE\_SDK\_Windows源码
- sample文件夹：eSDK eLTE SDK的代码样例
- doc：eSDK eLTE SDK的接口参考、开发指南
- third-party:eSDK eLTE SDK中使用的第三方库


## 入门指导 ##

- 编译eSDK\_eLTE\_SDK\_Windows\_OCX：在src文件夹中找到source\eLTE\_SDK\_Player\eLTE\_Player.sln，使用VS2010打开，在VS2010中，菜单>Build>Build Solution，注意区分Debug和Realse版本
- 编译运行sample：到\CI\script\目录下运行compile\_player.bat脚本。到\source\eLTE\_Player\output\debug\目录下已管理员身份运行regsvrOCX.bat脚本，成功之后，会显示eLTE_Player.ocx注册成功。到\test\eLTE_Player\eLTE\_PlayerDemo\目录下打开eLTE\_PlayerDemo.sln，选用debug或release环境下编译源代码，成功之后， 会在\test\eLTE\_Player\eLTE\_PlayerDemo\output\debug\或者\test\eLTE\_Player\eLTE\_PlayerDemo\output\release\目录生成相应的可执行文件，用户可以双击运行。(如果运行可执行文件时提示缺少系统dll,可以到\third\_party\Microsoft Visual Studio 10.0\目录下进行拷贝。)

- 编译eSDK\_eLTE\_SDK\_Windows\_SDK：在src文件夹中找到source\eLTE\_SDK\eLTE\_SDK.sln，使用VS2010打开，在VS2010中，菜单>Build>Build Solution，注意区分Debug和Realse版本
- 编译运行sample：到\CI\script\目录下运行compile\_sdk.bat脚本。到到\source\eLTE_SDK\output\debug\eLTE_NativeService\目录下以管理员身份运行InstallService.bat脚本，成功之后，会提示安装服务成功。如果服务之前安装过，需要先进行服务卸载（以管理员身份运行UninstallService.bat脚本），然后再安装。到\test\eLTE_SDK\eLTE\_SDK\_Demo\目录下打开eLTE\_SDK\_Demo.sln，选用debug或release环境下编译源代码，成功之后， 会在\test\eLTE\_SDK\eLTE\_SDK\_Demo\output\debug\或者\test\eLTE\_SDK\eLTE\_SDK\_Demo\output\release\目录生成相应的可执行文件，用户可以双击运行。(如果运行可执行文件时提示缺少系统dll,可以到\third\_party\Microsoft Visual Studio 10.0\目录下进行拷贝。)


- 详细的开发指南请参考doc中的开发指南

###登录eLTE平台###
要体验华为eLTE系统的能力，首先要登录eLTE系统,以下代码演示如何登录eLTE系统

    int iRet = eLTE_SDK_Init(0);//init eLTE SDK
    
	//用户名
	CString strUserName = ("***");
	//用户密码
	CString strPWD = ("***");
	//服务器IP 
	CString strServerIP = ("**.**.**.**");
	//本地IP地址
	CString strLocalIP = ("**.**.**.**");
	//端口号
	int SipPort = 5060;
	//登录
	iRet = ELTE_SDK_Login(strUserName, strPWD, strServerIP, strLocalIP, SipPort);

## 获取帮助 ##

在开发过程中，您有任何问题均可以至[DevCenter](https://devcenter.huawei.com)中提单跟踪。也可以在[华为开发者社区](http://bbs.csdn.net/forums/hwucdeveloper)中查找或提问。另外，华为技术支持热线电话：400-822-9999（转二次开发）