#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"
int main()
{
    PSkipList sl = skiplist_init();
    DataType vals[] = {1,3,5,7,9,2,4,6,8,10,11,15,12,18,21,31,56,100,94,68,60,88,66};
    unsigned val_size = sizeof(vals)/sizeof(vals[0]);
    printf("value number = %u\n",val_size);
    unsigned i = 0;
    for(;i < val_size; i++)
    {
        add(sl,vals[i]);
    }
    print_skiplist(sl);
    find(sl,31);
    find(sl,100);
    find(sl,66);
 
    PDataNode del_node = del(sl,31);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,88);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,21);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,6);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,94);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,60);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,68);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,100);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = del(sl,1);
    free_node(del_node);
    print_skiplist(sl);
 
    del_node = NULL;
 
    destory_skiplist(&sl);
    print_skiplist(sl);
 
    return 0;
}