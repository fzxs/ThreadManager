
#ifndef __UNCHARCODE_H_
#define __UNCHARCODE_H_

#ifdef __cplusplus
extern "C"
{
#endif

	/********************************************************
	   Func Name: getStringCode
	Date Created: 2018-8-3
	 Description: ��ȡ�ַ�������
		   Input: pcSrc��Դ��������
				 iLenIn��Դ���볤��
				 pcCode���������ڴ��ַ
		  Output:         
		  Return: error code
		 Caution: pcCode�ڴ���Ҫ�ɵ��ú����ͷ�
	*********************************************************/
	int getStringCode(const char *pcSrc, int iLenIn, char **pcCode);

	/********************************************************
	   Func Name: transcodeToUTF8
	Date Created: 2018-8-3
	 Description: ת��UTF8
		   Input: pcSrc��Դ��������
				  lenIn��Դ���볤��
				 pcDest���������ڴ��ַ��INOUT��
				 lenOut��ʣ���ڴ浥λ������INOUT��
			 pcCodeType��Դ��������
		  Output:         
		  Return: error code
		 Caution: pcDest�ڴ���Ҫ�ɵ��ú�������
	*********************************************************/
	int transcodeToUTF8(char *pcSrc, int lenIn, char *pcDest, int *lenOut,const char *pcCodeType);

	/********************************************************
	   Func Name: transcodeToGBK
	Date Created: 2018-8-3
	 Description: ת��GBK
		   Input: pcSrc��Դ��������
				  lenIn��Դ���볤��
				 pcDest���������ڴ��ַ��INOUT��
				 lenOut��ʣ���ڴ浥λ������INOUT��
			 pcCodeType��Դ��������
		  Output:         
		  Return: error code
		 Caution: pcDest�ڴ���Ҫ�ɵ��ú�������
	*********************************************************/
	int transcodeToGBK(char *pcSrc, int lenIn, char *pcDest, int *lenOut,const char *pcCodeType);

	/********************************************************
	   Func Name: getUTF8charSize
	Date Created: 2018-8-29
	 Description: �������ֽ�,��ȡutf8�ַ���ռ�ֽ���
		   Input: ch��utf8���ֽ�
		  Output:         
		  Return: utf8�ַ���ռ�ֽ���
		 Caution: 
	*********************************************************/
	int getUTF8charSize(unsigned char ch);

#ifdef __cplusplus
}
#endif

#endif








