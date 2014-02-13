#include "crypt.h"

#include "memory.h"

 

/***********

 关键加密函数

 ***********/

int My_Encrypt_Func(unsigned char * pData, unsigned int data_len,

              unsigned char * key, unsigned int len_of_key)

 

{

       unsigned int i;

       unsigned char bit, val;

 

       for (i = 0; i < data_len; i++)

       {

              val = ~(*pData);

              *pData = val;

              pData++;

       }

       return 0;

}

 

/***********

 关键解密函数

 ***********/

int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len,

              unsigned char * key, unsigned int len_of_key)

 

{

       unsigned int i;

       unsigned char bit, val;

 

       for (i = 0; i < data_len; i++)

       {

              val = ~(*pData);

              *pData = val;

              pData++;

       }

       return 0;

}
