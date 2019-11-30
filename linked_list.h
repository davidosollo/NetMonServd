/////////////////////////////////////////////////////////////////////////////
//  Source:      linked_list.h
//  Project:     NeMonServd
//  Subject:     Aplicacion y diseño de sistemas embebidos
//  Univerisity: Universidad Autonoma de Guadalajara
//  Description: Handle the linked listo header
//  
//

#define NODE_FIELD_LEN 100

typedef struct node {
        int iNodeNum;
        char sTimeIni[NODE_FIELD_LEN];
        char sTimeLast[NODE_FIELD_LEN];
        char sMacSend[NODE_FIELD_LEN];
        char sMacRec[NODE_FIELD_LEN];
        char sIPSend[NODE_FIELD_LEN];
        char sIPRec[NODE_FIELD_LEN];
        long iNumBytes;
        long iNumPaquetes;

	struct node *next;
} node;

typedef void (*callback)(node* data);

node* create(int data,node* next);
node* prepend(node* head,int data);
node* append(node* head, int data);
node* insert_after(node *head, int data, node* prev);
node* insert_before(node *head, int data, node* nxt);
void traverse(node* head,callback f);
node* remove_front(node* head);
node* remove_back(node* head);
node* remove_any(node* head,node* nd);
void display(node* n);
node* search(node* head, char *sMac1, char *sMac2, char *sIp1, char *sIp2);
void dispose(node *head);
int count(node *head);
node* insertion_sort(node* head);
node* reverse(node* head);
void display_all(node *head, FILE *fpNetLogOut);
void display(node* n);




