#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "Heap5.h"

#define MALLOC	MM_Ops.Malloc
#define FREE	MM_Ops.Free

/* Creates a list (node) and returns it
 * Arguments: The data the list will contain or NULL to create an empty
 * list/node
 */
static PS_LinkNode Create(PVOID data) {
	PS_LinkNode l = MALLOC(sizeof(S_LinkNode));
	if (l != NULL) {
		l->next = NULL;
		l->data = data;
	}
	return l;
}

/* Completely destroys a list
 * Arguments: A pointer to a pointer to a list
 */
static BVOID Destroy(PPS_LinkNode list) {
	if (list == NULL) return;
	while (*list != NULL) {
		Remove(list, *list);
	}
}

/* Creates a list node and inserts it after the specified node
 * Arguments: A node to insert after and the data the new node will contain
 */
static PS_LinkNode InsertAfter(PS_LinkNode node, PVOID data) {
	PS_LinkNode new_node = Create(data);
	if (new_node) {
		new_node->next = node->next;
		node->next = new_node;
	}
	return new_node;
}

/* Creates a new list node and inserts it in the beginning of the list
 * Arguments: The list the node will be inserted to and the data the node will
 * contain
 */
static PS_LinkNode InsertBeginning(PS_LinkNode list, PVOID data) {
	PS_LinkNode new_node = Create(data);
	if (new_node != NULL) { new_node->next = list; }
	return new_node;
}

/* Creates a new list node and inserts it at the end of the list
 * Arguments: The list the node will be inserted to and the data the node will
 * contain
 */
static PS_LinkNode InsertEnd(PS_LinkNode list, PVOID data) {
	PS_LinkNode new_node = Create(data);
	if (new_node != NULL) {
		for (PS_LinkNode it = list; it != NULL; it = it->next) {
			if (it->next == NULL) {
				it->next = new_node;
				break;
			}
		}
	}
	return new_node;
}

/* Removes a node from the list
 * Arguments: The list and the node that will be removed
 */
static BVOID Remove(PPS_LinkNode list, PS_LinkNode node) {
	PS_LinkNode tmp = NULL;
	if (list == NULL || *list == NULL || node == NULL) return;

	if (*list == node) {
		*list = (*list)->next;
		FREE(node);
		node = NULL;
	}
	else {
		tmp = *list;
		while (tmp->next && tmp->next != node) tmp = tmp->next;
		if (tmp->next) {
			tmp->next = node->next;
			FREE(node);
			node = NULL;
		}
	}
}

/* Removes an element from a list by comparing the data pointers
 * Arguments: A pointer to a pointer to a list and the pointer to the data
 */
static BVOID RemoveByData(PPS_LinkNodelist, PVOID data) {
	if (list == NULL || *list == NULL || data == NULL) return;
	Remove(list, FindByData(*list, data));
}

/* Find an element in a list by the pointer to the element
 * Arguments: A pointer to a list and a pointer to the node/element
 */
static PS_LinkNode FindNode(PS_LinkNode list, PS_LinkNode node) {
	while (list) {
		if (list == node) break;
		list = list->next;
	}
	return list;
}

/* Finds an elemt in a list by the data pointer
 * Arguments: A pointer to a list and a pointer to the data
 */
static PS_LinkNode FindByData(PS_LinkNode list, PVOID data) {
	while (list) {
		if (list->data == data) break;
		list = list->next;
	}
	return list;
}

/* Finds an element in the list by using the comparison function
 * Arguments: A pointer to a list, the comparison function and a pointer to the
 * data
 */
static PS_LinkNode Find(PS_LinkNode list, int(*func)(PS_LinkNode, PVOID), PVOID data) {
	if (!func) return NULL;
	while (list) {
		if (func(list, data)) break;
		list = list->next;
	}
	return list;
}

S_LinkedApi LinkedApi = {
	.Create = Create,
	.Destroy = Destroy,
	.InsertAfter = InsertAfter,
	.InsertBeginning = InsertBeginning,
	.InsertEnd = InsertEnd,
	.Remove = Remove,
	.RemoveByData = RemoveByData,
	.FindNode = FindNode,
	.FindByData = FindByData,
	.Find = Find,
};





