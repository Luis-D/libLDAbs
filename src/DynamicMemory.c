#include "DynamicMemory.h"

#ifndef LD_DYNMEM_DEFAULT_ARENA_SIZE 
    #define LD_DYNMEM_DEFAULT_ARENA_SIZE 4096 //<- 4kb
#endif

#include <stdlib.h>

LD_DYNAMICMEM * LD_DynamicMemory_Init(LD_DYNAMICMEM * DynMem)
{
	DynMem->Data = NULL;
	DynMem->Cur_Data = NULL;
	DynMem->Filled_Arenas = NULL;
}

LD_DYNAMICMEM * LD_DynamicMemory_Create()
{
	LD_DYNAMICMEM * RET = malloc(sizeof(LD_DYNAMICMEM));
	if(RET == NULL){return NULL;}
	return LD_DynamicMemory_Init(RET);
}

void LD_DynamicMemory_Clear(LD_DYNAMICMEM * DynMem)
{
	//TODO: Limpiar lo demás
	struct LD_DynamicMem_Arena_Header * Cur = DynMem->Data;
	while(Cur != NULL){
	    struct LD_DynamicMem_Arena_Header * Next = Cur->Next;
	    free(Cur);
	    Cur = Next;
	}

	Cur = DynMem->Filled_Arenas;
	while(Cur != NULL){
	    struct LD_DynamicMem_Arena_Header * Next = Cur->Next;
	    free(Cur);
	    Cur = Next;
	}

	LD_DynamicMemory_Init(DynMem);
}

void LD_DynamicMemory_Destroy(LD_DYNAMICMEM * DynMem)
{
	LD_DynamicMemory_Clear(DynMem);
	free(DynMem);
}

struct LD_DynamicMem_Tree_Node_Header * __DynamicMem_Tree_Node_Header_Init(void * ptr, size_t size, struct LD_DynamicMem_Tree_Node_Header * Parent){
    struct LD_DynamicMem_Tree_Node_Header * RET = 
	(struct LD_DynamicMem_Tree_Node_Header*)ptr;
    RET->Parent = Parent;
    RET->Left = NULL;
    RET->Right = NULL;
    RET->Free = 1; //1 means it is free space
    RET->Net_Size = size;
    return RET;
}

static struct LD_DynamicMem_Arena_Header *__DynamicMem_Arena_Create(size_t size){
    	void * tmp = malloc(size);
	if(tmp == NULL){return NULL;}
	struct LD_DynamicMem_Arena_Header * Arena = 
		(struct LD_DynamicMem_Arena_Header*) tmp;
	Arena->Next = NULL;
	__DynamicMem_Tree_Node_Header_Init(&Arena->Root,
		size-sizeof(struct LD_DynamicMem_Arena_Header),
		NULL);
	//Arena->Net_Size = size-sizeof(struct LD_DynamicMem_Arena_Header);
	return Arena;
}


void * __DynamicMem_Search_Alloc_Rec(struct LD_DynamicMem_Tree_Node_Header * Node,size_t Size, int * err){
    if((Node == NULL)){*err = 1;return NULL;} //No Node
    if(Node->Free == 0){*err =3; return NULL;} //This node is not free
    if(Node->Net_Size<Size){*err = 2;return NULL;} //No more space by size
    struct LD_DynamicMem_Tree_Node_Header * L = Node->Left;
    struct LD_DynamicMem_Tree_Node_Header * R = Node->Right;
	
    struct LD_DynamicMem_Tree_Node_Header * Returnable = NULL;

    //If the Node->Net_Size and Size is just the same, the entire node is used
    if(Node->Net_Size==Size){
	//We are going to return the Free Memory Section of L (Not L itself, but the free memory region next to the header)
	Returnable = Node;
	Returnable->Free = 0;
	*err = 0;
	return (((void*)Returnable)+sizeof(struct LD_DynamicMem_Tree_Node_Header));
    } 

    //If both leafs are non-existent, then
    //create both leaves an populate one
    //the other leaf may be a new 'root' later
    if((L==NULL)&&(R==NULL)){
		//In Memory, Region R is just next to Region L
	L = __DynamicMem_Tree_Node_Header_Init(
		(((void*)Node)+sizeof(struct LD_DynamicMem_Tree_Node_Header)),
		Size,Node);
	R = __DynamicMem_Tree_Node_Header_Init(
		(((void*)Node)+sizeof(struct LD_DynamicMem_Tree_Node_Header)
		+Size
		+sizeof(struct LD_DynamicMem_Tree_Node_Header)), //L's header size is added
		(Node->Net_Size)-Size,Node);
	Node->Left = L;
	Node->Right = R;
    }
	
    Returnable = __DynamicMem_Search_Alloc_Rec(L,Size,err);

    if(Returnable == NULL){
	return __DynamicMem_Search_Alloc_Rec(R,Size,err);
    }

    if(Returnable == NULL){*err= 4;}

    return Returnable;
}

void * __DynamicMem_Search_Alloc(struct LD_DynamicMem_Arena_Header * Node,size_t Size,int * err){
    int tmperr;
    while(true){
	if(Node ==NULL){*err=1;return NULL;}
	void * ret = __DynamicMem_Search_Alloc_Rec(&Node->Root,Size,&tmperr);
	switch(tmperr)
	{
	    case 4:
	    case 3:
	    case 2:{
		    Node = Node->Next;
		    break;
		}
	    default:{
		*err = tmperr;
		return ret;
	    }
	}
    }
}

void*LD_DynamicMem_Alloc(LD_DYNAMICMEM*DynMem,size_t Size)
{ 
    int err = 0;
    void * ptr = NULL;

    while(true){
	err = 0;
    	ptr=__DynamicMem_Search_Alloc(DynMem->Data,Size,&err);
	    switch(err){
		case 0: {return ptr;break;}
		case 1:{
			struct LD_DynamicMem_Arena_Header * Dat = 
				 __DynamicMem_Arena_Create(LD_DYNMEM_DEFAULT_ARENA_SIZE);
			Dat->Next = DynMem->Data;
			DynMem->Data = Dat;
			break;
		    }
	    }
    }
    return ptr;
}

void LD_DynamicMem_Free(void * Object){
    struct LD_DynamicMem_Tree_Node_Header * Cur = (((void*)Object)-sizeof(struct LD_DynamicMem_Tree_Node_Header));
    __DynamicMem_Tree_Node_Header_Init(Cur,Cur->Net_Size,Cur->Parent);
}


