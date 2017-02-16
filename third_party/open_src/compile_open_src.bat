::执行之前openssl路径，且保证目录结构为开源软件包原始结构

@echo off

set OPENSSL_PATH=%cd%\openssl-1.0.2j\openssl-1.0.2j
set VCVARS32="%VS100COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
set OPENSSL_LIB=%cd%\openssl-1.0.2j\lib\windows
set TINYXML_PATH=%cd%\tinyxml\tinyxml
set TINYXML_LIB=%cd%\tinyxml\lib
set SDL_PATH=%cd%\sdl2-2.0.3\sdl2-2.0.3\VisualC
set SDL_LIB=%cd%\sdl2-2.0.3\release\x86\lib
set SDL_DLL=%cd%\sdl2-2.0.3\release\x86\dll
set JSON_PATH=%cd%\jsoncpp-0.10.5\source\makefiles\msvc2010
set JSON_LIB=%cd%\jsoncpp-0.10.5\lib\win32_x86_msvc\release
set JSON_DLL=%cd%\jsoncpp-0.10.5\bin\win32_x86_msvc\release

@echo －－－－－－－－－－－－－－－－请确保已成功安装perl－－－－－－－－－－－－－－－－

@echo .
@echo －－－－－－－－－－－－－－－－编译openssl Start－－－－－－－－－－－－－－－－
@echo .
call %VCVARS32%
cd %OPENSSL_PATH%
perl Configure VC-WIN32 no-asm
call "ms/do_ms.bat"
nmake -f ms/nt.mak

xcopy /Y "out32\ssleay32.lib" 				"%OPENSSL_LIB%\"
xcopy /Y "out32\libeay32.lib" 				"%OPENSSL_LIB%\"
@echo .
@echo －－－－－－－－－－－－－－－－编译openssl End－－－－－－－－－－－－－－－－

@echo .
@echo －－－－－－－－－－－－－－－－编译tinyxml Start－－－－－－－－－－－－－－－－
@echo .
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%TINYXML_PATH%\tinyxml.sln" /Rebuild "Release|Win32" 

xcopy /Y "%TINYXML_PATH%\Releasetinyxml\tinyxml.lib" 				"%TINYXML_LIB%\"
@echo .
@echo －－－－－－－－－－－－－－－－编译tinyxml End－－－－－－－－－－－－－－－－

@echo .
@echo －－－－－－－－－－－－－－－－编译SDL Start－－－－－－－－－－－－－－－－
@echo .
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%SDL_PATH%\SDL_VS2010.sln" /Rebuild "Release|Win32" 

xcopy /Y "%SDL_PATH%\SDL\Win32\Release\SDL2.lib" 				"%SDL_LIB%\"
xcopy /Y "%SDL_PATH%\SDL\Win32\Release\SDL2.dll" 				"%SDL_DLL%\"
@echo .
@echo －－－－－－－－－－－－－－－－编译SDL End－－－－－－－－－－－－－－－－

@echo .
@echo －－－－－－－－－－－－－－－－编译json Start－－－－－－－－－－－－－－－－
@echo .
@"%VS100COMNTOOLS%\..\IDE\devenv.com" "%JSON_PATH%\jsoncpp.sln" /Rebuild "Release|Win32" 

xcopy /Y "%JSON_PATH%\Release\lib_json.lib" 				    "%JSON_LIB%\"
xcopy /Y "%JSON_PATH%\Release\lib_json.dll" 					"%JSON_DLL%\"
xcopy /Y "%JSON_PATH%\Release\lib_json.pdb" 					"%JSON_DLL%\"
@echo .
@echo －－－－－－－－－－－－－－－－编译json End－－－－－－－－－－－－－－－－

pause