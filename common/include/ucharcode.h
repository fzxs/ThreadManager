
#ifndef __UNCHARCODE_H_
#define __UNCHARCODE_H_

#ifdef __cplusplus
extern "C"
{
#endif

	/********************************************************
	   Func Name: getStringCode
	Date Created: 2018-8-3
	 Description: 获取字符串编码
		   Input: pcSrc：源编码数据
				 iLenIn：源编码长度
				 pcCode：结果存放内存地址
		  Output:         
		  Return: error code
		 Caution: pcCode内存需要由调用函数释放
	*********************************************************/
	int getStringCode(const char *pcSrc, int iLenIn, char **pcCode);

	/********************************************************
	   Func Name: transcodeToUTF8
	Date Created: 2018-8-3
	 Description: 转码UTF8
		   Input: pcSrc：源编码数据
				  lenIn：源编码长度
				 pcDest：结果存放内存地址（INOUT）
				 lenOut：剩余内存单位个数（INOUT）
			 pcCodeType：源编码类型
		  Output:         
		  Return: error code
		 Caution: pcDest内存需要由调用函数分配
	*********************************************************/
	int transcodeToUTF8(char *pcSrc, int lenIn, char *pcDest, int *lenOut,const char *pcCodeType);

	/********************************************************
	   Func Name: transcodeToGBK
	Date Created: 2018-8-3
	 Description: 转码GBK
		   Input: pcSrc：源编码数据
				  lenIn：源编码长度
				 pcDest：结果存放内存地址（INOUT）
				 lenOut：剩余内存单位个数（INOUT）
			 pcCodeType：源编码类型
		  Output:         
		  Return: error code
		 Caution: pcDest内存需要由调用函数分配
	*********************************************************/
	int transcodeToGBK(char *pcSrc, int lenIn, char *pcDest, int *lenOut,const char *pcCodeType);

	/********************************************************
	   Func Name: getUTF8charSize
	Date Created: 2018-8-29
	 Description: 根据首字节,获取utf8字符所占字节数
		   Input: ch：utf8首字节
		  Output:         
		  Return: utf8字符所占字节数
		 Caution: 
	*********************************************************/
	int getUTF8charSize(unsigned char ch);

#ifdef __cplusplus
}
#endif

#endif








