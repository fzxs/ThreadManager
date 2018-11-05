
#ifndef __BGD_COMMON_H_
#define __BGD_COMMON_H_

typedef int THR_FUNC_RETURN;
typedef THR_FUNC_RETURN (*FuncUser)(void *);
typedef void * (*FuncEntrance)(void *);
typedef void(*FUNC_THR_CLEANUP)(void *object, void *param);


#endif



