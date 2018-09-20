
#ifndef __CRYPTO_AES_H_
#define __CRYPTO_AES_H_

#ifdef __cplusplus
extern "C"
{
#endif

	//º”√‹
	int aes_encode(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey
		, char *pcPlaintext, int iPlaintextLen, char **pcCiphertext, int *piCiphertextLen);

	//Ω‚√‹
	int aes_decode(const unsigned char *pcFirstKey, const unsigned char *pcSecondKey
		, char *pcCiphertext, int iCiphertextLen, char **pcPlaintext, int *piPlaintextLen);

#ifdef __cplusplus
}
#endif

#endif
