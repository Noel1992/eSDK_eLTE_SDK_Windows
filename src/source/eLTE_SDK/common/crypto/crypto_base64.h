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
 * Base64�����
 ********************************************************************/
class CBase64
{
public:
    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������, ���Զ���ʽ��(����֮��)�������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     ****************************************************************/
    static bool EncodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ����Base64�������ĸ�ʽ��(�л���֮��)��������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������ (�����Զ���ʽ��)
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     ****************************************************************/
    static bool EncodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /******************************************************************
     * �ַ������ݵ�Base64����
     * pszInBuff:  �ַ�����������
     * pszOutBuff: ����Base64������������ (�ַ���)
     * iOutLen:    ����������ݵĻ����С
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool EncodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  �ַ�����������
     * pszOutBuff: ����Base64������������ (�ַ���)
     * iOutLen:    ����������ݵĻ����С
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * ����ΪinLen�Ķ�������������, ����Base64�����������������ռ�õ���󳤶�
     *****************************************************************/
    static int GetEncodeBinarySize(int iInLen);

    /******************************************************************
     * ����ΪinLen�Ķ�������������, ����Base64�����������������ռ�õ���󳤶�
     *****************************************************************/
    static int GetDecodeBinarySize(int iInLen);

    /******************************************************************
     * �ַ�����������, ����Base64�����������������ռ�õ���󳤶�(����'\0')
     *****************************************************************/
    static int GetEncodeStringSize(const char * pszInBuff);

    /******************************************************************
     * �ַ�����������, ����Base64�����������������ռ�õ���󳤶�(����'\0')
     *****************************************************************/
    static int GetDecodeStringSize(const char * pszInBuff);
};

#endif

