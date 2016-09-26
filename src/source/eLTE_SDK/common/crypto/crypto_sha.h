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
     description: 使用SHA-1加密方法加密
     input      : pszData --> 明文, iDataLen --> 明文长度
     output     : pszDigest --> 密文, iDigestLen -->密文长度
     return     : int, 成功返回0， 错误返回-1。
    *******************************************************************/
	int security_sha1( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

	/******************************************************************
     function   : security_sha1_hex
     description: 使用SHA-1加密方法加密, 并将加密结果转化为十六进制形式
     input      : pszData --> 明文, iDataLen --> 明文长度
     output     : pszDigest --> 密文, iDigestLen -->密文长度
     return     : int, 成功返回0， 错误返回-1。
    *******************************************************************/
	int security_sha1_hex( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

		/******************************************************************
     function   : security_sha256
     description: 使用SHA-256加密方法加密
     input      : pszData --> 明文, iDataLen --> 明文长度
     output     : pszDigest --> 密文, iDigestLen -->密文长度
     return     : int, 成功返回0， 错误返回-1。
    *******************************************************************/
	int security_sha256( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

	/******************************************************************
     function   : security_sha256_hex
     description: 使用SHA-256加密方法加密, 并将加密结果转化为十六进制形式
     input      : pszData --> 明文, iDataLen --> 明文长度
     output     : pszDigest --> 密文, iDigestLen -->密文长度
     return     : int, 成功返回0， 错误返回-1。
    *******************************************************************/
	int security_sha256_hex( const unsigned char * pszData, unsigned int iDataLen, 
		unsigned char * pszDigest, unsigned int& iDigestLen);

private:
	/******************************************************************
     function   : uchar_to_hexes
     description: 将一个字符转换为十六进制形式, 并以两个字符保存
     input      : ucIn --> 一个字符
     output     : szOut --> 两个字符(十六进制形式)
     return     : NA
    *******************************************************************/
	void uchar_to_hexes(unsigned char ucIn, unsigned char* szOut);

	/******************************************************************
     function   : uchar_to_hexes
     description: 将字符串转换成可打印的十六进制格式的字符串
     input      : pszInData --> 密文, iInDataLen --> 密文长度
     output     : szOut --> 十六进制格式的字符串
     return     : NA
    *******************************************************************/
	void binary_to_hexes(const unsigned char * pszInData, int iInDataLen, unsigned char * pszOutData);

};
#endif
