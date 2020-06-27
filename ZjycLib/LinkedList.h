#ifndef _LIST_H
#define _LIST_H

#include "DataType.h"

typedef struct S_LinkNode {
	struct S_LinkNode * next;
	PVOID data;
} S_LinkNode,*PS_LinkNode,**PPS_LinkNode;

typedef struct {
	PS_LinkNode Create(PVOID data);
	BVOID Destroy(PPS_LinkNode list);
	PS_LinkNode InsertAfter(PS_LinkNode node, PVOID data);
	PS_LinkNode InsertBeginning(PS_LinkNode list, PVOID data);
	PS_LinkNode InsertEnd(PS_LinkNode list, PVOID data);
	BVOID Remove(PPS_LinkNode list, PS_LinkNode node);
	BVOID RemoveByData(PPS_LinkNode list, PVOID data);
	PS_LinkNode FindNode(PS_LinkNode list, PS_LinkNode node);
	PS_LinkNode FindByData(PS_LinkNode list, PVOID data);
	PS_LinkNode Find(PS_LinkNode list, int(*func)(PS_LinkNode, PVOID), PVOID data);
}S_LinkedApi;

extern S_LinkedApi LinkedApi;

#endif