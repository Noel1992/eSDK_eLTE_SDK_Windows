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

#ifndef _VT_CRYPTO_BASE_64_H_
#define _VT_CRYPTO_BASE_64_H_

/*********************************************************************
 * Base64编解码
 ********************************************************************/
class CBase64
{
public:
    /*****************************************************************
     * 二进制数据的Base64编码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64编码过后的数据, 会自动格式化(换行之类)输出数据
     * iOutLen:    编码过后的数据的长度
     * 返回值:      编码是否成功
     ****************************************************************/
    static bool EncodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  经过Base64编码过后的格式化(有换行之类)输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64解码过后的数据
     * iOutLen:    解码过后的数据的长度
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64编码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64编码过后的数据 (不会自动格式化)
     * iOutLen:    编码过后的数据的长度
     * 返回值:      编码是否成功
     ****************************************************************/
    static bool EncodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64解码过后的数据
     * iOutLen:    解码过后的数据的长度
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /******************************************************************
     * 字符串数据的Base64编码
     * pszInBuff:  字符串输入数据
     * pszOutBuff: 经过Base64编码过后的数据 (字符串)
     * iOutLen:    保存输出数据的缓冲大小
     * 返回值:      编码是否成功
     *****************************************************************/
    static bool EncodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  字符串输入数据
     * pszOutBuff: 经过Base64解码过后的数据 (字符串)
     * iOutLen:    保存输出数据的缓冲大小
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * 长度为inLen的二进制输入数据, 经过Base64编码后的输出数据所需占用的最大长度
     *****************************************************************/
    static int GetEncodeBinarySize(int iInLen);

    /******************************************************************
     * 长度为inLen的二进制输入数据, 经过Base64解码后的输出数据所需占用的最大长度
     *****************************************************************/
    static int GetDecodeBinarySize(int iInLen);

    /******************************************************************
     * 字符串输入数据, 经过Base64编码后的输出数据所需占用的最大长度(包括'\0')
     *****************************************************************/
    static int GetEncodeStringSize(const char * pszInBuff);

    /******************************************************************
     * 字符串输入数据, 经过Base64解码后的输出数据所需占用的最大长度(包括'\0')
     *****************************************************************/
    static int GetDecodeStringSize(const char * pszInBuff);
};

#endif

