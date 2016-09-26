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

#ifndef _VT_CRYPTO_API_H_
#define _VT_CRYPTO_API_H_


/********************************************************************
*                           AES�ӽ��ܽӿ�
********************************************************************/
enum AES_PADDING_TYPE
{
	 NO_PADDING    = 0,  //�����
	 PKCS5_PADDING_ = 1,  //PKCS5��䷽ʽ
	 ECB_PADDING   = 2,	 //ECB��䷽ʽ
	 BCB_PADDING   = 3,	 //BCB��䷽ʽ
	 CFB_PADDING   = 4,  //CFB��䷽ʽ
	 OFB_PADDING   = 5   //OCF��䷽ʽ
};

/******************************************************************************
* Function ......:  AES_ENCRYPT
* Description ...:  AES���ܺ���
* @Parameter.....:  key         [in]:������Կ
* @Parameter.....:  bits        [in]:��Կλ��������Ϊ128��192��256����֮һ
* @Parameter.....:  padding_type[in]:�趨�������
* @Parameter.....:  in_buf      [in]:�����ַ���
* @Parameter.....:  in_buf_len  [in]:�����ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���
* @Parameter.....:  out_buf_len [in/out]:�����������С/���ܺ��ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int AES_ENCRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);
/******************************************************************************
* Function ......:  AES_DECRYPT
* Description ...:  AES���ܺ���
* @Parameter.....:  key         [in]:������Կ
* @Parameter.....:  bits        [in]:��Կλ��������Ϊ128��192��256����֮һ
* @Parameter.....:  padding_type[in]:�趨�������
* @Parameter.....:  in_buf      [in]:�����ܵ��ַ���
* @Parameter.....:  in_buf_len  [in]:�������ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���
* @Parameter.....:  out_buf_len [in/out]:�����������С/���ܺ��ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int AES_DECRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);



/********************************************************************
*                          BASE64�����ӿ�
********************************************************************/
enum BASE64_CODEC_TYPE{
	BINARY_FORMAT    = 0,	//�������������ݣ���Ҫָ���������ݳ���,�����ʽ��
	BINARY_NO_FORMAT = 1,	//�������������ݣ���Ҫָ���������ݳ���,�������ʽ��
	STRING           = 2   //�ַ����������ݣ�����Ҫָ���������ݳ���
};

/******************************************************************************
* Function ......:  BASE64_AES_ENCRYPT
* Description ...:  BASE64���뺯��
* @Parameter.....:  codec_type  [in]:����������������
* @Parameter.....:  in_buf      [in]:��������ַ���
* @Parameter.....:  in_buf_len  [in]:��������ַ�������(�ַ������Ϳ���д0)
* @Parameter.....:  out_buf     [out]:�������ַ���
* @Parameter.....:  out_buf_len [in/out]:�����������С/������ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int BASE64_ENCRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len);
/******************************************************************************
* Function ......:  BASE64_AES_DECRYPT
* Description ...:  BASE64���ܺ���
* @Parameter.....:  codec_type  [in]:����������������
* @Parameter.....:  in_buf      [in]:��������ַ���
* @Parameter.....:  in_buf_len  [in]:��������ַ�������(�ַ������Ϳ���д0)
* @Parameter.....:  out_buf     [out]:�������ַ���
* @Parameter.....:  out_buf_len [int/out]:�����������С/������ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int BASE64_DECRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len);


/********************************************************************
*                          SHA����ӿ�
********************************************************************/

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA���ܺ���
* @Parameter.....:  in_buf      [in]:�����ַ���
* @Parameter.....:  in_buf_len  [in]:�����ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���
* @Parameter.....:  out_buf_len     [out]:���ܺ���ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA1_ENCRYPT(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA���ܺ���
* @Parameter.....:  in_buf      [in]:�����ַ���
* @Parameter.....:  in_buf_len  [in]:�����ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���(ʮ������)
* @Parameter.....:  out_buf_len     [out]:���ܺ���ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA1_HEX_ENCRYPT(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA���ܺ���
* @Parameter.....:  in_buf      [in]:�����ַ���
* @Parameter.....:  in_buf_len  [in]:�����ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���
* @Parameter.....:  out_buf_len     [out]:���ܺ���ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA256_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA���ܺ���
* @Parameter.....:  in_buf      [in]:�����ַ���
* @Parameter.....:  in_buf_len  [in]:�����ַ�������
* @Parameter.....:  out_buf     [out]:���ܺ���ַ���(ʮ������)
* @Parameter.....:  out_buf_len     [out]:���ܺ���ַ�������
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA256_HEX_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

#endif
