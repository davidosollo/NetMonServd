/////////////////////////////////////////////////////////////////////////////
//  Source:      linked_list.c
//  Project:     NeMonServd
//  Subject:     Aplicacion y diseño de sistemas embebidos
//  Univerisity: Universidad Autonoma de Guadalajara
//  Description: Handle the linked list used to keep the monitoring network 
//               activity
//  
//


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "linked_list.h"

/*
    create a new node
    initialize the data and next field
 
    return the newly created node
*/
node* create(int iNodeNum,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->iNodeNum = iNodeNum;
    new_node->next = next;
 
    return new_node;
}
 
/*
    add a new node at the beginning of the list
*/
node* prepend(node* head,int iNodeNum)
{
    node* new_node = create(iNodeNum,head);
    head = new_node;
    return head;
}
 
/*
    add a new node at the end of the list
*/
node* append(node* head, int iNodeNum)
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(iNodeNum,NULL);
    cursor->next = new_node;
 
    return head;
}
 
/*
    insert a new node after the prev node
*/
node* insert_after(node *head, int iNodeNum, node* prev)
{
    if(head == NULL || prev == NULL)
        return NULL;
    /* find the prev node, starting from the first node*/
    node *cursor = head;
    while(cursor != prev)
        cursor = cursor->next;
 
    if(cursor != NULL)
    {
        node* new_node = create(iNodeNum,cursor->next);
        cursor->next = new_node;
        return head;
    }
    else
    {
        return NULL;
    }
}
 
/*
    insert a new node before the nxt node
*/
node* insert_before(node *head, int iNodeNum, node* nxt)
{
    if(nxt == NULL || head == NULL)
        return NULL;
 
    if(head == nxt)
    {
        head = prepend(head,iNodeNum);
        return head;
    }
 
    /* find the prev node, starting from the first node*/
    node *cursor = head;
    while(cursor != NULL)
    {
        if(cursor->next == nxt)
            break;
        cursor = cursor->next;
    }
 
    if(cursor != NULL)
    {
        node* new_node = create(iNodeNum,cursor->next);
        cursor->next = new_node;
        return head;
    }
    else
    {
        return NULL;
    }
}
 
/*
    traverse the linked list
*/
void traverse(node* head,callback f)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}
/*
    remove node from the front of list
*/
node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    return head;
}
 
/*
    remove node from the back of the list
*/
node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
 
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
 
    return head;
}
 
/*
    remove a node from the list
*/
node* remove_any(node* head,node* nd)
{
    if(nd == NULL)
        return NULL;
    /* if the node is the first node */
    if(nd == head)
        return remove_front(head);
 
    /* if the node is the last node */
    if(nd->next == NULL)
        return remove_back(head);
 
    /* if the node is in the middle */
    node* cursor = head;
    while(cursor != NULL)
    {
        if(cursor->next == nd)
            break;
        cursor = cursor->next;
    }
 
    if(cursor != NULL)
    {
        node* tmp = cursor->next;
        cursor->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }
    return head;
 
}
/*
    display a node
*/
void display(node* n)
{
    if(n != NULL)
        printf("%d ", n->iNodeNum);
}
 
/*
    Search for a specific node with input iNodeNum
 
    return the first matched node that stores the input iNodeNum,
    otherwise return NULL
*/
node* search(node* head, char *sMac1, char *sMac2, char *sIp1, char *sIp2)
{
 
    node *cursor = head;
    while(cursor!=NULL)
    {
        if(!strcmp(cursor->sMacSend, sMac1) &&
           !strcmp(cursor->sMacRec,  sMac2)  &&
           !strcmp(cursor->sIPSend,  sIp1)  &&
           !strcmp(cursor->sIPRec,   sIp2))
        {
              return cursor;
        }       
        cursor = cursor->next;
    }
    return NULL;
}
 
/*
    remove all element of the list
*/
void dispose(node *head)
{
    node *cursor, *tmp;
 
    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;

        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

/*
    Display all 
*/
void display_all(node *head, FILE *fpNetLogOut)
{
    if(!head)  fprintf(fpNetLogOut,"Empty\n");
    node *cursor;
    if(head != NULL)
    {
        cursor = head;
        while(cursor != NULL)
        {
            fprintf(fpNetLogOut,"Mac Sender:     %s\n",cursor->sMacSend);
            fprintf(fpNetLogOut,"IP Sender:      %s\n",cursor->sIPSend);
            fprintf(fpNetLogOut,"Mac Receiver:   %s\n",cursor->sMacRec);
            fprintf(fpNetLogOut,"IP Receiver:    %s\n",cursor->sIPRec);
            fprintf(fpNetLogOut,"Package Count:  %d\n",cursor->iNumPaquetes);
            fprintf(fpNetLogOut,"Bytes Number:   %d\n",cursor->iNumBytes);
            fprintf(fpNetLogOut,"First Time:     %s\n",cursor->sTimeIni);
            fprintf(fpNetLogOut,"Last Time:      %s\n",cursor->sTimeLast);

            fprintf(fpNetLogOut,"-------------------------------------------\n");
            cursor  = cursor->next;
        }
    }
}

/*
    return the number of elements in the list
*/
int count(node *head)
{
    node *cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}
/*
    sort the linked list using insertion sort
*/
node* insertion_sort(node* head)
{
    node *x, *y, *e;
 
    x = head;
    head = NULL;
 
    while(x != NULL)
    {
        e = x;
        x = x->next;
        if (head != NULL)
        {
            if(e->iNodeNum > head->iNodeNum)
            {
                y = head;
                while ((y->next != NULL) && (e->iNodeNum> y->next->iNodeNum))
                {
                    y = y->next;
                }
                e->next = y->next;
                y->next = e;
            }
            else
            {
                e->next = head;
                head = e ;
            }
        }
        else
        {
            e->next = NULL;
            head = e ;
        }
    }
    return head;
}
 
/*
    reverse the linked list
*/
node* reverse(node* head)
{
    node* prev    = NULL;
    node* current = head;
    node* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}


