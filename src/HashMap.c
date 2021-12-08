#include "HashMap.h"
#include "ObjectPool.h"

#include "string.h"
#include "stddef.h"
#include "stdio.h"

/*******************************************************************
 * Notes:
 * - Hash Map entries depends on external buffers to store strings
 * *****************************************************************/

/***************************
 * HASH MAP STRUCTS
 * ************************/

#define LD_HASHMAP_MAP_SIZE ((1024*2)-32-sizeof(LD_HASHMAP))
#define LD_HASHMAP_OP_MAX_LEN 64
#define LD_HASHMAP_VALUE_EMPTY NULL

struct LD_HashMap_Key
{
    size_t Length;
    char * String; //<- Depends on external buffer
};

struct LD_HashMap_Node
{
    struct LD_HashMap_Node * Next; //<- IT HAVE TO BE THE FIRST
    struct LD_HashMap_Node * Previous;
    struct LD_HashMap_Key Key;
    LD_HASHMAP_VALUE_TYPE Value;
};
#define LD_HASHMAP_NODE struct LD_HashMap_Node


#define LD_HASHMAP_HASH_TYPE size_t
struct LD_HashMap_Hash
{
    LD_HASHMAP_HASH_TYPE Hash;
    struct LD_HashMap_Key Key;
};
#define LD_HASHMAP_HASH struct LD_HashMap_Hash
#define LD_HASH_MAP LD_HASHMAP

static char _LD_HM_Key_CMP
(struct LD_HashMap_Key * A, struct LD_HashMap_Key * B)
{
    /*
    size_t A_Len = A->Length;
    if(A_Len != B->Length){return 0;}
    */
    return (strcmp(A->String,B->String)==0);
}

void LD_HashMap_Hash_Init(LD_HASHMAP_HASH * Hash, char * Str_Buffer)
{
    struct LD_HashMap_Key * Key = &Hash->Key;
    Hash->Hash = 0;
    Key->Length = 0;
    Key->String = Str_Buffer; //<- It's be buffer where the string will be

    char *str = Str_Buffer;
    while((*str) != '\0')
    {
	size_t NewLen = Key->Length+1;
	(Key->Length) = NewLen;
	Hash->Hash += ((*str) * NewLen);
	str++;
    }
}


static LD_HASHMAP_VALUE_TYPE _LD_HashMap_Find_Entry
    (LD_HASHMAP * HMPP, char * str, char Alloc)
{
    if(str == NULL){return NULL;}

    LD_HASHMAP_HASH _Hash;
    LD_HashMap_Hash_Init(&_Hash,str);

    size_t Actual_Len = _Hash.Key.Length;
    char * string_save = _Hash.Key.String;

    size_t Prime = 2069;
    LD_HASHMAP_HASH_TYPE Hash = (_Hash.Hash)%Prime;

    size_t Len = (HMPP)->Length;
    Hash = Hash%Len;

    struct LD_HashMap_Node ** Bucket = NULL;

    Bucket=((struct LD_HashMap_Node**)(HMPP+1))+Hash;
    struct LD_HashMap_Node * Node = *Bucket;
    struct LD_HashMap_Node * Previous_Next = NULL;

	//Collision! Use Iterate then Linked List if needed
    if(Node!=LD_HASHMAP_VALUE_EMPTY)
    {
	while(1)
	{
	    //IF EQUAL
	    if(_LD_HM_Key_CMP(&Node->Key,&_Hash.Key))
	    {
		goto Hashmap_find_well_used_goto;
	    }

	    if(Node->Next == NULL)
	    {
		Previous_Next = Node;
		break;
	    }

	    Node = Node->Next;
	}

    }

	//If the algorith was just to search, then return nothing
    if(Alloc == 0){return NULL;}

	//If the algorith was to alloc, then continue
    Node = LD_ObjectPool_New(&HMPP->Entry_Pool);
    if(Node == NULL){return NULL;}

    (Node)->Next = NULL;
    memcpy(&Node->Key,&_Hash.Key,sizeof(struct LD_HashMap_Key));
    memset(&Node->Value,0,sizeof(LD_HASHMAP_VALUE_TYPE));

    if(Previous_Next != NULL)
    {
	Previous_Next->Next = Node;
	Node->Previous = Previous_Next;
    }
    else
    {
	*Bucket = Node;
	Node->Previous = (struct LD_HashMap_Node*) Bucket;
    }


Hashmap_find_well_used_goto:

    return &(Node->Value);
}


static void _LD_HashMap_Remove_Entry(void * Data)
{
    if(Data == NULL){return;}

    struct LD_HashMap_Node * Node =
	(void*)
	(
	((char*)Data)
	-offsetof(struct LD_HashMap_Node,Value)
	);

    struct LD_HashMap_Node * Next = Node->Next;
    struct LD_HashMap_Node * Prev = Node->Previous;

    Prev->Next = Next;
    if(Next != NULL)
    {
	Next->Previous = Prev;
    }

    LD_ObjectPool_Delete(Node);
}



/**************************
 * HASH MAP API
 * ***********************/

LD_HASHMAP * LD_HashMap_Create_EXT(size_t Length)
{
    LD_HASHMAP * Return = 
	(LD_HASHMAP*) calloc(1,sizeof(LD_HASHMAP)+(sizeof(LD_HASHMAP_VALUE_TYPE)*Length));
    if(Return == NULL){return NULL;}
    size_t OP_L = Length>>1;
    if(OP_L > LD_HASHMAP_OP_MAX_LEN)
    {OP_L = LD_HASHMAP_OP_MAX_LEN;}
    LD_ObjectPool_Init_EXT(&Return->Entry_Pool,OP_L,sizeof(LD_HASHMAP_NODE));
    Return->Length = OP_L;
    Return->DATA = Return+1;
    return Return;
}

LD_HASHMAP * LD_HashMap_Create()
{
    size_t Len = (LD_HASHMAP_MAP_SIZE)/sizeof(void*);
    return LD_HashMap_Create_EXT(Len);
}


void LD_HashMap_Destroy(LD_HASHMAP * HM)
{
   LD_ObjectPool_deInit(&HM->Entry_Pool);
   free(HM);
}

void LD_HashMap_Clear(LD_HASHMAP * HM)
{
    LD_ObjectPool_Clear(&HM->Entry_Pool);
    memset(HM->DATA,0,sizeof(void*)*HM->Length);
}

LD_HASHMAP_VALUE_TYPE * LD_HashMap_Alloc(LD_HASHMAP * HM, char * str)
{ return _LD_HashMap_Find_Entry(HM, str, 1); }

LD_HASHMAP_VALUE_TYPE * LD_HashMap_Find(LD_HASHMAP * HM, char * str)
{ return _LD_HashMap_Find_Entry(HM, str, 0); }

void LD_HashMap_Remove(LD_HASHMAP_VALUE_TYPE * Entry)
{ _LD_HashMap_Remove_Entry((void *) Entry);}


void LD_HashMap_Set_Value(LD_HASHMAP_VALUE_TYPE * Entry,LD_HASHMAP_VALUE_TYPE Value)
{
    if(Entry != NULL)
    {
	*Entry = Value;
    }
}


LD_HASHMAP_VALUE_TYPE LD_HashMap_Get_Value(LD_HASHMAP_VALUE_TYPE * Entry)
{
    if(Entry == NULL){return NULL;}
    return *Entry;
}
