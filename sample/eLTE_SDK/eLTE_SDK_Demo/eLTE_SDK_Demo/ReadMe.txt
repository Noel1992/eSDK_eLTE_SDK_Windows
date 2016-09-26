================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : eLTE_SDK_Demo Project Overview
===============================================================================

The application wizard has created this eLTE_SDK_Demo application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your eLTE_SDK_Demo application.

eLTE_SDK_Demo.vcxproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

eLTE_SDK_Demo.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

eLTE_SDK_Demo.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CeLTE_SDK_DemoApp application class.

eLTE_SDK_Demo.cpp
    This is the main application source file that contains the application
    class CeLTE_SDK_DemoApp.

eLTE_SDK_Demo.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\eLTE_SDK_Demo.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file eLTE_SDK_Demo.rc.

res\eLTE_SDK_Demo.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.


/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:

eLTE_SDK_DemoDlg.h, eLTE_SDK_DemoDlg.cpp - the dialog
    These files contain your CeLTE_SDK_DemoDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in eLTE_SDK_Demo.rc, which can be edited in Microsoft Visual C++.


/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named eLTE_SDK_Demo.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

eLTE_SDK_Demo.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC100XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////

V1.5.60
增加新功能：
1、	发起人工转接、发起缜密侦听、停止缜密侦听、发起环境侦听、调度台发起视频上墙、调度台终止视频上墙、获取视频上墙目的ID列表
2、	创建派接组、取消已创建派接组、增加派接组成员、删除派接组成员、获取指定调度台的所有派接组列表、获取指定派接组管理的成员列表、获取指定派接组属性信息
