
#ifndef __BGD_SERVANT_H_
#define __BGD_SERVANT_H_

template<typename T>
class AbsServant
{
public:
	virtual ~AbsServant() {}
public:
	//��Ҫ�첽ִ�еļ���
	/*
	Ϊʲôû�в���?
	    ��Ϊ����������ݺͼ����Ƿ�װ��һ��ģ�Ϊ�˼��ٲ����Ĵ��ݣ��뽫������װ��AbsServant����������
	*/
	virtual T invoke() = 0;

	//������--������������Լ��ڴ���Ҫ�ͷţ����ڸ�release������ʵ��
	virtual int release() { return 0; }
};

#endif



