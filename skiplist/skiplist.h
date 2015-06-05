#ifndef MY_SKIP_LIST
#define MY_SKIP_LIST
 
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
 
typedef int DataType;
 
#define MAX_LEVEL 8
 
struct DataNode;
 
 
typedef struct NodeLevel
{
    struct DataNode * forward;
    unsigned span;
 
}NodeLevel,*PNodeLevel;
 
/**
data node which stores data with DataType
*/
typedef struct DataNode
{
//    int (*compare)(DataType val1,DataType val2);
    DataType value;
    unsigned level_size;
    NodeLevel level[];
 
}DataNode,*PDataNode;
 
 
/**
skip list struct
*/
typedef struct SkipList
{
    unsigned level;
    unsigned size;
    PDataNode header;
 
}SkipList,*PSkipList;
 
/**
 *get rand level for data node created
 */
unsigned rand_level();
 
/**
 *initialize an empty skiplist
 */
PSkipList skiplist_init();
 
/**
 *destory a skip list 'sl',and free the memory occupied.
 */
void destory_skiplist(PSkipList *sl);
 
/**
 *make a new skip list node which stores 'val',
 *returns a pointer to the data node newly made
 */
PDataNode make_node(DataType val,unsigned level);
 
/**
 *delete a data node with data 'val' from skip list 'sl',
 *returns a pointer of DataNode deleted,
 *if DataNode with data 'val' does not exist,return NULL
 *note:memory occupied by 'val' not freed
 */
PDataNode del(PSkipList sl,DataType val);
 
/**
 *add a data node with data 'val' from skip list 'sl',
 *returns a pointer of DataNode added,
 *note:memory occupied by 'val' not freed
 */
PDataNode add(PSkipList sl,DataType val);
 
/**
 *find 'val' in skiplist 'sl',if founed,return the pointer of node with value 'val'
 *if not return NULL
 */
PDataNode find(PSkipList sl,DataType val);
 
/**
 *free the memory occupied by 'node'
 */
void free_node(PDataNode node);
 
void print_skiplist(PSkipList sl);
void print_level(PSkipList sl, int i);
 
#endif // MY_SKIP_LIST