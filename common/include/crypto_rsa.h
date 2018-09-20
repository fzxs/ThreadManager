
#ifndef __CRYPTO_RSA_H_
#define __CRYPTO_RSA_H_

/*
����ԭ��
ʹ��˽Կ���ܣ���Կ���ܣ���Կ���Խ��ܣ����ǲ��ܼ��ܣ���Կ�ָ��û��������û�����֪Ϥ�����ļ����ݣ������޷����ɼ����ļ�

RSA���ܳ��õ���䷽ʽ������3�֣�

1.RSA_PKCS1_PADDING ���ģʽ����õ�ģʽ

Ҫ�� :
���룺���� �� RSA Կģ��(modulus) ������11���ֽ�, Ҳ���ǡ�RSA_size(rsa) �C 11
�����������Ĺ����������и��Ȼ�����

�������modulusһ����

�������Ҫ�󣬶���512bit����Կ����block length = 512 / 8 �C 11 = 53 �ֽ�

2.RSA_PKCS1_OAEP_PADDING
���룺RSA_size(rsa) �C 41

�������modulusһ����

3.f or RSA_NO_PADDING���������

���룺���Ժ�RSAԿģ��һ�����������������Ĺ����������и��Ȼ�����

�������modulusһ����

��DES��AESһ������RSAҲ��һ��������㷨�� block cipher algorithm����������һ���̶����ȵĿ��Ͻ��в�����

����AES�Ȳ�ͬ���ǣ���block length�Ǹ�key length�йصġ�

*/

/*
openssl��Կ��������

>>openssl genrsa -out prikey.pem 1024
>>openssl rsa -in prikey.pem -pubout -out pubkey.pem
*/

#ifdef __cplusplus
extern "C"
{
#endif

//����
int rsa_encode(const char *pcPrikeyPath, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen);

//����
int rsa_decode(const char *pcPubkeyPath, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen);

#ifdef __cplusplus
}
#endif

#endif








