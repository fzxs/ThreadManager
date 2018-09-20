
#ifndef __CRYPTO_RSA_H_
#define __CRYPTO_RSA_H_

/*
加密原理：
使用私钥加密，公钥解密，公钥可以解密，但是不能加密，公钥分给用户，所以用户可以知悉加密文件内容，但是无法生成加密文件

RSA加密常用的填充方式有下面3种：

1.RSA_PKCS1_PADDING 填充模式，最常用的模式

要求 :
输入：必须 比 RSA 钥模长(modulus) 短至少11个字节, 也就是　RSA_size(rsa) – 11
如果输入的明文过长，必须切割，　然后填充

输出：和modulus一样长

根据这个要求，对于512bit的密钥，　block length = 512 / 8 – 11 = 53 字节

2.RSA_PKCS1_OAEP_PADDING
输入：RSA_size(rsa) – 41

输出：和modulus一样长

3.f or RSA_NO_PADDING　　不填充

输入：可以和RSA钥模长一样长，如果输入的明文过长，必须切割，　然后填充

输出：和modulus一样长

跟DES，AES一样，　RSA也是一个块加密算法（ block cipher algorithm），总是在一个固定长度的块上进行操作。

但跟AES等不同的是，　block length是跟key length有关的。

*/

/*
openssl密钥生成命令

>>openssl genrsa -out prikey.pem 1024
>>openssl rsa -in prikey.pem -pubout -out pubkey.pem
*/

#ifdef __cplusplus
extern "C"
{
#endif

//加密
int rsa_encode(const char *pcPrikeyPath, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen);

//解密
int rsa_decode(const char *pcPubkeyPath, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen);

#ifdef __cplusplus
}
#endif

#endif








