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

#ifndef _VT_CRYPTO_SHA_H_
#define _VT_CRYPTO_SHA_H_



class CSHA
{
public:
	explicit CSHA();
	virtual ~CSHA();
public:
	/******************************************************************
     function   : security_sha1
     description: ʹ��SHA-1���ܷ�������
     input      : pszData --> ����, iDataLen --> ���ĳ���
     output     : pszDigest --> ����, iDigestLen -->���ĳ���
     return     : int, �ɹ�����0�� ���󷵻�-1��
    *******************************************************************/
	int security_sha1( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

	/******************************************************************
     function   : security_sha1_hex
     description: ʹ��SHA-1���ܷ�������, �������ܽ��ת��Ϊʮ��������ʽ
     input      : pszData --> ����, iDataLen --> ���ĳ���
     output     : pszDigest --> ����, iDigestLen -->���ĳ���
     return     : int, �ɹ�����0�� ���󷵻�-1��
    *******************************************************************/
	int security_sha1_hex( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

		/******************************************************************
     function   : security_sha256
     description: ʹ��SHA-256���ܷ�������
     input      : pszData --> ����, iDataLen --> ���ĳ���
     output     : pszDigest --> ����, iDigestLen -->���ĳ���
     return     : int, �ɹ�����0�� ���󷵻�-1��
    *******************************************************************/
	int security_sha256( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

	/******************************************************************
     function   : security_sha256_hex
     description: ʹ��SHA-256���ܷ�������, �������ܽ��ת��Ϊʮ��������ʽ
     input      : pszData --> ����, iDataLen --> ���ĳ���
     output     : pszDigest --> ����, iDigestLen -->���ĳ���
     return     : int, �ɹ�����0�� ���󷵻�-1��
    *******************************************************************/
	int security_sha256_hex( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

private:
	/******************************************************************
     function   : uchar_to_hexes
     description: ��һ���ַ�ת��Ϊʮ��������ʽ, ���������ַ�����
     input      : ucIn --> һ���ַ�
     output     : szOut --> �����ַ�(ʮ��������ʽ)
     return     : NA
    *******************************************************************/
	void uchar_to_hexes(unsigned char ucIn, unsigned char* szOut);

	/******************************************************************
     function   : uchar_to_hexes
     description: ���ַ���ת���ɿɴ�ӡ��ʮ�����Ƹ�ʽ���ַ���
     input      : pszInData --> ����, iInDataLen --> ���ĳ���
     output     : szOut --> ʮ�����Ƹ�ʽ���ַ���
     return     : NA
    *******************************************************************/
	void binary_to_hexes(const unsigned char * pszInData, int iInDataLen, unsigned char * pszOutData);

};
#endif
