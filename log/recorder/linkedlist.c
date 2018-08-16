#include "linkedlist.h"
Node * createnode(char* data,unsigned int len)
{
  Node * newNode = malloc(sizeof(Node));
  newNode->data = data;
  newNode->len=len;
  newNode->next = NULL;
  return newNode;
}

List* init_list()
{
  List * list = malloc(sizeof(List));
  list->head = NULL;
  return list;
}

void display(List * L)
{
  Node * current = L->head;
  if(L->head == NULL)
    return;
  while(current->next != NULL)
  {
    printf("%s ", current->data);
    printf("%u\n",current->len);
    current = current->next;
  }
  printf("%s ", current->data);
  printf("%d\n",current->len);
}

void insert_to_list(char* data,unsigned int len, List *L)
{
  Node * current = NULL;
  if(L->head == NULL){
    L->head = createnode(data,len);
  }
  else {
    current = L->head;
    while (current->next!=NULL){
      current = current->next;
    }
    current->next = createnode(data,len);
  }
}

void destroy(List * L)
{
  Node * current = L->head;
  Node * next = current;
  while(current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }
  free(L);
  L=NULL;
}
