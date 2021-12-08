/** @file
 * @brief Hash Map creation and usage
 * @details Note: The Hash Map entries are pointers and the strings have to persists in external buffers.
 * */

// Luis Delgado. 2021 (Originally from 2020)
// Current version: June, 08, 2021

#ifndef __LD_HASH_MAP_H__
#define __LD_HASH_MAP_H__

#include <stdlib.h>
#include <string.h>
#include "ObjectPool.h"


/*******************************************************************
 * Notes:
 * - Hash Map entries depends on external buffers to store strings
 * *****************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

struct LD_HashMap
{
    LD_OBJECTPOOL Entry_Pool;
    size_t Length;
    void * DATA; 
};

#define LD_HASHMAP struct LD_HashMap


#define LD_HASHMAP_VALUE_TYPE void *
#define LD_HASHMAP_ENTRY_VALUE LD_HASHMAP_VALUE_TYPE
#define LD_HASHMAP_ENTRY LD_HASHMAP_ENTRY_VALUE *

/**
 * @brief Create a Hash Map.
 * @details It allocs the Hash Map along with its "buckets" and a memory pool for the entries. Basicaly, each entry is a pointer, so it is a Hash Map of pointers.
 * @details The memory allocated for the Hash Map depends on the length of the array.
 * @param Length Length of the Hash Map array.
 * @return It returns a pointer to the created Hash Map.
 * @retval NULL The process failed.
 * */
LD_HASHMAP * LD_HashMap_Create_EXT(size_t Length);

/**
 * @brief Create a Hash Map.
 * @details It allocs the Hash Map along with its "buckets" and a memory pool for the entries. Basicaly, each entry is a pointer, so it is a Hash Map of pointers.
 * @details The memory allocated for the Hash Map is fixed
 * @return It returns a pointer to the created Hash Map.
 * @retval NULL The process failed.
 * */
LD_HASHMAP * LD_HashMap_Create();

/** 
 * @brief Destroy a Hash Map.
 * @details It deallocs the given Hash Map along with its memory pool and "buckets".
 * @param HM Hash Map to destroy.
 * */
void LD_HashMap_Destroy(LD_HASHMAP * HM);
/**
 * @brief Clear a Hash Map.
 * @details Instead of destroying a Hash Map, it just clears it.
 * @param HM Hash Map to clear.
 * */
void LD_HashMap_Clear(LD_HASHMAP * HM);

/** @brief Given a string, create an entry in the Hash Map
 *  @details Given a string, this function will create (allocate) a new entry in the Hash Map by using the hashed string as a "bucket" index. The New entry have the allocated size of a pointer.
 *  @deatils When allocating, the string used WON'T BE COPIED AND SAVED, so it have to persist somewere else.
 *  @details If the entry already exists, this function will return it.
 *  @param HM Hash Map 
 *  @param str String to hash 
 *  @return It returns a pointer to the entry.
 *  @retval NULL The process failed.
 *  */
LD_HASHMAP_ENTRY LD_HashMap_Alloc(LD_HASHMAP * HM, char * str);



/** @brief Given a string, search for an entry in the Hash Map
 *  @details Given a string, this function will find entry in the Hash Map by using the hashed string as a "bucket" index.
 *  @param HM Hash Map.
 *  @param str String to hash.
 *  @return It returns a pointer to the found entry.
 *  @retval NULL There was not entry found.
 *  */
LD_HASHMAP_ENTRY LD_HashMap_Find(LD_HASHMAP * HM, char * str);


/** @brief Remove an entry from its Hash Map
 * @details Given a pointer to a previously found or created entry, this function will remove it from its Hash Map.
 * @param Entry Pointer to an entry to remove.
 * */
void LD_HashMap_Remove(LD_HASHMAP_ENTRY Entry);

/** @brief Get the value of an entry
 * @details Given a pointer to a previously found or created entry, this function will get the value of that Entry
 * @param Entry Pointer to an entry.
 * @return It will return the value of that entry (A pointer type variable).
 * */
LD_HASHMAP_VALUE_TYPE LD_HashMap_Get_Value(LD_HASHMAP_ENTRY Entry);


/** @brief Set the value of an entry
 * @details Given a pointer to a previously found or created entry, this function will set the value of that Entry given a value.
 * @param Entry Pointer to an entry.
 * @param Value The new value (Pointer type variable).
 * */
void LD_HashMap_Set_Value(LD_HASHMAP_ENTRY Entry,LD_HASHMAP_VALUE_TYPE Value);

#ifdef __cplusplus
}
#endif

#endif
