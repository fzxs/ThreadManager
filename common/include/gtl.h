
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
//开发者使用时输入的参数有问题：ptr与member类型不匹配，编译时便会有warnning

#ifndef LIST_ENTRY
//ptr--节点指针类型。例如STDLinkNode *
//type --数据类型  例如STStudent 这里是类型，不是变量
//member --成员属性
#define LIST_ENTRY(ptr, type, member) \
container_of(ptr, type, member)
#endif

#endif

