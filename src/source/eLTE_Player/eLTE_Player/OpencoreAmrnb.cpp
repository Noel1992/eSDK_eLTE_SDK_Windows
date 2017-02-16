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

#include "StdAfx.h"
#include "OpencoreAmrnb.h"
#include "DataType.h"
#include "eLTE_Tool.h"
#include "eLTE_Log.h"


OpencoreAmrnb::OpencoreAmrnb(void)
	: m_fpDecoder_Interface_init(NULL)
	, m_fpDecoder_Interface_exit(NULL)
	, m_fpDecoder_Interface_Decode(NULL)
	, m_hModule(NULL)
	, m_pState(NULL)
	, m_pWav(NULL)
	, m_pOutBuf(NULL)
{
}


OpencoreAmrnb::~OpencoreAmrnb(void)
{
	try
	{
		// ȥ��ʼ��
		if (NULL != m_pState)
		{
			if (NULL != m_fpDecoder_Interface_exit)
			{
				m_fpDecoder_Interface_exit(m_pState);
			}
			m_pState = NULL;
		}

		// �ͷ�WAV buffer
		if (NULL != m_pWav)
		{
			delete[] m_pWav;
			m_pWav = NULL;
		}
		if (NULL != m_pOutBuf)
		{
			delete[] m_pOutBuf;
			m_pOutBuf = NULL;
		}

		// ж�ؿ�Դ��
		if (NULL != m_hModule)
		{
			(void)FreeLibrary(m_hModule);
			m_hModule = NULL;
			m_fpDecoder_Interface_init = NULL;
			m_fpDecoder_Interface_exit = NULL;
			m_fpDecoder_Interface_Decode = NULL;
		}
	}
	catch (...)
	{//lint !e1775
		m_pState = NULL;
		m_pWav = NULL;
		m_pOutBuf = NULL;
		m_hModule = NULL;
		m_fpDecoder_Interface_init = NULL;
		m_fpDecoder_Interface_exit = NULL;
		m_fpDecoder_Interface_Decode = NULL;
	}
}


int OpencoreAmrnb::Init()
{
	LOG_TRACE();

	// ����Ѿ���ʼ�������˳�
	(void)Exit();

	// ����DLL��
	std::string strDllPath = eLTE_Tool::GetOcxPathA();
	strDllPath.append(OPENCORE_AMR_MODULE_NAME);
	m_hModule = LoadLibraryA(strDllPath.c_str());
	if (NULL == m_hModule)
	{
		LOG_RUN_ERROR("LoadLibrary libopencore-amrnb-0.dll failed. errorcode = %d", GetLastError());
		return eLTE_ERR_AUDIO_DECODE;
	}

	// ��ȡDLL�ӿ�
	m_fpDecoder_Interface_init = (Decoder_Interface_init)GetProcAddress(m_hModule, "Decoder_Interface_init");
	if (NULL == m_fpDecoder_Interface_init)
	{
		LOG_RUN_ERROR("GetProcAddress Decoder_Interface_init failed.");
		return eLTE_ERR_AUDIO_DECODE;
	}
	m_fpDecoder_Interface_exit = (Decoder_Interface_exit)GetProcAddress(m_hModule, "Decoder_Interface_exit");
	if (NULL == m_fpDecoder_Interface_exit)
	{
		LOG_RUN_ERROR("GetProcAddress Decoder_Interface_exit failed.");
		return eLTE_ERR_AUDIO_DECODE;
	}
	m_fpDecoder_Interface_Decode = (Decoder_Interface_Decode)GetProcAddress(m_hModule, "Decoder_Interface_Decode");
	if (NULL == m_fpDecoder_Interface_Decode)
	{
		LOG_RUN_ERROR("GetProcAddress Decoder_Interface_Decode failed.");
		return eLTE_ERR_AUDIO_DECODE;
	}

	// ��ʼ��
	m_pState = m_fpDecoder_Interface_init();
	if (NULL == m_pState)
	{
		LOG_RUN_ERROR("Decoder_Interface_init failed.");
		return eLTE_ERR_AUDIO_DECODE;
	}

	// ����WAV buffer
	if (NULL == m_pWav)
	{
		m_pWav = new uint8_t[WAV_BUFFER_SIZE]; 
	}
	if (NULL == m_pOutBuf)
	{
		m_pOutBuf = new int16_t[OUT_BUFFER_SIZE];
	}

	return eLTE_ERR_SUCCESS;
}

int OpencoreAmrnb::Exit()
{
	LOG_TRACE();

	// ȥ��ʼ��
	if (NULL != m_pState)
	{
		if (NULL != m_fpDecoder_Interface_exit)
		{
			m_fpDecoder_Interface_exit(m_pState);
		}
		m_pState = NULL;
	}

	// �ͷ�WAV buffer
	if (NULL != m_pWav)
	{
		delete[] m_pWav;
		m_pWav = NULL;
	}
	if (NULL != m_pOutBuf)
	{
		delete[] m_pOutBuf;
		m_pOutBuf = NULL;
	}

	// ж�ؿ�Դ��
	if (NULL != m_hModule)
	{
		(void)FreeLibrary(m_hModule);
		m_hModule = NULL;
		m_fpDecoder_Interface_init = NULL;
		m_fpDecoder_Interface_exit = NULL;
		m_fpDecoder_Interface_Decode = NULL;
	}

	return eLTE_ERR_SUCCESS;
}

uint8_t* OpencoreAmrnb::DecodeAmrToWav(uint8_t* pAmr)
{
	// У��״̬
	if (NULL == m_fpDecoder_Interface_Decode
		|| NULL == m_pState
		|| NULL == m_pWav
		|| NULL == m_pOutBuf
		|| NULL == pAmr)
	{
		LOG_RUN_ERROR("null point.");
		return NULL;
	}

	// ��ʼ������
	eSDK_MEMSET(m_pWav, 0, sizeof(uint8_t)*WAV_BUFFER_SIZE);
	eSDK_MEMSET(m_pOutBuf, 0, sizeof(int16_t)*OUT_BUFFER_SIZE);

	// ����һ��AMR��Ƶ֡��PCM����
	m_fpDecoder_Interface_Decode(m_pState, pAmr, m_pOutBuf, 0);

	eSDK_MEMCPY(m_pWav, sizeof(uint8_t)*WAV_BUFFER_SIZE, m_pOutBuf, sizeof(uint8_t)*WAV_BUFFER_SIZE);

	// ����ʹ�ó�Ա�����������ڴ��ظ������ͷſ���
	return m_pWav;
}//lint !e818 
