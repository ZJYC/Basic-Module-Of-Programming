#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkedList.h"
#include "Heap5.h"

#define MALLOC	MemApi.Malloc
#define FREE	MemApi.Free
//�����ڵ�
static PS_ListNode NodeCreate(BUL32 Data) {
	PS_ListNode Node = MALLOC(SIZE(S_ListNode));
	if (Node != NULL) {
		Node->Next = NULL;
		Node->Data = Data;
	}
	return Node;
}
//��ӽڵ㵽ĩβ
static EBOOL NodeAddToEnd(PS_ListNode Header,PS_ListNode Node) {
	if (Header == NULL || Node == NULL)return eFAIL;
	for (Header = Header; Header->Next != NULL; Header = Header->Next);
	Header->Next = Node;
	return eTRUE;
}
//��ӽڵ㵽�б���
static EBOOL NodeAddToBgn(PS_ListNode Header, PS_ListNode Node) {
	if (Header == NULL || Node == NULL)return eFAIL;
	Node->Next = Header->Next;
	Header->Next = Node;
	return eTRUE;
}
//ʹ�ú������������б�
static EBOOL NodeTraversalWithFuc(PS_ListNode Header, MapFunc Map, BUL32 MapParam) {
	if (Header == NULL || Map == NULL)return eFAIL;
	for (Header = Header; Header != NULL; Header = Header->Next) {
		if (Map != NULL) {
			Map(Header, MapParam);
		}
		else {
			return NULL;
		}
	}
	return eTRUE;
}
//ʹ�ú������������б�
static PS_ListNode NodeFindFirstWithFuc(PS_ListNode Header, MapFunc Map, BUL32 MapParam) {
	if (Header == NULL || Map == NULL)return eFAIL;
	for (Header = Header; Header != NULL; Header = Header->Next) {
		if (Map != NULL) {
			if(Map(Header, MapParam) == eTRUE)return Header;
		}
		else {
			return NULL;
		}
	}
	return NULL;
}
//��������������
static EBOOL MapFunc_Data(PS_ListNode Node, BUL32 Data) {
	if (Node == NULL)return eFAIL;
	if (Node->Data == Data)return eTRUE;
	return eFAIL;
}
//ɾ��ĳ���ڵ�
static EBOOL NodeDel(PS_ListNode Header, PS_ListNode Node) {
	if (Header == NULL || Node == NULL)return eFAIL;
	if (Node == Header) {
		return eFAIL;
	}
	for (Header = Header; Header->Next != NULL; Header = Header->Next) {
		if (Header->Next == Node) {
			PS_ListNode Fore, Betn, Back;
			Fore = Header;
			Betn = Header->Next;
			Back = (Betn == NULL ? NULL : Betn->Next);
			Fore->Next = Back;
			FREE((PVOID)Node);
			return eTRUE;
		}
	}
	return eFAIL;
}
//��������ɾ��ĳ���ڵ�
static EBOOL NodeDelByData(PS_ListNode Header, BUL32 Data) {
	if (Header == NULL)return eFAIL;

	PS_ListNode Find = NodeFindFirstWithFuc(Header, MapFunc_Data, Data);

	if (Find != NULL) {
		NodeDel(Header, Find);
		return eTRUE;
	}
	else {
		return eFAIL;
	}
	

}
//��������ɾ��ĳ���ڵ�
static EBOOL NodeDelByIndex(PS_ListNode Header, BUL32 Index) {
	if (Header == NULL || Index == NULL)return eFAIL;
	BUL32 Cnt = NULL;
	for (PS_ListNode Node = Header; Node != NULL; Node = Node->Next, Cnt++) {
		if (Cnt == Index && Node != NULL) {
			NodeDel(Header,Node);
			return eTRUE;
		}
	}
	return eFAIL;
}
//��ӡÿһ���ڵ����Ϣ
static EBOOL MapFunc_Printf(PS_ListNode Node, BUL32 Data) {
	if (Node == NULL)return eFAIL;
	printf("0x%08X\r\n", Node->Data);
}
static EBOOL NodePrintfAllData(PS_ListNode Header) {
	if (Header == NULL)return eFAIL;
	NodeTraversalWithFuc(Header, MapFunc_Printf, NULL);
}
//������(����eTRUE��������)
static EBOOL SortFunc(PS_ListNode Node1, PS_ListNode Node2) {
	if (Node1 == NULL || Node2 == NULL)return eFAIL;
	if (Node1->Data >= Node2->Data)return eTRUE;
	return eFAIL;
}
//��ֵ����
static EBOOL NodeSwapData(PS_ListNode Node1, PS_ListNode Node2) {
	if (Node1 == NULL || Node2 == NULL)return eFAIL;
	BUL32 Mid = Node1->Data;
	Node1->Data = Node2->Data;
	Node2->Data = Mid;
	return eTRUE;
}
static EBOOL NodeSwapByDataS2B(PS_ListNode Header) {
	if (Header == NULL)return eFAIL;

	

}

S_LinkedApi LinkedApi = {
.NodeCreate = NodeCreate,
.NodeAddToEnd = NodeAddToEnd,
.NodeAddToBgn = NodeAddToBgn,
.NodeTraversalWithFuc = NodeTraversalWithFuc,
.NodeFindFirstWithFuc = NodeFindFirstWithFuc,
.NodeDel = NodeDel,
.NodeDelByData = NodeDelByData,
.NodeDelByIndex = NodeDelByIndex,
.NodePrintfAllData = NodePrintfAllData,
};





