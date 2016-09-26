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

#ifndef _VT_CRYPTO_AES_H_
#define _VT_CRYPTO_AES_H_

#define AES_SECRET_KEY_128 128	//128位秘钥
#define AES_SECRET_KEY_192 192	//192位秘钥
#define AES_SECRET_KEY_256 256	//256位秘钥

class CAES
{
public:
	CAES();
	CAES(unsigned char *key, unsigned int bits);
	virtual ~CAES();
	/**************************************************************************
	* name			: aes_padding_encrypt
	* description: 先采用java的PKCS5Padding进行字符填充、再采用AES加密
	* input			: in_buf 明文
						: in_buf_len 明文长度
	* output		: out_buf 密文
						: out_buf_len 密文长度
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	int aes_padding_encrypt(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);

	/**************************************************************************
	* name			: aes_padding_encrypt
	* description: 先采用AES解密、在进行填充字符的过滤
	* input			: in_buf 密文
						: in_buf_len 密文长度
	* output		: out_buf 明文
						: out_buf_len 明文长度
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	int aes_padding_decrypt(unsigned char* in_buf, unsigned int in_buf_len, unsigned char* out_buf, unsigned int &out_buf_len);

private:
	void* m_aes_encrypt_key;
	void* m_aes_decrypt_key;
};
#endif
