/*Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
eSDK is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
		http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

/********************************************************************
filename    :    eLTE_NativeService.h
author      :    yubinbin wx200189
created     :    2015/01/12
description :    服务程序入口，负责注册、开启服务，并开启业务监听
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/01/12 初始版本
*********************************************************************/

#ifndef __ELTE_NATIVE_SERVICE_H__
#define __ELTE_NATIVE_SERVICE_H__


#define INSTALL_AGRC		2
#define MY_SERVICE_NAME		"HUAWEI eSDK eLTE Service"
#define ELTE_LOG_PATH		".\\log\\"


void InstallService();
void UninstallService();
VOID WINAPI ServiceMain(DWORD dwNumServicesArgs, LPSTR *lpServiceArgVectors);
VOID WINAPI ControlHandler(DWORD dwControl);
void StartRun();

#endif
