::关闭回显
@echo off

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

::工程所在路径（sln路径），根据该脚本所在目录进行修改
set ProjectPath=%cd%

::生成的压缩包路径，根据要求进行修改
set ZipPath=%ProjectPath%\..\..\..\00.Release\%DateTime%
set ZipName=%ZipPath%\eSDK_eLTE_SDK_V1.5.10.zip


::拷贝依赖库
::--------------------------------------------------------------------------------
set eLTE_Product_SDK_PATH=%ProjectPath%\..\..\platform\eDC610_TD V100R003C00B027
set eSDKClientLogAPI_PATH=%ProjectPath%\..\..\platform\eSDKClientLogAPI
set Srv_OutPut_Debug_PATH=%ProjectPath%\output\debug\eLTE_NativeService
set Srv_OutPut_Release_PATH=%ProjectPath%\output\release\eLTE_NativeService
set Cli_OutPut_Debug_PATH=%ProjectPath%\output\debug\eLTE_SDK
set Cli_OutPut_Release_PATH=%ProjectPath%\output\release\eLTE_SDK

::拷贝debug库
::eLTE Product SDK
xcopy /Y "%eLTE_Product_SDK_PATH%\*.dll" 					"%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\*.ini" 					"%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\sdk_plugins\*.dll" 		"%Srv_OutPut_Debug_PATH%\sdk_plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\plugins\*.dll" 			"%Srv_OutPut_Debug_PATH%\plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\openssl\lib\*.dll" 		"%Srv_OutPut_Debug_PATH%\openssl\lib\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.cnf" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.jks" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.keystore" 			"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.pem" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.txt" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.xsd" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.xml" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.ini" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.xsd" 		"%Srv_OutPut_Debug_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.xml" 		"%Srv_OutPut_Debug_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.cnf" 			"%Srv_OutPut_Debug_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.xml" 			"%Srv_OutPut_Debug_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\*.xml" 	"%Srv_OutPut_Debug_PATH%\conf\cm_service\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\en\*.xml" "%Srv_OutPut_Debug_PATH%\conf\cm_service\en\"
::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.dll" 				"%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.ini" 				"%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.dll" 				"%Cli_OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.ini" 				"%Cli_OutPut_Debug_PATH%\"

::拷贝release库
::eLTE Product SDK
xcopy /Y "%eLTE_Product_SDK_PATH%\*.dll" 					"%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\*.ini" 					"%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\sdk_plugins\*.dll" 		"%Srv_OutPut_Release_PATH%\sdk_plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\plugins\*.dll" 			"%Srv_OutPut_Release_PATH%\plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\openssl\lib\*.dll" 		"%Srv_OutPut_Release_PATH%\openssl\lib\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.cnf" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.jks" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.keystore" 			"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.pem" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.txt" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.xsd" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.xml" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.ini" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.xsd" 		"%Srv_OutPut_Release_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.xml" 		"%Srv_OutPut_Release_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.cnf" 			"%Srv_OutPut_Release_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.xml" 			"%Srv_OutPut_Release_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\*.xml" 	"%Srv_OutPut_Release_PATH%\conf\cm_service\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\en\*.xml" "%Srv_OutPut_Release_PATH%\conf\cm_service\en\"
::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.dll" 			"%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.ini" 			"%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.dll" 			"%Cli_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.ini" 			"%Cli_OutPut_Release_PATH%\"
::--------------------------------------------------------------------------------


::编译
::--------------------------------------------------------------------------------
::编译 eLTE_SDK Debug
echo "--------- 编译 eLTE_SDK Debug start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_SDK.sln" /Rebuild "Debug|Win32" /out builddebug.log
echo "--------- 编译 eLTE_SDK Debug  end  ---------"

::编译 eLTE_SDK Release
echo "--------- 编译 eLTE_SDK Release start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_SDK.sln" /Rebuild "Release|Win32" /out buildrelease.log
echo "--------- 编译 eLTE_SDK Release  end  ---------"
::--------------------------------------------------------------------------------


::打包
::--------------------------------------------------------------------------------
::设置各变量名
set	WinRarRoot=C:\Program Files\WinRAR

::创建目录
mkdir "%ZipPath%\"

::打包 eLTE SDK 头文件
echo "--------- 打包 eLTE SDK 头文件 start ---------"
(chdir "%ProjectPath%\eLTE_SDK\")
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"include" "%ZipName%" "eLTE_Types.h"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"include" "%ZipName%" "eLTE_SDK.h"
echo "--------- 打包 eLTE SDK 头文件  end  ---------"

::打包eLTE SDK Debug
echo "--------- 打包 eLTE SDK Debug start ---------"
(chdir "%ProjectPath%\output\")
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.exe"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.ocx"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.bat"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.conf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\sdk_plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\openssl\lib\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.cnf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.jks"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.keystore"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.pem"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.txt"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.xsd"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\error\*.xsd"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\error\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\dc\*.cnf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\dc\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\cm_service\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\cm_service\en\*.xml"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\sign\*.pem"

"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.lib"
echo "--------- 打包 eLTE SDK Debug  end  ---------"

::打包eLTE SDK Release
echo "--------- 打包 eLTE SDK Release start ---------"
(chdir "%ProjectPath%\output\")
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.exe"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.ini"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.dll"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.ocx"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.bat"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\*.conf"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\sdk_plugins\*.dll"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\plugins\*.dll"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\openssl\lib\*.dll"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.cnf"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.jks"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.keystore"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.pem"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.txt"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.xsd"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.xml"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\*.ini"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\error\*.xsd"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\error\*.xml"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\dc\*.cnf"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\dc\*.xml"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\cm_service\*.xml"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\conf\cm_service\en\*.xml"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_NativeService\sign\*.pem"

"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.lib"
echo "--------- 打包 eLTE SDK Release  end  ---------"