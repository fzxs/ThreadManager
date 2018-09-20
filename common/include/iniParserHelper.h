
#ifndef __INIPARSERHELPER_H_
#define __INIPARSERHELPER_H_

/*ini文件解析函数*/

#include "iniparser.h"

typedef enum tagEMIniPraseType
{
	INIP_STR = 0,
	INIP_INT,
	INIP_DOUBLE,
	INIP_BOOL,
}EMIniPraseType;

typedef struct tagSTIniParserResult
{
	char * pcStrResult;
	int iIntResult;
	double dDoubleResult;
	int iBoolResult;
	int iErrorCode;
}STIniParserResult;

#ifdef __cplusplus
extern "C"
{
#endif

	//初始化
	int initIniParser(const char *pcFileName , dictionary **dicIni);

	//获取section的个数
	int getSectionCount(dictionary *dicIni, int *iSenctionCount);

	//获取指定位置的section的值
	int getSectionByIndex(dictionary *dicIni, unsigned int n, char **pcResult);

	//获取section下key的个数
	int getSectionKeyCount(dictionary *dicIni, const char *pcSectionName, unsigned int *n);

	//获取section的键的集合
	int getSectionKeys(dictionary *dicIni, const char *pcSectionName, char **pcKeys);

	//获取指定section:key的值
	STIniParserResult * getIniValue(dictionary *dicIni, const char *pcSectionName, const char *pcKey, EMIniPraseType enValueType);

	//获取指定section:key的字符串值
	char * getIniValueExtend(dictionary *dicIni, const char *pcSectionName, const char *pcKey);

	//获取ini文件中字符串的值（section已经拼接完成）
	char * inipGetStringExtend(dictionary *dicIni, const char * pcKey);

	//释放资源
	void releaseIniParser(dictionary * dicIni);

#ifdef __cplusplus
}
#endif

#endif

