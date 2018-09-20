
#ifndef __GTL_H_
#define __GTL_H_

#ifndef offsetof
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({\
const typeof(((type *)0)->member) *__mptr = (ptr);\
(type *)((char *)__mptr - offsetof(type,member));})
#endif

// "const typeof(((type *)0)->member) *__mptr = (ptr);"
//������ʹ��ʱ����Ĳ��������⣺ptr��member���Ͳ�ƥ�䣬����ʱ�����warnning

#ifndef LIST_ENTRY
//ptr--�ڵ�ָ�����͡�����STDLinkNode *
//type --��������  ����STStudent ���������ͣ����Ǳ���
//member --��Ա����
#define LIST_ENTRY(ptr, type, member) \
container_of(ptr, type, member)
#endif

#endif

