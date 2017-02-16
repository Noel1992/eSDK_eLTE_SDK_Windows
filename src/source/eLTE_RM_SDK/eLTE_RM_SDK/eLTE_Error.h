/*
Copyright 2015 Huawei Technologies Co., Ltd. All rights reserved.
	   eSDK is licensed under the Apache License, Version 2.0 (the "License");
	   you may not use this file except in compliance with the License.
	   You may obtain a copy of the License at
	
	       http://www.apache.org/licenses/LICENSE-2.0

	
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

*/
/********************************************************************
filename    :    eLTE_Error.h
author      :    zWX229156
created     :    2015/10/12
description :    错误码定义
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2015/10/12 初始版本
*********************************************************************/

#ifndef __ELTE_ERROR_H__
#define __ELTE_ERROR_H__


//////////////////////////////////////////////////////////////////////////
// eLTE_RM_SDK 错误码定义 2135102XXX
typedef enum
{
	eLTE_SDK_ERR_SUCCESS = 0,					// 成功

	// 2135102001 - 2135102100 普通错误
	eLTE_SDK_ERR_FAILED = 2135102001,			// 通用失败
	eLTE_SDK_ERR_INVALID_PARAM,					// 非法入参
	eLTE_SDK_ERR_NULL_POINTER,					// 空指针错误
	eLTE_SDK_ERR_CREATE_OBJECT,                 // 创建对象失败
	eLTE_SDK_ERR_CREATE_DIRECTORY,              // 创建目录失败
	eLTE_SDK_ERR_LOG_INIT,                      // 日志模块初始化失败

	// 2135102101 - 2135102200 网络错误
	eLTE_SDK_ERR_WSA_START_UP = 2135102101,     // 初始化套结字动态库失败
	eLTE_SDK_ERR_CONNECT_SERVER,                // 连接服务器失败
	eLTE_SDK_ERR_SSL_CONNECT,                   // SSL连接失败
	eLTE_SDK_ERR_SEND_MSG,                      // 发送数据失败
	eLTE_SDK_ERR_WAIT_TIME_OUT,                 // 等待超时
	eLTE_SDK_ERR_SERVER_NOT_RUNNING,            // 服务未运行

	// 2135102201 - 2135102300 文件操作错误
	eLTE_SDK_ERR_CONFIG_FILE_LOAD = 2135102201,	// 加载ini配置文件失败
	eLTE_SDK_ERR_CONFIG_FILE_READ,				// 读取ini配置文件失败
	eLTE_SDK_ERR_CONFIG_FILE_WRITE,				// 写入ini配置文件失败

	//2135102301 - 2135102400 XML操作错误
	eLTE_SDK_ERR_XML_CONSTRUCT = 2135102301,	// 构造XML字符串失败
	eLTE_SDK_ERR_XML_PARSE,						// 解析XML字符串失败
	eLTE_SDK_ERR_XML_FIND_ELEM,					// 查找XML节点失败
	eLTE_SDK_ERR_XML_GET_ELEM_VALUE,		    // 获取XML节点失败


	eLTE_SDK_ERR_SERVICE_EXPIRE	= 2135103009   // 服务过期，客户端需要重启服务
} eLTE_RM_SDK_RET_ERROR_CODE_E;

//////////////////////////////////////////////////////////////////////////

#endif
