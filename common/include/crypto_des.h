
#ifndef __CRYPTO_DES_H_
#define __CRYPTO_DES_H_

#ifdef __cplusplus
extern "C"
{
#endif

    //����
	int des_encode(const unsigned char *pcDesKey, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen);

    //����
	int des_decode(const unsigned char *pcDesKey, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen);

#ifdef __cplusplus
}
#endif

#endif

