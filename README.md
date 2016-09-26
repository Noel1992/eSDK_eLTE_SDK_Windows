# esdk_elte
ocx编译如下

1.到\CI\script\目录下运行compile_player.bat脚本。

2.到\source\eLTE_Player\output\debug\目录下已管理员身份运行regsvrOCX.bat脚本，成功之后，会显示eLTE_Player.ocx注册成功。

3.到\test\eLTE_Player\eLTE_PlayerDemo\目录下打开eLTE_PlayerDemo.sln，选用debug或release环境下编译源代码，成功之后， 会在\test\eLTE_Player\eLTE_PlayerDemo\output\debug\或者\test\eLTE_Player\eLTE_PlayerDemo\output\release\目录生成相应的可执行文件，用户可以双击运行。

注 如果运行可执行文件时提示缺少系统dll,可以到\third_party\Microsoft Visual Studio 10.0\目录下进行拷贝。

独立进程sdk编译如下

1.到\CI\script\目录下运行compile_sdk.bat脚本。

2.到\source\eLTE_SDK\output\debug\eLTE_NativeService\目录下以管理员身份运行InstallService.bat脚本，成功之后，会提示安装服务成功。如果服务之前安装过，需要先进行服务卸载（以管理员身份运行UninstallService.bat脚本），然后再安装。

3.到\test\eLTE_SDK\eLTE_SDK_Demo\目录下打开eLTE_SDK_Demo.sln，选用debug或release环境下编译源代码，成功之后， 会在\test\eLTE_SDK\eLTE_SDK_Demo\output\debug\或者\test\eLTE_SDK\eLTE_SDK_Demo\output\release\目录生成相应的可执行文件，用户可以双击运行。

注 如果运行可执行文件时提示缺少系统dll,可以到\third_party\Microsoft Visual Studio 10.0\目录下进行拷贝。

附录：
	eLTE开发指南：
	http://support.huawei.com/enterprise/docinforeader.action?contentId=DOC1000083224&idPath=7919754%7C19891038%7C21268681
	eLTE编译指南：
	https://github.com/eSDK/esdk_elte/wiki/eLTE%E7%BC%96%E8%AF%91%E6%8C%87%E5%8D%97
	News：
	http://support.huawei.com/enterprise/docinforeader.action?contentId=DOC1000083224&idPath=7919754%7C19891038%7C21268681
	
