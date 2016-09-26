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

// SHA_1.cpp : ���� ʹ��SHA-1���ܷ������ܡ�
//

#include "stdafx.h"
#include "crypto_sha.h"
#include "eLTE_Error.h"
#include "eLTE_Xml.h"
#include <openssl/sha.h>

//lint -e1762

CSHA::CSHA()
{
}

CSHA::~CSHA()
{

}

int CSHA::security_sha1(
						 const unsigned char * pszData, unsigned int iDataLen, 
						 unsigned char * pszDigest, unsigned int& iDigestLen)
{
	CHECK_POINTER(pszData, -1);
	CHECK_POINTER(pszDigest, -1);
	if (0 == iDataLen)
	{
		return -1;
	}

	// ����Ҳ������OpenSSL�ĺ���:
	// SHA1(pszData, iDataLen, pszDigest), 
	// �滻�������������, ���ǲ��Ǳ�׼����, 
	// ��ΪSHA1_Update()�����������ö��, 
	// ����, ��pszData��ɶ���Ӵ��ֱ����SHA1_Update(), 
	// ���һ�ε���SHA1_Update(), ��õ�һ���Ľ��

	SHA_CTX aSHACTX;

	// ����ժҪ����
	if (SHA1_Init(&aSHACTX) <= 0 || 
		SHA1_Update(&aSHACTX, pszData, iDataLen) <= 0 || //lint !e732
		SHA1_Final(pszDigest, &aSHACTX) <= 0)
	{
		return -1;
	}
	iDigestLen = SHA_DIGEST_LENGTH;
	return 0;
}

int CSHA::security_sha1_hex(
							 const unsigned char * pszData, unsigned int iDataLen, 
							 unsigned char * pszDigestHex, unsigned int& iDigestLen)
{
	CHECK_POINTER(pszData, -1);
	CHECK_POINTER(pszDigestHex, -1);
	if (0 == iDataLen)
	{
		return -1;
	}

	// ����Ҳ������OpenSSL�ĺ���:
	// SHA1(pszData, iDataLen, szDigest), 
	// �滻�������������, ���ǲ��Ǳ�׼����, 
	// ��ΪSHA1_Update()�����������ö��, 
	// ����, ��pszData��ɶ���Ӵ��ֱ����SHA1_Update(), 
	// ���һ�ε���SHA1_Update(), ��õ�һ���Ľ��

	SHA_CTX aSHACTX;
	unsigned char szDigest[SHA_DIGEST_LENGTH];

	// ����ժҪ����
	if (SHA1_Init(&aSHACTX) <= 0 || 
		SHA1_Update(&aSHACTX, pszData, iDataLen) <= 0 || //lint !e732
		SHA1_Final(szDigest, &aSHACTX) <= 0)
	{
		pszDigestHex[0] = '\0';
		return -1;
	}

	// ת���ɿɴ�ӡ��ʮ�����Ƹ�ʽ
	binary_to_hexes(szDigest, SHA_DIGEST_LENGTH, pszDigestHex);

	pszDigestHex[2 * SHA_DIGEST_LENGTH] = '\0';

	iDigestLen = 2 * SHA_DIGEST_LENGTH + 1;
	return 0;
}

int CSHA::security_sha256(
							  const unsigned char * pszData, unsigned int iDataLen, 
							  unsigned char * pszDigest, unsigned int& iDigestLen)
{
	CHECK_POINTER(pszData, -1);
	CHECK_POINTER(pszDigest, -1);
	if (0 == iDataLen)
	{
		return -1;
	}

	// ����Ҳ������OpenSSL�ĺ���:
	// SHA256(pszData, iDataLen, pszDigest), 
	// �滻�������������, ���ǲ��Ǳ�׼����, 
	// ��ΪSHA256_Update()�����������ö��, 
	// ����, ��pszData��ɶ���Ӵ��ֱ����SHA256_Update(), 
	// ���һ�ε���SHA256_Update(), ��õ�һ���Ľ��

	SHA256_CTX aSHACTX;

	// ����ժҪ����
	if (SHA256_Init(&aSHACTX) <= 0 || 
		SHA256_Update(&aSHACTX, pszData, iDataLen) <= 0 || //lint !e732
		SHA256_Final(pszDigest, &aSHACTX) <= 0)
	{
		return -1;
	}
	iDigestLen = SHA256_DIGEST_LENGTH;
	return 0;
}

int CSHA::security_sha256_hex(
								  const unsigned char * pszData, unsigned int iDataLen, 
								  unsigned char * pszDigestHex, unsigned int& iDigestLen)
{
	CHECK_POINTER(pszData, -1);
	CHECK_POINTER(pszDigestHex, -1);
	if (0 == iDataLen)
	{
		return -1;
	}

	// ����Ҳ������OpenSSL�ĺ���:
	// SHA256(pszData, iDataLen, szDigest), 
	// �滻�������������, ���ǲ��Ǳ�׼����, 
	// ��ΪSHA256_Update()�����������ö��, 
	// ����, ��pszData��ɶ���Ӵ��ֱ����SHA256_Update(), 
	// ���һ�ε���SHA256_Update(), ��õ�һ���Ľ��

	SHA256_CTX aSHACTX;
	unsigned char szDigest[SHA256_DIGEST_LENGTH];

	// ����ժҪ����
	if (SHA256_Init(&aSHACTX) <= 0 || 
		SHA256_Update(&aSHACTX, pszData, iDataLen) <= 0 || //lint !e732
		SHA256_Final(szDigest, &aSHACTX) <= 0)
	{
		pszDigestHex[0] = '\0';
		return -1;
	}

	// ת���ɿɴ�ӡ��ʮ�����Ƹ�ʽ
	binary_to_hexes(szDigest, SHA256_DIGEST_LENGTH, pszDigestHex);

	pszDigestHex[2 * SHA256_DIGEST_LENGTH] = '\0';

	iDigestLen = 2 * SHA256_DIGEST_LENGTH + 1;
	return 0;
}


void CSHA::binary_to_hexes(
						  const unsigned char * pszInData, int iInDataLen, unsigned char * pszOutData)
{
	CHECK_POINTER_VOID(pszInData);
	CHECK_POINTER_VOID(pszOutData);
	if (0 >= iInDataLen)
	{
		return;
	}

	for (int i = 0; i < iInDataLen; ++i)
	{
		uchar_to_hexes(pszInData[i], pszOutData + 2 * i);
	}
}

void CSHA::uchar_to_hexes(
						 unsigned char ucIn, unsigned char* szOut)
{
	CHECK_POINTER_VOID(szOut);

	static const unsigned char Number[] = 
	{
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'  
	};

	szOut[0] = Number[(ucIn & 0xF0) >> 4];
	szOut[1] = Number[(ucIn & 0x0F)];
}


