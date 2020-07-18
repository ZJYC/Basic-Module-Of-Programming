#include "DataType.h"
#include "Heap5.h"
#include "LinkedList.h"



int main(BVOID) {
	MemApi.Init();
	PS_ListNode Header = LinkedApi.NodeCreate(0xAAAA);
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x1));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x2));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x3));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x4));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x5));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x6));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x7));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x8));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0x9));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0xA));
	LinkedApi.NodeAddToEnd(Header, LinkedApi.NodeCreate(0xB));
	LinkedApi.NodePrintfAllData(Header);

	LinkedApi.NodeAddToBgn(Header, LinkedApi.NodeCreate(0xC));
	LinkedApi.NodePrintfAllData(Header);
	
	LinkedApi.NodeDelByData(Header, 0xC);
	LinkedApi.NodePrintfAllData(Header);

	LinkedApi.NodeDelByIndex(Header, 0x08);
	LinkedApi.NodePrintfAllData(Header);
}













