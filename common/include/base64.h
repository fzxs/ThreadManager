#ifndef __BASE64_H_
#define __BASE64_H_

#ifdef __cplusplus
extern "C"
{
#endif
	/********************************************************
	   Func Name: base64_encode_calculate
	Date Created: 2018-8-2
	 Description: 编码算法
		   Input: plaintext_in：源文件
					 length_in：源文件长度
		  Output:     code_out：生成编码文件
					length_out：生成编码文件的长度
		  Return: 
		 Caution: code_out内存由调用函数释放
	*********************************************************/
	int base64_encode(const char *plaintext_in, int length_in, char **code_out, int *length_out);

	/********************************************************
	   Func Name: base64_decode_calculate
	Date Created: 2018-8-3
	 Description: 解码算法
		   Input:       code_in;编码后的文件
					  length_in：编码后的文件长度
		  Output: plaintext_out：源文件
						 outlen：源文件长度
		  Return: 
		 Caution: plaintext_out内存由调用函数释放
	*********************************************************/
	int base64_decode(char *code_in, int length_in, char **plaintext_out, int *outlen);

#ifdef __cplusplus
}
#endif

#endif
