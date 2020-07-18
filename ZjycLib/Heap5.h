#ifndef __HEAP_5_H__
#define __HEAP_5_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#define portBYTE_ALIGNMENT      ( 4 )
#define portBYTE_ALIGNMENT_MASK ( 0x0003 )

#ifndef mtCOVERAGE_TEST_MARKER
#define mtCOVERAGE_TEST_MARKER()
#endif

#define configASSERT        assert

typedef struct HeapRegion{
	BUB08 *pucStartAddress;
	BUL32 xSizeInBytes;
} HeapRegion_t;

typedef struct MM_OpsTypedef_{
	BVOID(*Init)();
	PVOID(*Malloc)(BUL32 xWantedSize);
	BVOID(*Free)(PVOID pv);
	BUL32(*HeapSize)();
}S_MemApi, *PS_MemApi;

extern S_MemApi MemApi;

#ifdef __cplusplus
}
#endif

#endif
