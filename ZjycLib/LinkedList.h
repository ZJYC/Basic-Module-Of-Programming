#ifndef _LIST_H
#define _LIST_H

#include "DataType.h"

typedef struct S_LinkNode {
	struct S_LinkNode * Next;
	BUL32 Data;
} S_ListNode,*PS_ListNode;

//∑µªÿeTRUE±È¿˙÷’÷π
typedef EBOOL(*MapFunc)(PS_ListNode Node, BUL32 Data);
typedef EBOOL(*SortFunc)(PS_ListNode Node1, PS_ListNode Node2);

typedef struct {
	PS_ListNode(*NodeCreate)(BUL32 Data);
	EBOOL(*NodeAddToEnd)(PS_ListNode Header, PS_ListNode Node);
	EBOOL(*NodeAddToBgn)(PS_ListNode Header, PS_ListNode Node);
	EBOOL(*NodeTraversalWithFuc)(PS_ListNode Header, MapFunc Map, BUL32 MapParam);
	PS_ListNode(*NodeFindFirstWithFuc)(PS_ListNode Header, MapFunc Map, BUL32 MapParam);
	EBOOL(*NodeDel)(PS_ListNode Header, PS_ListNode Node);
	EBOOL(*NodeDelByData)(PS_ListNode Header, BUL32 Data);
	EBOOL(*NodeDelByIndex)(PS_ListNode Header, BUL32 Index);
	EBOOL(*NodePrintfAllData)(PS_ListNode Header);
}S_LinkedApi;

extern S_LinkedApi LinkedApi;

#endif