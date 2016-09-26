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

#include "stdafx.h"
#include "crypto_aes.h"
#include "eLTE_Xml.h"
#include <openssl/aes.h>

#define PADDING_MIN_VALUE 1		//填充字段最小值
#define PADDING_MAX_VALUE 16	//填充字段最大值


CAES::CAES(unsigned char *key, unsigned int bits)
{
	if (NULL == key || (AES_SECRET_KEY_128 != bits 
		            && AES_SECRET_KEY_192 != bits 
		            && AES_SECRET_KEY_256 != bits))
	{
		m_aes_encrypt_key = NULL;
		m_aes_decrypt_key = NULL;
		return;
	}
	m_aes_encrypt_key = NULL;
	SAFE_NEW(m_aes_encrypt_key, AES_KEY);
	m_aes_decrypt_key = NULL;
	SAFE_NEW(m_aes_decrypt_key, AES_KEY);
	AES_set_encrypt_key(key, (int)bits, (AES_KEY*)m_aes_encrypt_key);
	AES_set_decrypt_key(key, (int)bits, (AES_KEY*)m_aes_decrypt_key);
} //lint !e818

CAES::~CAES()
{
	if(m_aes_encrypt_key)
	{
		delete (AES_KEY*)m_aes_encrypt_key;
		m_aes_encrypt_key = NULL;
	}
	if(m_aes_decrypt_key)
	{
		delete (AES_KEY*)m_aes_decrypt_key;
		m_aes_decrypt_key = NULL;
	}
}

int CAES::aes_padding_encrypt(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len)
{
	if (NULL == in_buf || 0 == in_buf_len && NULL == out_buf)
	{
		return -1;
	}

	// 采用java的PKCS5Padding填充方式:
	unsigned int add_len = (AES_BLOCK_SIZE - in_buf_len) % AES_BLOCK_SIZE;
	unsigned char padding_buf[1000];
	unsigned int padding_buf_len = 0;
	strncpy_s((char *)padding_buf, 1000, (char *)in_buf, in_buf_len);

	unsigned char *pBegin = NULL;
	unsigned char *pEnd   = NULL;
	if (0 != add_len) // 1.填充的原则是，如果长度少于16个字节，需要补满16个字节，补(16-len)个(16-len)
	{
		padding_buf_len = in_buf_len + add_len;
		pBegin = padding_buf + in_buf_len;
		pEnd   = padding_buf + padding_buf_len;
		while(pBegin != pEnd)
		{
			*pBegin++ = (unsigned char)add_len;
		}
	}
	else// 2.如果字符串长度正好是16字节，则需要再补16个字节的十进制的16。
	{
		padding_buf_len = in_buf_len + AES_BLOCK_SIZE;
		pBegin = padding_buf + in_buf_len;
		pEnd   = padding_buf + padding_buf_len;
		while(pBegin != pEnd)
		{
			*pBegin++ = AES_BLOCK_SIZE;
		}
	}

	if (out_buf_len < padding_buf_len)//输出缓冲区太小
	{
		return -1;
	}
	//开始采用AES加密
	const unsigned char * ptr = padding_buf;
	const unsigned char * ptrEnd = ptr + padding_buf_len;
	for(; ptr < ptrEnd; ptr += AES_BLOCK_SIZE, out_buf += AES_BLOCK_SIZE) //lint !e662 内部函数，保证不越界
	{ 
		AES_encrypt(ptr, out_buf, (AES_KEY*)m_aes_encrypt_key);
	}
	out_buf_len = padding_buf_len;

	return 0;
}//lint !e818 !e1762

int CAES::aes_padding_decrypt(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len)
{
	if (NULL == in_buf || 0 == in_buf_len || NULL == out_buf)
	{
		return -1;
	}
	
	//不是16的倍数或者输出缓冲区太小
	if ((in_buf_len & 0x0F) != 0 || out_buf_len < in_buf_len)
	{
		return -1;
	}

	//AES解码
	unsigned char * ptr_begin = in_buf;
	unsigned char * ptr_end   = ptr_begin + in_buf_len;
	unsigned char * out_begin = out_buf;
	for(; ptr_begin < ptr_end; ptr_begin += AES_BLOCK_SIZE, out_begin += AES_BLOCK_SIZE) //lint !e662 内部函数，保证不越界
	{
		AES_decrypt(ptr_begin, out_begin, (AES_KEY*)m_aes_decrypt_key);
	}
	out_buf_len = in_buf_len;

	//过滤掉填充的字段
	ptr_begin = out_buf;
	ptr_end = out_buf + out_buf_len;
	while (ptr_end > ptr_begin)
	{
		--ptr_end;
		if (*ptr_end >= PADDING_MIN_VALUE && *ptr_end <= PADDING_MAX_VALUE)
		{
			*ptr_end = '\0';
		}
		else
		{
			break;
		}
		out_buf_len--;
	}
	return 0;
}//lint !e1762
