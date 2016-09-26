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
*                           AES加解密接口
********************************************************************/
enum AES_PADDING_TYPE
{
	 NO_PADDING    = 0,  //无填充
	 PKCS5_PADDING_ = 1,  //PKCS5填充方式
	 ECB_PADDING   = 2,	 //ECB填充方式
	 BCB_PADDING   = 3,	 //BCB填充方式
	 CFB_PADDING   = 4,  //CFB填充方式
	 OFB_PADDING   = 5   //OCF填充方式
};

/******************************************************************************
* Function ......:  AES_ENCRYPT
* Description ...:  AES加密函数
* @Parameter.....:  key         [in]:加密秘钥
* @Parameter.....:  bits        [in]:秘钥位数，必须为128、192、256其中之一
* @Parameter.....:  padding_type[in]:设定填充类型
* @Parameter.....:  in_buf      [in]:加密字符串
* @Parameter.....:  in_buf_len  [in]:加密字符串长度
* @Parameter.....:  out_buf     [out]:加密后的字符串
* @Parameter.....:  out_buf_len [in/out]:输出缓冲区大小/机密后字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int AES_ENCRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);
/******************************************************************************
* Function ......:  AES_DECRYPT
* Description ...:  AES解密函数
* @Parameter.....:  key         [in]:解密秘钥
* @Parameter.....:  bits        [in]:秘钥位数，必须为128、192、256其中之一
* @Parameter.....:  padding_type[in]:设定填充类型
* @Parameter.....:  in_buf      [in]:待解密的字符串
* @Parameter.....:  in_buf_len  [in]:待解密字符串长度
* @Parameter.....:  out_buf     [out]:解密后的字符串
* @Parameter.....:  out_buf_len [in/out]:输出缓冲区大小/解密后字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int AES_DECRYPT(unsigned char *key, unsigned int bits, AES_PADDING_TYPE padding_type,
	unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);



/********************************************************************
*                          BASE64编解码接口
********************************************************************/
enum BASE64_CODEC_TYPE{
	BINARY_FORMAT    = 0,	//二进制输入数据，需要指定输入数据长度,输出格式化
	BINARY_NO_FORMAT = 1,	//二进制输入数据，需要指定输入数据长度,输出不格式化
	STRING           = 2   //字符串输入数据，不需要指定输入数据长度
};

/******************************************************************************
* Function ......:  BASE64_AES_ENCRYPT
* Description ...:  BASE64编码函数
* @Parameter.....:  codec_type  [in]:编解码输入输出类型
* @Parameter.....:  in_buf      [in]:待编码的字符串
* @Parameter.....:  in_buf_len  [in]:待编码的字符串长度(字符串类型可填写0)
* @Parameter.....:  out_buf     [out]:编码后的字符串
* @Parameter.....:  out_buf_len [in/out]:输出缓冲区大小/编码后字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int BASE64_ENCRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len);
/******************************************************************************
* Function ......:  BASE64_AES_DECRYPT
* Description ...:  BASE64解密函数
* @Parameter.....:  codec_type  [in]:编解码输入输出类型
* @Parameter.....:  in_buf      [in]:待解码的字符串
* @Parameter.....:  in_buf_len  [in]:待解码的字符串长度(字符串类型可填写0)
* @Parameter.....:  out_buf     [out]:解码后的字符串
* @Parameter.....:  out_buf_len [int/out]:输出缓冲区大小/解码后字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/09/18
* Author ........:  r00226606
*******************************************************************************/
int BASE64_DECRYPT(BASE64_CODEC_TYPE codec_type, 
	const unsigned char *in_buf, int in_buf_len, unsigned char* out_buf, int &out_buf_len);


/********************************************************************
*                          SHA编码接口
********************************************************************/

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA加密函数
* @Parameter.....:  in_buf      [in]:加密字符串
* @Parameter.....:  in_buf_len  [in]:加密字符串长度
* @Parameter.....:  out_buf     [out]:加密后的字符串
* @Parameter.....:  out_buf_len     [out]:加密后的字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA1_ENCRYPT(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA加密函数
* @Parameter.....:  in_buf      [in]:加密字符串
* @Parameter.....:  in_buf_len  [in]:加密字符串长度
* @Parameter.....:  out_buf     [out]:加密后的字符串(十六进制)
* @Parameter.....:  out_buf_len     [out]:加密后的字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA1_HEX_ENCRYPT(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA加密函数
* @Parameter.....:  in_buf      [in]:加密字符串
* @Parameter.....:  in_buf_len  [in]:加密字符串长度
* @Parameter.....:  out_buf     [out]:加密后的字符串
* @Parameter.....:  out_buf_len     [out]:加密后的字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA256_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

/******************************************************************************
* Function ......:  SHA1_ENCRYPT
* Description ...:  SHA加密函数
* @Parameter.....:  in_buf      [in]:加密字符串
* @Parameter.....:  in_buf_len  [in]:加密字符串长度
* @Parameter.....:  out_buf     [out]:加密后的字符串(十六进制)
* @Parameter.....:  out_buf_len     [out]:加密后的字符串长度
* Returns .......:  int: 0-success negative-failed
* Creation Date .:  2014/11/6
* Author ........:  zWX229156
*******************************************************************************/
int SHA256_HEX_ENCRYPT(const unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int& out_buf_len);

#endif
