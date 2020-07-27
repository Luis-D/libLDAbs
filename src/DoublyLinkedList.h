/**
 * @file
 * @brief Creation and usage of Doubly Linked Lists
 * @details Doubly Linked Lists uses Nodes, these nodes are in fact just void * pointers and can be accesses just as if they were normal objects after a recast.
 * @details Functions and Macros from GeneralLinkedList.h may be used to navigate and get more info from the Linked List.
 */

#ifndef _LD_DOUBLYLINKEDLIST_H
#define _LD_DOUBLYLINKEDLIST_H

#ifdef __cplusplus
extern "C" 
{
#endif

//#include "SinglyLinkedList.h"
#include "GeneralLinkedList.h"

/** @name Doubly Linked List Alias */
///@{ 
#define LD_DOUBLYLINKEDLIST struct LD_LinkedList
///@} 

//--- Doubly Linked List functions---//

/** @name Retreving operations over Doubly Linked Lists
 */
///@{ 
#define LD_DoublyLinkedList_Next_to	LD_LinkedList_Next_to
#define LD_DoublyLinkedList_Previous_to LD_LinkedList_Previous_to
#define LD_DoublyLinkedList_First	LD_LinkedList_First
#define LD_DoublyLinkedList_Last	LD_LinkedList_Last
#define LD_DoublyLinkedList_Length	LD_LinkedList_Length
///@}


/** @name Deleting operations over Singly Linked Lists
 */
///@{ 
#define LD_DoublyLinkedList_Clear LD_LinkedList_Clear
#define LD_DoublyLinkedList_Destroy LD_LinkedList_Destroy
///@}

/**
 * @brief Creates a Doubly Linked List.
 * @details It is based on an Object Pool whose Arena size is calculated based on the Object Size.
 * @return It returns a pointer to the created Linked List.
 * @retval NULL if it fails
 * @param Object_Size Size in bytes of the data that will be held.
 */
LD_DOUBLYLINKEDLIST * LD_DoublyLinkedList_Create (size_t Object_Size);


/** @brief Creates a Doubly Linked List with specified object count per Arena.
 *  @details It behaves like LD_DoublyLinkedList_Create()
 * @param Object_Size Size in bytes of the data that will be held.
 * @param Object_Count The number of instances that will be held per Arena.
 */
LD_DOUBLYLINKEDLIST * LD_DoublyLinkedList_Create_EXT (size_t Object_Size, size_t Object_Count);


/**
 * @brief Clear the Linked List without destroying it by iterating over its elements.
 * @details This method of clearing may have a different effect on performance.
 * @param LinkedList Pointer to the Linked List
 */
void LD_DoublyLinkedList_Clear_iterate(LD_DOUBLYLINKEDLIST * LinkedList);


/**
 * @brief Append an element to the tail of the Linked List.
 * @details The appended item become the First element.
 * @return It returns a pointer to the included (copied) item. The returned item is a Node.
 * @param LinkedList Pointer to the Linked List.
 * @param Data Pointer to the data to be included (copied).
 */
void * LD_DoublyLinkedList_append(LD_DOUBLYLINKEDLIST * LinkedList, void * Data);

/**
 * @brief Append an element to the head of the Linked List
 * @return It returns a pointer to the included (copied) item. The returned item is a Node.
 * @param LinkedList Pointer to the Linked List
 * @param Data Pointer to the data to be included (copied)
 */
void * LD_DoublyLinkedList_prepend(LD_DOUBLYLINKEDLIST* LinkedList,void * Data);


/**
 * @brief Insert an item next to a given Node created by LD_DoublyLinkedList_append() or LD_DoublyLinkedList_prepend()
 * @details If \p Node is also the Head, the Inserted data now becomes the Head of the Linked List
 * @return It returns a pointer to the included (copied) item
 * @param Node Pointer to the Node 
 * @param Data Pointer to the data to be included (copied)
 */
void * LD_DoublyLinkedList_insert_Next_to(void * Node,  void * Data);


/**
 * @brief Insert an item previous to a given Node created by LD_LinkedList_append() or LD_LinkedList_prepend()
 * @details If \p Node is also the Tail, the Inserted data now becomes the Tail of the Linked List
 * @return It returns a pointer to the included (copied) item
 * @param Node Pointer to the Node 
 * @param Data Pointer to the data to be included (copied)
 */
void * LD_DoublyLinkedList_insert_Previous_to(void * Node,  void * Data);

/**
 * @brief Remove the given Node from the Doubly Linked List
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_remove(void * Node);

/**
 * @brief Remove all the Nodes starting from a given Node
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_remove_Onward(void * Node);

/**
 * @brief Remove all the Nodes starting from a given Node, but backward
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_remove_Backward(void * Node);


/**
 * @brief Detach from the Linked List the given Node.
 * @details Internally the Node is still asociate with the Linked List Arenas, but it's detached from the rest of Nodes
 * @return It returns a Pointer to the detached item (the Node). It may be recasted to the appropiated type.
 * @param LinkedList Pointer to the Linked List
 */
void LD_DoublyLinkedList_detach(void * Node);


/** 
 * @brief Deletes a Node that have been detached from a Doubly Linked List.
 * @details Internally a detached node is still asociate with it's original Doubly Linked List, it may be properly deleted by this Function.
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_remove_detached(void * Node);


/** 
 * @brief Attaches a detached Node appending it to the Doubly Linked List.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Doubly Linked List.
 * @details The new Node is not attached to the Doubly Linked List Memory Arena.
 * @param LinkedList Pointer to the new host Doubly Linked List.
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_attach_append(LD_DOUBLYLINKEDLIST * LinkedList,void * Node);

/** 
 * @brief Attaches a detached Node prepending it to the Doubly Linked List.
 * @details The attached Node becomes the Head of the Doubly Linked List.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Doubly Linked List.
 * @details The new Node is not attached to the Doubly Linked List Memory Arena.
 * @param LinkedList Pointer to the new host Doubly Linked List.
 * @param Node Pointer to the Node
 */
void LD_DoublyLinkedList_attach_prepend(LD_DOUBLYLINKEDLIST * LinkedList,void * Node);


/**
 * @brief Attaches a detached Node appending it next to a given Node.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Doubly Linked List.
 * @details The new Node is not attached to the Doubly Linked List Memory Arena.
 * @param Node_ref Pointer to the Node to be used as reference
 * @param Node Pointer to the Node to be included next to \p Node_ref
 */
void LD_DoublyLinkedList_attach_Next_to(void * Node_ref,void *Node);


/**
 * @brief Attaches a detached Node prepending it next to a given Node.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Doubly Linked List.
 * @details The new Node is not attached to the Doubly Linked List Memory Arena.
 * @param Node_ref Pointer to the Node to be used as reference
 * @param Node Pointer to the Node to be included next to \p Node_ref
 */
void LD_DoublyLinkedList_attach_Previous_to(void * Node_ref,void *Node);








//Some arcane functions:

void LD_DoublyLinkedList_Init(LD_DOUBLYLINKEDLIST * LinkedList,size_t Object_Size);

#define LD_DoublyLinkedList_deInit LD_LinkedList_deInit


void LD_DoublyLinkedList_Make_Circular(LD_DOUBLYLINKEDLIST * LinkedList);

#ifdef __cplusplus
}
#endif

#endif
