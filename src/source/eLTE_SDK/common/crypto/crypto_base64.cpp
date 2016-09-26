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
#include "crypto_base64.h"
#include "openssl/evp.h"
// 带格式(换行)的Base64解码时, 每次解码的最大大小
// 每行Base64编码数据的长度64(个可视字符) + 1(换行符)
// 此变量作用域为当前文件
static const int CHARS_PER_LINE_OF_BASE64 = 64 + 1;


	bool CBase64::EncodeBinaryFormat(
	const unsigned char * pszInBuff, int iInLen, 
	unsigned char * pszOutBuff, int & iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 
		0 >= iInLen || iOutLen < EVP_ENCODE_LENGTH(iInLen) + 1) //lint !e834
	{
		return(false);
	}

	EVP_ENCODE_CTX eEncodeCTX;
	EVP_EncodeInit(&eEncodeCTX);

	int iEncodeLen = 0;

	iOutLen = 0;
	EVP_EncodeUpdate(&eEncodeCTX, pszOutBuff, &iEncodeLen, pszInBuff, iInLen);
	iOutLen += iEncodeLen;
	EVP_EncodeFinal(&eEncodeCTX, pszOutBuff + iOutLen, &iEncodeLen);
	iOutLen += iEncodeLen;

	return(true);
}

bool CBase64::DecodeBinaryFormat(
	const unsigned char * pszInBuff, int iInLen, 
	unsigned char * pszOutBuff, int & iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 
		0 >= iInLen || iOutLen < EVP_DECODE_LENGTH(iInLen) + 1) //lint !e834
	{
		return(false);
	}

	EVP_ENCODE_CTX eDecodeCTX;
	EVP_DecodeInit(&eDecodeCTX);

	int iRet = 0;
	int iOffset = 0;
	int iDecodeLen = 0;

	iOutLen = 0;

	while (iOffset < iInLen)
	{
		const int iUpdateLen = 
			(iInLen - iOffset) > CHARS_PER_LINE_OF_BASE64 
			? CHARS_PER_LINE_OF_BASE64 : (iInLen - iOffset);

		iRet = EVP_DecodeUpdate(
			&eDecodeCTX, pszOutBuff + iOutLen, &iDecodeLen, 
			pszInBuff + iOffset, iUpdateLen);
		if (0 > iRet)
		{
			pszOutBuff[0] = '\0';
			return(false);
		}

		iOutLen += iDecodeLen;
		iOffset += iUpdateLen;
	}

	iRet = EVP_DecodeFinal(&eDecodeCTX, pszOutBuff + iOutLen, &iDecodeLen);
	if (0 > iRet)
	{
		pszOutBuff[0] = '\0';
		return(false);
	}

	iOutLen += iDecodeLen;

	return(true);
}

bool CBase64::EncodeBinary(
	const unsigned char * pszInBuff, int iInLen, 
	unsigned char * pszOutBuff, int & iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 
		0 >= iInLen || iOutLen < GetEncodeBinarySize(iInLen))
	{
		return(false);
	}

	iOutLen = EVP_EncodeBlock(pszOutBuff, pszInBuff, iInLen);

	return(0 <= iOutLen);
}

bool CBase64::DecodeBinary(
	const unsigned char * pszInBuff, int iInLen, 
	unsigned char * pszOutBuff, int & iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 
		0 >= iInLen || iOutLen < GetDecodeBinarySize(iInLen))
	{
		return(false);
	}

	int iPadLen = 0;
	for (int i = iInLen - 1; i >= 0; --i)
	{
		if ('=' == pszInBuff[i])
		{
			++iPadLen;
		}
		else
		{
			break;
		}
	}

	iOutLen = EVP_DecodeBlock(pszOutBuff, pszInBuff, iInLen) - iPadLen;

	return(0 <= iOutLen);
}

bool CBase64::EncodeString(
	const char * pszInBuff, 
	char * pszOutBuff, int iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 1 > iOutLen)
	{
		return(false);
	}

	int iInLen = strlen(pszInBuff); //lint !e713

	// 空字符串编码还是空字符串
	if (0 == iInLen)
	{
		pszOutBuff[0] = '\0';
		return(true);
	}

	bool bEncode = EncodeBinary(
		reinterpret_cast<const unsigned char *>(pszInBuff), iInLen, 
		reinterpret_cast<unsigned char *>(pszOutBuff), iOutLen);

	if (!bEncode)
	{
		pszOutBuff[0] = '\0';
		return(false);
	}

	pszOutBuff[iOutLen] = '\0';

	return(true);
}

bool CBase64::DecodeString(
	const char * pszInBuff, 
	char * pszOutBuff, int iOutLen)
{
	if (NULL == pszInBuff || NULL == pszOutBuff || 1 > iOutLen)
	{
		return(false);
	}

	int iInLen = strlen(pszInBuff); //lint !e713

	// 空字符串解码还是空字符串
	if (0 == iInLen)
	{
		pszOutBuff[0] = '\0';
		return(true);
	}

	bool bDecode = DecodeBinary(
		reinterpret_cast<const unsigned char *>(pszInBuff), iInLen, 
		reinterpret_cast<unsigned char *>(pszOutBuff), iOutLen);

	if (!bDecode)
	{
		pszOutBuff[0] = '\0';
		return(false);
	}

	pszOutBuff[iOutLen] = '\0';

	return(true);
}

int CBase64::GetEncodeBinarySize(int iInLen)
{
	return(((iInLen + 2) / 3 * 4) + 1); //lint !e834
}

int CBase64::GetDecodeBinarySize(int iInLen)
{
	return(((iInLen + 3) / 4 * 3) + 1); //lint !e834
}

int CBase64::GetEncodeStringSize(const char * pszInBuff)
{
	if (NULL == pszInBuff)
	{
		return(0);
	}

	int iInLen = strlen(pszInBuff); //lint !e713

	return(GetEncodeBinarySize(iInLen) + 1);
}

int CBase64::GetDecodeStringSize(const char * pszInBuff)
{
	if (NULL == pszInBuff)
	{
		return(0);
	}

	int iInLen = strlen(pszInBuff); //lint !e713

	return(GetDecodeBinarySize(iInLen) + 1);
}
