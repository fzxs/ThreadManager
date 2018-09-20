
#ifndef __INIPARSERHELPER_H_
#define __INIPARSERHELPER_H_

/*ini�ļ���������*/

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

	//��ʼ��
	int initIniParser(const char *pcFileName , dictionary **dicIni);

	//��ȡsection�ĸ���
	int getSectionCount(dictionary *dicIni, int *iSenctionCount);

	//��ȡָ��λ�õ�section��ֵ
	int getSectionByIndex(dictionary *dicIni, unsigned int n, char **pcResult);

	//��ȡsection��key�ĸ���
	int getSectionKeyCount(dictionary *dicIni, const char *pcSectionName, unsigned int *n);

	//��ȡsection�ļ��ļ���
	int getSectionKeys(dictionary *dicIni, const char *pcSectionName, char **pcKeys);

	//��ȡָ��section:key��ֵ
	STIniParserResult * getIniValue(dictionary *dicIni, const char *pcSectionName, const char *pcKey, EMIniPraseType enValueType);

	//��ȡָ��section:key���ַ���ֵ
	char * getIniValueExtend(dictionary *dicIni, const char *pcSectionName, const char *pcKey);

	//��ȡini�ļ����ַ�����ֵ��section�Ѿ�ƴ����ɣ�
	char * inipGetStringExtend(dictionary *dicIni, const char * pcKey);

	//�ͷ���Դ
	void releaseIniParser(dictionary * dicIni);

#ifdef __cplusplus
}
#endif

#endif

