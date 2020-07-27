/**
 * @file
 * @brief Creation and usage of Singly Linked Lists
 * @details Singly Linked Lists uses Nodes, these nodes are in fact just void * pointers and can be accesses just as if they were normal objects after a recast.
 * @details Functions and Macros from GeneralLinkedList.h may be used to navigate and get more info from the Linked List.
 */

#ifndef _LD_SINGLYLINKEDLIST_H_
#define _LD_SINGLYLINKEDLIST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "GeneralLinkedList.h"

/** @name Singly Linked List Aliases */
///@{ 
#define LD_LINKEDLIST struct LD_LinkedList
#define LD_SINGLYLINKEDLIST struct LD_LinkedList
///@}

/** @name Retreving operations over Singly Linked Lists
 */
///@{ 
#define LD_SinglyLinkedList_Next_to	LD_LinkedList_Next_to
#define LD_SinglyLinkedList_Previous_to LD_LinkedList_Previous_to
#define LD_SinglyLinkedList_First	LD_LinkedList_First
#define LD_SinglyLinkedList_Last	LD_LinkedList_Last
#define LD_SinglyLinkedList_Length	LD_LinkedList_Length
///@}


    //--- Singly Linked List functions---//

/** @name Deleting operations over Singly Linked Lists
 */
///@{ 
#define LD_SinglyLinkedList_Clear LD_LinkedList_Clear
#define LD_SinglyLinkedList_Destroy LD_LinkedList_Destroy
///@}

#define LD_SinglyLinkedList LD_LinkedList


/**
 * @brief Creates a Singly Linked List.
 * @details It is based on an Object Pool whose Arena size is calculated based on the Object Size.
 * @return It returns a pointer to the created Linked List.
 * @retval NULL if it fails.
 * @param Object_Size Size in bytes of the data that will be held.
 */
LD_LINKEDLIST * LD_LinkedList_Create (size_t Object_Size);

/** @brief Creates a Singly Linked List with specified object count per Arena.
 *  @details It behaves like LD_LinkedList_Create()
 * @param Object_Count The number of instances that will be held per Arena.
 */
LD_LINKEDLIST * LD_LinkedList_Create_EXT (size_t Object_Size, size_t Object_Count);




/**
 * @brief Clear the Linked List without destroying it by iterating over its elements.
 * @details This method of clearing may have a different effect on performance.
 * @param LinkedList Pointer to the Linked List
 */
void LD_LinkedList_Clear_iterate(LD_LINKEDLIST * LinkedList);


/**
 * @brief Append an element to the tail of the Linked List.
 * @details The appended item become the First element.
 * @return It returns a pointer to the included (copied) item. The returned item is a Node.
 * @param LinkedList Pointer to the Linked List.
 * @param Data Pointer to the data to be included (copied).
 */
void * LD_LinkedList_append(LD_LINKEDLIST * LinkedList,void * Data);

/**
 * @brief Append an element to the head of the Linked List
 * @return It returns a pointer to the included (copied) item. The returned item is a Node.
 * @param LinkedList Pointer to the Linked List
 * @param Data Pointer to the data to be included (copied)
 */
void * LD_LinkedList_prepend(LD_LINKEDLIST * LinkedList,void * Data);


/**
 * @brief Insert an item next to a given Node created by LD_LinkedList_append() or LD_LinkedList_prepend()
 * @details If \p Node is also the Head, the Inserted data now becomes the Head of the Linked List
 * @return It returns a pointer to the included (copied) item
 * @param Node Pointer to the Node 
 * @param Data Pointer to the data to be included (copied)
 */
void * LD_LinkedList_insert_Next_to(void * Node, void * Data);

/**
 * @brief Remove the first Node (the head) of the Linked List
 * @param LinkedList Pointer to the Linked List
 */
void LD_LinkedList_remove_First(LD_LINKEDLIST * LinkedList);


/**
 * @brief Remove all the Nodes next to the given Node
 * @param Node Pointer to the Node
 */
void LD_LinkedList_remove_Onward_Next_to(void * Node);

/**
 * @brief Detach from the Linked List the First Node (the head).
 * @details Internally the Node is still asociate with the Linked List Arenas, but it's detached from the rest of Nodes
 * @return It returns a Pointer to the detached item (the Node). It may be recasted to the appropiated type.
 * @param LinkedList Pointer to the Linked List
 */
void * LD_LinkedList_detach_First(LD_LINKEDLIST * LinkedList);

/** 
 * @brief Deletes a Node that have been detached from a Linked List.
 * @details Internally a detached node is still asociate with it's original Linked List, it may be properly deleted by this Function.
 * @param Node Pointer to the Node
 */
void LD_LinkedList_remove_detached(void * Node);

/** 
 * @brief Attaches a detached Node appending it to the Linked List.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Linked List.
 * @details The new Node is not attached to the Linked List Memory Arena.
 * @param LinkedList Pointer to the new host Linked List.
 * @param Node Pointer to the Node
 */
void LD_LinkedList_attach_append(LD_LINKEDLIST * LinkedList, void * Node);

/** 
 * @brief Attaches a detached Node prepending it to the Linked List.
 * @details The attached Node becomes the Head of the Linked List.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Linked List.
 * @details The new Node is not attached to the Linked List Memory Arena.
 * @param LinkedList Pointer to the new host Linked List.
 * @param Node Pointer to the Node
 */
void LD_LinkedList_attach_prepend(LD_LINKEDLIST * LinkedList,void * Node);



/**
 * @brief Attaches a detached Node appending it next to a given Node.
 * @details The new Node have to be of the same type or at least of the same size of the Nodes of its new host Linked List.
 * @details The new Node is not attached to the Linked List Memory Arena.
 * @param Node_ref Pointer to the Node to be used as reference
 * @param Node Pointer to the Node to be included next to \p Node_ref
 */
void LD_LinkedList_attach_Next_to(void* Node_ref ,void * Node);







//Some arcane functions:
LD_LINKEDLIST * LD_LinkedList_Init(LD_LINKEDLIST * LinkedList, size_t Object_Size);
LD_LINKEDLIST * LD_LinkedList_Init_EXT(LD_LINKEDLIST * LinkedList,size_t Object_Size, size_t Object_Count);
void LD_LinkedList_deInit(LD_LINKEDLIST * LinkedList);
void LD_LinkedList_Make_Circular(LD_LINKEDLIST * LinkedList);

#ifdef __cplusplus
}
#endif

#endif
