::�رջ���
@echo off

::��������·����sln·���������ݸýű�����Ŀ¼�����޸�
set ProjectPath=%cd%\..\..\src\source\eLTE_SDK

::����������
::--------------------------------------------------------------------------------
set eLTE_Product_SDK_PATH=%ProjectPath%\..\..\platform\SDK
set eSDKClientLogAPI_PATH=%ProjectPath%\..\..\platform\eSDKClientLogAPI
set SDL2_PATH=%ProjectPath%\..\..\..\third_party\open_src\sdl2-2.0.3\release\x86
set SECURE_PATH=%ProjectPath%\..\..\self_dev\eSDKSecurec\windows
set Srv_OutPut_Debug_PATH=%ProjectPath%\output\debug\eLTE_NativeService
set Srv_OutPut_Release_PATH=%ProjectPath%\output\release\eLTE_NativeService
set Cli_OutPut_Debug_PATH=%ProjectPath%\output\debug\eLTE_SDK
set Cli_OutPut_Release_PATH=%ProjectPath%\output\release\eLTE_SDK

::����debug��
::eLTE Product SDK
xcopy /Y "%eLTE_Product_SDK_PATH%\cert\*.*" 				"%Srv_OutPut_Debug_PATH%\cert\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.*" 				"%Srv_OutPut_Debug_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cert\*.*" 			"%Srv_OutPut_Debug_PATH%\conf\cert\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.*" 		    "%Srv_OutPut_Debug_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.*" 			    "%Srv_OutPut_Debug_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\itm\*.*" 			"%Srv_OutPut_Debug_PATH%\conf\itm\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\ddp\*.*" 			"%Srv_OutPut_Debug_PATH%\conf\ddp\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\trace_info\*.*" 	    "%Srv_OutPut_Debug_PATH%\conf\trace_info\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\*.*" 	    "%Srv_OutPut_Debug_PATH%\conf\cm_service\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\en\*.*"   "%Srv_OutPut_Debug_PATH%\conf\cm_service\en\"
xcopy /Y "%eLTE_Product_SDK_PATH%\*.*" 					    "%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\sdk_plugins\*.dll" 		"%Srv_OutPut_Debug_PATH%\sdk_plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\plugins\*.dll" 			"%Srv_OutPut_Debug_PATH%\plugins\"
xcopy /Y /E "%eLTE_Product_SDK_PATH%\res\*.*" 				"%Srv_OutPut_Debug_PATH%\res\"
xcopy /Y /E "%eLTE_Product_SDK_PATH%\pdb\*.*" 				"%Srv_OutPut_Debug_PATH%\pdb\"

::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.dll" 				                "%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%SECURE_PATH%\eSDK_Secure_C.dll" 				                    "%Srv_OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\eSDKClientLogCfg.ini" 				    "%Srv_OutPut_Debug_PATH%\"

xcopy /Y "%eSDKClientLogAPI_PATH%\debug\*.dll" 				                "%Cli_OutPut_Debug_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\eSDKClientLogCfg.ini" 				"%Cli_OutPut_Debug_PATH%\"
xcopy /Y "%SECURE_PATH%\eSDK_Secure_C.dll" 				                    "%Cli_OutPut_Debug_PATH%\"

::SDL2
xcopy /Y "%SDL2_PATH%\dll\*.dll" 			                                "%Cli_OutPut_Debug_PATH%\"

::����release��
::eLTE Product SDK
xcopy /Y "%eLTE_Product_SDK_PATH%\cert\*.*" 				"%Srv_OutPut_Release_PATH%\cert\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\*.*" 				"%Srv_OutPut_Release_PATH%\conf\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cert\*.*" 			"%Srv_OutPut_Release_PATH%\conf\cert\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\error\*.*" 		    "%Srv_OutPut_Release_PATH%\conf\error\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\dc\*.*" 			    "%Srv_OutPut_Release_PATH%\conf\dc\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\itm\*.*" 			"%Srv_OutPut_Release_PATH%\conf\itm\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\ddp\*.*" 			"%Srv_OutPut_Release_PATH%\conf\ddp\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\trace_info\*.*" 	    "%Srv_OutPut_Release_PATH%\conf\trace_info\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\*.*" 	    "%Srv_OutPut_Release_PATH%\conf\cm_service\"
xcopy /Y "%eLTE_Product_SDK_PATH%\conf\cm_service\en\*.*"   "%Srv_OutPut_Release_PATH%\conf\cm_service\en\"
xcopy /Y "%eLTE_Product_SDK_PATH%\*.*" 					    "%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eLTE_Product_SDK_PATH%\sdk_plugins\*.dll" 		"%Srv_OutPut_Release_PATH%\sdk_plugins\"
xcopy /Y "%eLTE_Product_SDK_PATH%\plugins\*.dll" 			"%Srv_OutPut_Release_PATH%\plugins\"
xcopy /Y /E "%eLTE_Product_SDK_PATH%\res\*.*" 				"%Srv_OutPut_Release_PATH%\res\"
xcopy /Y /E "%eLTE_Product_SDK_PATH%\pdb\*.*" 				"%Srv_OutPut_Release_PATH%\pdb\"

::eSDKClientLogAPI
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.dll" 			                "%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\eSDKClientLogCfg.ini" 					"%Srv_OutPut_Release_PATH%\"
xcopy /Y "%SECURE_PATH%\eSDK_Secure_C.dll" 				                    "%Srv_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\release\*.dll" 			                "%Cli_OutPut_Release_PATH%\"
xcopy /Y "%eSDKClientLogAPI_PATH%\debug\eSDKClientLogCfg.ini" 				"%Cli_OutPut_Release_PATH%\"
xcopy /Y "%SECURE_PATH%\eSDK_Secure_C.dll" 				                    "%Cli_OutPut_Release_PATH%\"

::SDL2
xcopy /Y "%SDL2_PATH%\dll\*.dll" 			                                "%Cli_OutPut_Release_PATH%\"
::--------------------------------------------------------------------------------


::����
::--------------------------------------------------------------------------------
::���� eLTE_SDK Debug
echo "--------- ���� eLTE_SDK Debug start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_SDK.sln" /Rebuild "Debug|Win32" /out builddebug.log
echo "--------- ���� eLTE_SDK Debug  end  ---------"

xcopy /Y "%Srv_OutPut_Debug_PATH%\*.pdb" 				"%Srv_OutPut_Debug_PATH%\pdb\"
xcopy /Y "%Cli_OutPut_Debug_PATH%\*.pdb" 				"%Cli_OutPut_Debug_PATH%\pdb\"


::���� eLTE_SDK Release
echo "--------- ���� eLTE_SDK Release start ---------"
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%ProjectPath%\eLTE_SDK.sln" /Rebuild "Release|Win32" /out buildrelease.log
echo "--------- ���� eLTE_SDK Release  end  ---------"
::--------------------------------------------------------------------------------

xcopy /Y "%Cli_OutPut_Release_PATH%\*.pdb" 				"%Cli_OutPut_Release_PATH%\pdb\"

::��õ�ǰʱ�䣬��Ϊ���ɰ汾��Ŀ¼��
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

::���ø�������
set DateTime=%Year%-%Month%-%Day%-%Hour%-%Minute%
set DateOnly=%Year%-%Month%-%Day%

::��������·����sln·���������ݸýű�����Ŀ¼�����޸�
set ProjectPath=%cd%\..\..\src\source\eLTE_SDK

::���ɵ�ѹ����·��������Ҫ������޸�
set ZipPath=%ProjectPath%\..\..\..\..\00.Release\%DateOnly%
set ZipName=%ZipPath%\eSDK_eLTE_API_V2.1.10.zip

::���
::--------------------------------------------------------------------------------
::���ø�������
if "%PROCESSOR_ARCHITECTURE%"=="x86" goto x86
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto x64
:x86
set WinRarRoot=C:\Program Files\WinRAR
goto setfinish
:x64
set WinRarRoot=C:\Program Files (x86)\WinRAR
:setfinish

::����Ŀ¼
mkdir "%ZipPath%\"

::��� eLTE SDK ͷ�ļ�
echo "--------- ��� eLTE SDK ͷ�ļ� start ---------"
(chdir "%ProjectPath%\eLTE_SDK\")
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"include" "%ZipName%" "eLTE_Types.h"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"include" "%ZipName%" "eLTE_SDK.h"
echo "--------- ��� eLTE SDK ͷ�ļ�  end  ---------"

::���eLTE SDK Debug
echo "--------- ��� eLTE SDK Debug start ---------"
(chdir "%ProjectPath%\output\")
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.exe"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.ocx"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.bat"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\*.conf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\sdk_plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\itm\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\trace_info\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\ddp\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\error\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\dc\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\cm_service\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\conf\cm_service\en\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\sign\*.pem"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\res"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_NativeService\pdb\*.pdb"

REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.ini"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.dll"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\*.lib"
REM "%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "debug\eLTE_SDK\pdb\*.pdb"
echo "--------- ��� eLTE SDK Debug  end  ---------"

::���eLTE SDK Release
echo "--------- ��� eLTE SDK Release start ---------"
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

::����readme.txt
xcopy /Y "..\eLTE_SDK\ReadMe.txt" 			"%Cli_OutPut_Release_PATH%\"
xcopy /Y "..\eLTE_SDK\ReadMe.txt" 			"%Cli_OutPut_Debug_PATH%\"

"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\*.lib"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\pdb\*.pdb"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "release\eLTE_SDK\ReadMe.txt"
echo "--------- ��� eLTE SDK Release  end  ---------"


echo "--------- ��� eLTE SDK Demo Source  start  ---------"
set ZipName=%ZipPath%\eSDK_eLTE_API_Demo_Source_CPP.zip
(chdir "%ProjectPath%\output\debug\")
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.exe"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.ocx"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.bat"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\*.conf"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\sdk_plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\plugins\*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\itm\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\trace_info\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\ddp\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\error\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\dc\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\cert\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\cm_service\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\conf\cm_service\en\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\sign\*.pem"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\res"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_NativeService\pdb\*.pdb"

(chdir "%ProjectPath%\output\debug\eLTE_SDK\")
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\debug" "%ZipName%" "*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\debug" "%ZipName%" "*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\debug" "%ZipName%" "*.lib"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\debug" "%ZipName%" "pdb\*.pdb"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\debug" "%ZipName%" "ReadMe.txt"

(chdir "%ProjectPath%\output\release\eLTE_SDK\")
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\release" "%ZipName%" "*.ini"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\release" "%ZipName%" "*.dll"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\release" "%ZipName%" "*.lib"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\release" "%ZipName%" "pdb\*.pdb"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\release" "%ZipName%" "ReadMe.txt"

(chdir "%ProjectPath%\eLTE_SDK\")
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\include" "%ZipName%" "eLTE_Types.h"
"%WinRarRoot%\Rar.exe" u -s- -m1 -ap"eLTE_SDK\include" "%ZipName%" "eLTE_SDK.h"

(chdir "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\")
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo.sln"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\res\*.*"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.cpp"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.h"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.rc"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.vcxproj"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.filters"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.txt"
"%WinRarRoot%\Rar.exe" u -s- -m1 "%ZipName%" "eLTE_SDK_Demo\*.rc"

REM xcopy /Y "%ProjectPath%\eLTE_SDK\eLTE_Types.h" 			                                        "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\include\"
REM xcopy /Y "%ProjectPath%\eLTE_SDK\eLTE_SDK.h" 			                                        "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\include\"
REM xcopy /Y "%ProjectPath%\output\release\eLTE_SDK\*.ini"                                          "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\release\"
REM xcopy /Y "%ProjectPath%\output\release\eLTE_SDK\*.dll"                                          "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\release\"
REM xcopy /Y "%ProjectPath%\output\release\eLTE_SDK\*.lib"                                          "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\release\"
REM xcopy /Y "%ProjectPath%\output\release\eLTE_SDK\pdb\*.pdb"                                      "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\release\"
REM xcopy /Y "%ProjectPath%\output\release\eLTE_SDK\ReadMe.txt"                                     "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\release\"
REM xcopy /Y "%ProjectPath%\output\debug\eLTE_SDK\*.ini"                                            "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\debug\"
REM xcopy /Y "%ProjectPath%\output\debug\eLTE_SDK\*.dll"                                            "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\debug\"
REM xcopy /Y "%ProjectPath%\output\debug\eLTE_SDK\*.lib"                                            "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\debug\"
REM xcopy /Y "%ProjectPath%\output\debug\eLTE_SDK\pdb\*.pdb"                                        "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\debug\"
REM xcopy /Y "%ProjectPath%\output\debug\eLTE_SDK\ReadMe.txt"                                       "%ProjectPath%\..\..\..\sample\eLTE_SDK\eLTE_SDK_Demo\eLTE_SDK\debug\"
echo "--------- ��� eLTE SDK Demo Source  end  ---------"