#include "skiplist.h"
 
unsigned rand_level()
{
    int level = 1;
    while(rand()%2 && level <= MAX_LEVEL)
        level++;
    return level;
}
 
PDataNode make_node(DataType val,unsigned level)
{
    PDataNode node = (PDataNode)malloc(sizeof(DataNode) + sizeof(NodeLevel)*level);
    if(node == NULL)
        return NULL;
 
    node->value = val;
    node->level_size = level;
 
    int i = 0;
    for(i=0; i<level; i++)
    {
        node->level[i].forward = NULL;
        node->level[i].span = 0;
    }
    return node;
}
 
void free_node(PDataNode node)
{
    if(NULL == node)
        return;
    free(node);
}
 
PSkipList skiplist_init()
{
    PSkipList sl = NULL;
    sl = (PSkipList)malloc(sizeof(SkipList));
    sl->level = 1;
    sl->size = 0;
    PDataNode header = make_node(INT_MIN,MAX_LEVEL);
    if(header == NULL)
    {
        free(sl);
        return NULL;
    }
    sl->header = header;
    return sl;
}
void destory_skiplist(PSkipList *sl)
{
    if(*sl == NULL) return;
 
    PDataNode pNode = (*sl)->header->level[0].forward;
    PDataNode next = NULL;
    while(pNode)
    {
        next = pNode->level[0].forward;
        free_node(pNode);
        pNode = next;
    }
    pNode = NULL;
    next = NULL;
    free_node((*sl)->header);
    (*sl)->header = NULL;
    free(*sl);
    (*sl) = NULL;
    return;
}
 
PDataNode del(PSkipList sl,DataType val)
{
    if(sl == NULL) return NULL;
    PDataNode del_node = NULL;
    PNodeLevel update[MAX_LEVEL];
    int i = MAX_LEVEL - 1;
    PDataNode cur,pre;
    pre = sl->header;
    cur = pre;
    while(i >= 0)
    {
        cur = pre->level[i].forward;
        while(cur != NULL && cur->value < val)
        {
            pre = cur;
            cur = cur->level[i].forward;
        }
        if(cur != NULL && cur->value == val)
        {
            del_node = cur;
        }
        update[i] = &(pre->level[i]);
        i--;
    }
 
    if(del_node == NULL)
        return NULL;
 
    i = MAX_LEVEL - 1;
    while(i >= 0)
    {
        if(update[i]->forward == NULL)
        {
            i--;
            continue;
        }
        else if(update[i]->forward->value > val)
        {
            update[i]->span--;
            i--;
        }
        else
        {
            update[i]->forward = del_node->level[i].forward;
            if(del_node->level[i].forward == NULL)
                update[i]->span = 0;
            else
                update[i]->span += del_node->level[i].span - 1;
            i--;
        }
    }
    sl->size--;
    return del_node;
}
 
PDataNode add(PSkipList sl,DataType val)
{
    if(sl == NULL) return NULL;
    unsigned node_level = rand_level();
    PDataNode pNode = make_node(val,node_level);
    printf("Level:%d   value:%d\n",node_level,val);
    if(pNode == NULL) return NULL;
 
    PNodeLevel update[MAX_LEVEL];
    //查找插入位置的遍历路径上处于第i层的节点在链表中的排位，头节点为rank[MAX_LEVEL-1]=0
    //查找从最高层开始且头节点在链表中是第一个，所以rank[MAX_LEVEL-1]=0
    unsigned rank[MAX_LEVEL] = {0};
    int i =  MAX_LEVEL-1;
    PDataNode cur,pre;
    pre = sl->header;
    cur = pre;
    while(i >= 0)
    {
        rank[i] = (i == MAX_LEVEL-1 ? 0 : rank[i+1]);
        cur = pre->level[i].forward;
        while(cur && cur->value < val)
        {
            rank[i] += pre->level[i].span;
            pre = cur;
            cur = cur->level[i].forward;
        }
        if(cur && cur->value == val)
            return NULL;
        update[i] = &(pre->level[i]);
        i--;
    }
    i = MAX_LEVEL-1;
    for(;i >=0; i--)
    {
        if(update[i]->forward == NULL && i > node_level-1)
        {
            continue;
        }
        else if(update[i]->forward == NULL && i <= node_level-1)
        {
            update[i]->forward = pNode;
            //节点的插入位置一定是在查找路基上第0层节点的后面一个
            //rank[0] - rank[i]得到的是查找路径上要更新的第层节点距离第0层节点的距离跨度，
            //这个跨度+1就得到了更新路劲上第i层节点到插入节点的跨度
            update[i]->span = (rank[0] - rank[i]) + 1;
        }
        else if(update[i]->forward && i > node_level-1)
        {
            update[i]->span++;
        }
        else
        {
            pNode->level[i].forward = update[i]->forward;
            //pNode->level[i].span = (update[i]->span + 1) - ((rank[0] - rank[i]) + 1);
            pNode->level[i].span = update[i]->span - (rank[0] - rank[i]);
            update[i]->forward = pNode;
            update[i]->span = (rank[0] - rank[i]) + 1;
        }
    }
    sl->size++;
    return pNode;
}
 
PDataNode find(PSkipList sl,DataType val)
{
    if(sl == NULL) return NULL;
    PDataNode pre  = sl->header;
    PDataNode cur = pre;
    int i =  MAX_LEVEL-1;
    unsigned rank[MAX_LEVEL-1] = {0};
    while(i >= 0)
    {
        rank[i] = (i == MAX_LEVEL-1 ? 0 : rank[i+1]);
        cur = pre->level[i].forward;
        while(cur != NULL && cur->value < val)
        {
            rank[i] += pre->level[i].span;
            pre = cur;
            cur = cur->level[i].forward;
        }
        if(cur != NULL && cur->value == val)
        {
            printf("val=%d,rank:%u\n",val,rank[i]+1);
            return cur;
        }
        i--;
    }
    return NULL;
}
 
void print_level(PSkipList sl, int i)
{
    PDataNode pNode = sl->header;
    printf("level  %d    ",i);
    while(pNode)
    {
        printf("%d",pNode->value);
        int num = pNode->level[i].span;
        while(num > 0)
        {
            printf("-");
            num--;
        }
        pNode = pNode->level[i].forward;
    }
    printf("\n");
}
 
void print_skiplist(PSkipList sl)
{
    if(sl == NULL)
    {
        printf("Null list");
        return ;
    }
    int i = MAX_LEVEL - 1;
    while(i >= 0)
    {
        print_level(sl,i);
        i--;
    }
}