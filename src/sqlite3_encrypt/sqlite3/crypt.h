#pragma once

/**

 * 加密函数

 */

int My_Encrypt_Func(unsigned char * pData, unsigned int data_len,

              unsigned char * key, unsigned int len_of_key);

 

/**

 * 解密函数

 */

int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len,

              unsigned char * key, unsigned int len_of_key);
