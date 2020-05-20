#include    "ucos/OS_CPU.H"
#include    "ucos/OS_CFG.H"
#include    "ucos/uCOS_II.H"



extern void *memcpy(void *destination,const void *source,unsigned int size);

extern void *memset(void * m,int c,unsigned int size);

// 20200521:
// the document nobody use.
// and, the " ./h/ucos " dir and content is backup from origin writer.
// so, we don't care about it. Taking a look is enough.