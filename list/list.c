#include <stdlib.h>
#include "list.h"

//传入一个list_t *指针, 把链表头指向NULL
void list_init(list_t *list)
{
    list->head = NULL;
}

//返回链表的长度.
int list_len(list_t *list)
{
    int i = 0;
    node_t *head = list->head;

    for (; head; head = head->next, i++)
        ;
    return i;
}

static node_t *new_node(void *data)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->next = NULL;
    return node;
}

static int get_index_by_node(list_t *list, node_t *p)
{
    int i;
    node_t *t = list->head;
    for (i = 0; t != p; t = t->next, i++)
        ;
    return i;
}

//在指定位置插入一个节点
int list_insert(list_t *list, int pos, void *data)
{
    int i = 0;
    node_t *head = list->head, *temp = head;
    node_t *node = new_node(data);

    if (!head)
    {
        list->head = node;
        return 1;
    }

    for (; temp && i != pos; temp = temp->next, i++)
        ; // get the node on position pos

    if (!temp)
        return 0; // unavaliable pos input:go through the list but no position equal pos.temp == NULL means we cycle the list

    if (temp == head)
    { // if insert node is the head node
        node->next = temp;
        list->head = node;
        return 1;
    }

    for (; head->next != temp; head = head->next)
        ; // else goto the insert node's pre node

    node->next = temp;
    head->next = node;
    return 1;
}

//在链表的末尾追加一个结点, 其数据为data
void list_append(list_t *list, void *data)
{
    node_t *head = list->head, *node = new_node(data);

    if (head)
    {
        for (; head->next; head = head->next)
            ; // goto the last node
        head->next = node;
    }
    else
        list->head = node;
}

//删除末尾元素, 执行成功返回1, 失败返回0
int list_pop_tail(list_t *list)
{
    node_t *head = list->head;

    if (!head)
        return 0; // only 0 eles, pop fail

    if (!(head->next))
    { // only 1 node , pop it.
        free(head);
        list->head = NULL;
        return 1;
    }

    for (; head->next->next; head = head->next)
        ; // goto the last second node
    free(head->next);
    head->next = NULL;
    return 1;
}

//删除位置为position的元素, 执行成功返回1, 失败返回0
int list_pop(list_t *list, int pos)
{
    int i = 0;
    node_t *head = list->head, *temp = head;

    if (!head)
        return 0; // no nodes at all

    for (; temp && i != pos; temp = temp->next, i++)
        ; // get the node on position pos

    if (!temp)
        return 0; // unavaliable pos input:go through the list but no position equal pos.temp == NULL means we cycle the list

    if (temp == head)
    { // if pop node is the head node
        list->head = head->next;
        free(head);
        return 1;
    }

    for (; head->next != temp; head = head->next)
        ; // else goto the pop node's pre node

    head->next = temp->next;
    free(temp);
    return 1;
}

/*
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list_init(list);

    list_append(list, "hello");
    list_append(list, "world");
    list_append(list, "hehe");

    char *data; //用来取出数据
    int pos; //用来记录位置

    while(list_iter(list, (void **)&data, &pos))//迭代打印链表
        printf("%s %d\n", data, pos);
 */
//迭代链表.函数每执行一次, 取出数据data和位置, 并移动到下一元素.取出成功返回1, 否则返回0(迭代完毕)
int list_iter(list_t *list, void **data_ptr, int *pos_ptr)
{
    static node_t *p = NULL;
    static int flag = 0;

    if (!flag)
    { // p init to head node when the first time
        p = list->head;
        flag = 1;
    }

    if (!p)
    {
        flag = 0;
        return 0;
    }
    *data_ptr = p->data;                   // get data
    *pos_ptr = get_index_by_node(list, p); // get index
    // move next
    p = p->next;
    return 1;
}

//获取位置为position的数据, 执行失败返回0, 执行成功返回1
int list_get(list_t *list, int pos, void **data_ptr)
{
    node_t *head = list->head;
    int i = 0;

    for (; head; head = head->next, i++)
    {
        if (i == pos)
        {
            *data_ptr = head->data;
            return 1;
        }
    }
    return 0;
}

//把位置为pos的数据设置为data.执行失败返回0, 执行成功返回1
int list_set(list_t *list, int pos, void *data)
{
    node_t *t;
    int i;

    for (t = list->head, i = 0; t; t = t->next, i++)
    {
        if (i == pos)
        {
            t->data = data;
            return 1;
        }
    }
    return 0;
}

//把链表list_b连接到list_a后面.
void list_extend(list_t *list_a, list_t *list_b)
{
    node_t *t;
    if (list_a->head == NULL)
        list_a->head = list_b->head;
    else
    {
        for (t = list_a->head; t->next; t = t->next)
            ;
        t->next = list_b->head;
    }
}

//交换位置为pos_a和pos_b上的元素, 成功返回1, 否则返回0
int list_swap(list_t *list, int pos_a, int pos_b)
{
    node_t *t, *a_pre = 0, *b_pre = 0, *a = 0, *b = 0, *head = list->head;
    int i;

    if (pos_a == 0)
        a = head;
    if (pos_b == 0)
        b = head;

    for (t = head, i = 0; t; t = t->next, i++)
    {
        if (i == pos_a - 1)
        {
            a_pre = t;
            a = t->next;
        }
        if (i == pos_b - 1)
        {
            b_pre = t;
            b = t->next;
        }
    }

    if (!a || !b)
        return 0; // wrong input position
    if (a == b)
        return 0; // do not swap the same node

    // change pre node's next
    if (a == head)
    {
        list->head = b;
        b_pre->next = a;
    }
    else if (b == head)
    {
        list->head = a;
        a_pre->next = b;
    }
    else
    {
        a_pre->next = b;
        b_pre->next = a;
    }

    // change a and b's next
    t = a->next;
    a->next = b->next;
    b->next = t;
    return 1;
}

//反转一个链表
void list_reverse(list_t *list)
{
    node_t *q = 0, *t, *m = list->head;
    for (; m; t = m->next, m->next = q, q = m, m = t)
        ;
    list->head = q;
}

//在链表中查找数据为data的结点, 返回找到的第一个符合条件的结点的位置, 否则找不到的话返回-1
int list_index(list_t *list, void *data)
{
    node_t *t = list->head;
    int i = 0;

    for (; t; t = t->next, i++)
    {
        if (t->data == data)
            return i;
    }
    return -1;
}

/*
    list_t *list = (list_t *)malloc(sizeof(list_t));

    list_init(list);

    int arr[6] = {1, 2, 3, 4, 5, 6};

    list_from_array(list, (void *)arr, sizeof(int), 6);

    int *data;
    int k = 20, pos;
    while(k--){
        list_cycle(list, (void **)&data, &pos);
        printf("%d %d\n", *data, pos);
    }
 */
//循环迭代器.相比list_iter而言, 就是不断重复的遍历到尾再回到头.
void list_cycle(list_t *list, void **data_ptr, int *pos_ptr)
{
    static node_t *p = NULL;
    if (!p)
        p = list->head;

    *data_ptr = p->data;                   // get data
    *pos_ptr = get_index_by_node(list, p); // get index
    // move
    p = p->next;
}

/*
例子
    list_t *list = (list_t *)malloc(sizeof(list_t));

    list_init(list);

    char str[] = "hello";
    //根据str字符数组生成一个链表
    list_from_array(list, (void *)str, sizeof(char), 5);
    //打印链表
    char *data;
    int pos;
    while(list_iter(list, (void **)&data, &pos))
        printf("%c %d\n", *data, pos);
或者根据一个整数数组来生成:

    list_t *list = (list_t *)malloc(sizeof(list_t));

    list_init(list);
    int arr[6] = {1, 2, 3, 4, 5, 6};
    list_from_array(list, (void *)arr, sizeof(int), 6);
    //打印链表
    int *data;
    int pos;
    while(list_iter(list, (void **)&data, &pos))
        printf("%d %d\n", *data, pos);
但是如下的数组arr是不可以的:

    char *arr[] = {"hello", "world", "hi"}; //因为这个数组存储的各个指针之间不是等差连续的
但是这个arr可以:

    char a[] = "hello";
    char b[] = "world";
    char c[] = "hi";
    char *arr[] = {a, b, c};
 */
//从一个数组生成一个链表(list必须是空链表才可以).
int list_from_array(list_t *list, void *ptr, int size, int len)
{
    if (list->head)
        return 0; // list is not empty

    int i;
    node_t *t;

    list->head = new_node(ptr); // init head

    for (i = 1, t = list->head; i < len; i++, t = t->next)
        t->next = new_node((char *)ptr + size * i);
    return 1;
}
