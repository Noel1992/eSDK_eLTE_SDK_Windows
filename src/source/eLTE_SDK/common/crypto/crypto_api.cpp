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
#include "crypto_api.h"
#include "crypto_aes.h"
#include "crypto_base64.h"
#include "crypto_sha.h"

//lint -e30 -e408 -e142

int AES_ENCRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len)
{
	if(NULL == key || (AES_SECRET_KEY_128 != bits && AES_SECRET_KEY_192 != bits && AES_SECRET_KEY_256 != bits)
		|| NULL == in_buf || 0 == in_buf_len || NULL == out_buf)
	{
		return -1;
	}
	//初始化，设置秘钥和秘钥长度
	CAES aes(key, bits);
	int ret = 0;

	//根据用户选择的填充类型进加密
	switch (padding_type)
	{
	case NO_PADDING:
		break;
	case PKCS5_PADDING_://lint !e650
		ret = aes.aes_padding_encrypt(in_buf, in_buf_len, out_buf, out_buf_len);
		break;
	case ECB_PADDING:
		break;
	case BCB_PADDING:
		break;
	case CFB_PADDING:
		break;
	case OFB_PADDING:
		break;
	default:
		break;
	}

	return ret;
}

int AES_DECRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len)
{
	if(NULL == key || AES_SECRET_KEY_128 != bits && AES_SECRET_KEY_192 != bits && AES_SECRET_KEY_256 != bits
		|| NULL == in_buf || 0 == in_buf_len || NULL == out_buf)
	{
		return -1;
	}

	//初始化，设置秘钥和秘钥长度
	CAES aes(key, bits);
	int ret = 0;

	//根据用户选择的填充类型进加密
	switch (padding_type)
	{
	case NO_PADDING:
		break;
	case PKCS5_PADDING_://lint !e650
		ret = aes.aes_padding_decrypt(in_buf, in_buf_len, out_buf, out_buf_len);
		break;
	case ECB_PADDING:
		break;
	case BCB_PADDING:
		break;
	case CFB_PADDING:
		break;
	case OFB_PADDING:
		break;
	default:
		break;
	}

	return ret;
}

int BASE64_ENCRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}

	//根据用户选择相应的加密项
	int ret = 0;
	switch (codec_type)
	{
	case BINARY_FORMAT:
		break;
	case BINARY_NO_FORMAT:
		ret = CBase64::EncodeBinary(in_buf, in_buf_len, out_buf, out_buf_len);
		break;
	case STRING:
		break;
	default:
		break;
	}
	return ret;
}

int BASE64_DECRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}

	//根据用户选择相应的解密项
	int ret = 0;
	switch (codec_type)
	{
	case BINARY_FORMAT:
		break;
	case BINARY_NO_FORMAT:
		ret = CBase64::DecodeBinary(in_buf, in_buf_len, out_buf, out_buf_len);
		break;
	case STRING:
		break;
	default:
		break;
	}
	return ret;
}

int SHA1_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}
	//初始化
	CSHA sha;
	int ret = sha.security_sha1(in_buf, in_buf_len, out_buf, out_buf_len);
	return ret;
}

int SHA1_HEX_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}
	//初始化
	CSHA sha;
	int ret = sha.security_sha1_hex(in_buf, in_buf_len, out_buf, out_buf_len);
	return ret;
}

int SHA256_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}
	//初始化
	CSHA sha;
	int ret = sha.security_sha256(in_buf, in_buf_len, out_buf, out_buf_len);
	return ret;
}

int SHA256_HEX_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len)
{
	if (NULL == in_buf || NULL == out_buf)
	{
		return -1;
	}
	//初始化
	CSHA sha;
	int ret = sha.security_sha256_hex(in_buf, in_buf_len, out_buf, out_buf_len);
	return ret;
}
