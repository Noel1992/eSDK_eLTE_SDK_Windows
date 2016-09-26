::关闭回显
@echo off

::工程所在路径（sln路径），根据该脚本所在目录进行修改
set ProjectPath=%cd%\..\..\source\eLTE_Player

::拷贝依赖库
::--------------------------------------------------------------------------------
set eLTE_SDK_PATH=%ProjectPath%\..\..\platform\eDC610_TD V100R004C10B185
set IVS_Player_PATH=%ProjectPath%\..\..\platform\IVS_Player
set IVS_NetSource_PATH=%ProjectPath%\..\..\self_dev\IVS_NetSource
set eSDKClientLogAPI_PATH=%ProjectPath%\..\..\platform\eSDKClientLogAPI
set SDL2_PATH=%ProjectPath%\..\..\open_src\sdl2-2.0.3\release\x86
set Microsoft_DLL_PATH=%ProjectPath%\..\..\third_party\Microsoft Visual Studio 10.0
set OutPut_Debug_PATH=%ProjectPath%\output\debug
set OutPut_Release_PATH=%ProjectPath%\output\release

::拷贝debug库
::eLTE SDK
xcopy /Y 	"%eLTE_SDK_PATH%\*.*" 					"%OutPut_Debug_PATH%\"
xcopy /Y /E "%eLTE_SDK_PATH%\sdk_plugins\*.*" 		"%OutPut_Debug_PATH%\sdk_plugins\"
xcopy /Y /E "%eLTE_SDK_PATH%\plugins\*.*" 			"%OutPut_Debug_PATH%\plugins\"
xcopy /Y /E "%eLTE_SDK_PATH%\conf\*.*" 				"%OutPut_Debug_PATH%\conf\"
xcopy /Y /E "%eLTE_SDK_PATH%\cert\*.*" 				"%OutPut_Debug_PATH%\cert\"
xcopy /Y /E "%eLTE_SDK_PATH%\pdb\*.*" 				"%OutPut_Debug_PATH%\pdb\"
xcopy /Y /E "%eLTE_SDK_PATH%\res\*.*" 				"%OutPut_Debug_PATH%\res\"
::IVS Player
xcopy /Y "%IVS_Player_PATH%\dll\*.dll" 				"%OutPut_Debug_PATH%\"
xcopy /Y "%IVS_Player_PATH%\dll\*.conf" 			"%OutPut_Debug_PATH%\"
::IVS NetSource
xcopy /Y "%IVS_NetSource_PATH%\dll\*.dll" 			"%OutPut_Debug_PATH%\"
::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.dll" 		"%OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\eSDKClientLogCfgOcx.ini" 		"%OutPut_Debug_PATH%\"
::microsoft vs
xcopy /Y "%Microsoft_DLL_PATH%\mfc100u.dll" 		"%OutPut_Debug_PATH%\"
::SDL2
xcopy /Y "%SDL2_PATH%\dll\*.dll" 			        "%OutPut_Debug_PATH%\"

::拷贝release库
::eLTE SDK
xcopy /Y 	"%eLTE_SDK_PATH%\*.*" 					"%OutPut_Release_PATH%\"
xcopy /Y /E "%eLTE_SDK_PATH%\sdk_plugins\*.*" 		"%OutPut_Release_PATH%\sdk_plugins\"
xcopy /Y /E "%eLTE_SDK_PATH%\plugins\*.*" 			"%OutPut_Release_PATH%\plugins\"
xcopy /Y /E "%eLTE_SDK_PATH%\conf\*.*" 				"%OutPut_Release_PATH%\conf\"
xcopy /Y /E "%eLTE_SDK_PATH%\cert\*.*" 				"%OutPut_Release_PATH%\cert\"
xcopy /Y /E "%eLTE_SDK_PATH%\pdb\*.*" 				"%OutPut_Release_PATH%\pdb\"
xcopy /Y /E "%eLTE_SDK_PATH%\res\*.*" 				"%OutPut_Release_PATH%\res\"
::IVS Player
xcopy /Y "%IVS_Player_PATH%\dll\*.dll" 				"%OutPut_Release_PATH%\"
xcopy /Y "%IVS_Player_PATH%\dll\*.conf" 			"%OutPut_Release_PATH%\"
::IVS NetSource
xcopy /Y "%IVS_NetSource_PATH%\dll\*.dll" 			"%OutPut_Release_PATH%\"
::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.dll" 	"%OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\release\eSDKClientLogCfgOcx.ini" 	"%OutPut_Release_PATH%\"
::microsoft vs
xcopy /Y "%Microsoft_DLL_PATH%\mfc100u.dll" 		"%OutPut_Release_PATH%\"
::SDL2
xcopy /Y "%SDL2_PATH%\dll\*.dll" 			        "%OutPut_Release_PATH%\"
::--------------------------------------------------------------------------------


::编译
::--------------------------------------------------------------------------------
::编译eLTE_Player.ocx控件Debug
echo "--------- 编译 eLTE_Player.ocx Debug start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_Player.sln" /Rebuild "Debug|Win32" /out builddebug.log
echo "--------- 编译 eLTE_Player.ocx Debug  end  ---------"

xcopy /Y "%OutPut_Debug_PATH%\*.pdb" 				"%OutPut_Debug_PATH%\pdb\"

::编译eLTE_Player.ocx控件Release
echo "--------- 编译 eLTE_Player.ocx Release start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_Player.sln" /Rebuild "Release|Win32" /out buildrelease.log
echo "--------- 编译 eLTE_Player.ocx Release  end  ---------"
::--------------------------------------------------------------------------------


::获得当前时间，作为生成版本的目录名
for /F "tokens=1-4 delims=-/ " %%i in ('date /t') do (
   set Year=%%i
   set Month=%%j
   set Day=%%k
   set DayOfWeek=%%l
)
for /F "tokens=1-2 delims=: " %%i in ('time /t') do (
   set Hour=%%i
   set Minute=%%j
)

::设置各变量名
set DateTime=%Year%-%Month%-%Day%-%Hour%-%Minute%
set DateOnly=%Year%-%Month%-%Day%

::工程所在路径（sln路径），根据该脚本所在目录进行修改
set ProjectPath=%cd%\..\..\source\eLTE_Player

::生成的压缩包路径，根据要求进行修改
set ZipPath=%ProjectPath%\..\..\..\00.Release\%DateOnly%
set ZipName=%ZipPath%\eSDK_eLTE_OCX_V1.5.30.zip

::打包
::--------------------------------------------------------------------------------
::设置各变量名
set	WinRarRoot=C:\Program Files\WinRAR

::创建目录
mkdir "%ZipPath%\"

::打包eLTE_Player.ocx控件Debug
::目前只能支持debug版本，因此不打包release版本
echo "--------- 打包 eLTE_Player.ocx Debug start ---------"
(chdir "%ProjectPath%\output\debug")
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "*.ocx"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "*.bat"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "*.conf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "sdk_plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "plugins\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\error\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\dc\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\itm\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\cm_service\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\cm_service\en\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "conf\cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "pdb\*.pdb"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "res"

"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "Lang\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "Skin\*.png"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "Skin\ToolBar\*.png"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "Skin\Caption\*.png"
echo "--------- 打包 eLTE_Player.ocx Debug  end  ---------"
::--------------------------------------------------------------------------------