#ifndef __BASE64_H_
#define __BASE64_H_

#ifdef __cplusplus
extern "C"
{
#endif
	/********************************************************
	   Func Name: base64_encode_calculate
	Date Created: 2018-8-2
	 Description: �����㷨
		   Input: plaintext_in��Դ�ļ�
					 length_in��Դ�ļ�����
		  Output:     code_out�����ɱ����ļ�
					length_out�����ɱ����ļ��ĳ���
		  Return: 
		 Caution: code_out�ڴ��ɵ��ú����ͷ�
	*********************************************************/
	int base64_encode(const char *plaintext_in, int length_in, char **code_out, int *length_out);

	/********************************************************
	   Func Name: base64_decode_calculate
	Date Created: 2018-8-3
	 Description: �����㷨
		   Input:       code_in;�������ļ�
					  length_in���������ļ�����
		  Output: plaintext_out��Դ�ļ�
						 outlen��Դ�ļ�����
		  Return: 
		 Caution: plaintext_out�ڴ��ɵ��ú����ͷ�
	*********************************************************/
	int base64_decode(char *code_in, int length_in, char **plaintext_out, int *outlen);

#ifdef __cplusplus
}
#endif

#endif
