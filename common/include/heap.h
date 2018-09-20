
#ifndef __HEAP_H_
#define __HEAP_H_

/* �ѽṹ */

#ifdef __cplusplus
extern "C"
{

#endif

	typedef struct tagSTHeapHead
	{
		void **pstRoot;                 //����������
		int size;                       //�ṹ��С
		int used;                       //�Ѿ�ʹ�õĿռ�
	}head_t;

	typedef int(*CompareFunc)(void *, void *);

	typedef void(*ReleaseFunc)(void *);

	int heap_reserve(head_t *pstHead, int iSize);

	int heap_push(head_t *pstHead, void *pvData, CompareFunc func);

	int head_empty(head_t *pstHead);

	int head_release(head_t *pstHead, ReleaseFunc func);

	void *heap_top(head_t *pstHead);

	void *heap_pop(head_t *pstHead, CompareFunc func);

	int head_clear(head_t *pstHead);

	typedef void(*ShowFunc)(void *);

	void test_show(head_t *pstHead, ShowFunc func);


#ifdef __cplusplus
}
#endif 

#endif



