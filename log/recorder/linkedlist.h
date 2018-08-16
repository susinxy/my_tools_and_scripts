#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char* data;
  unsigned int len;
  struct node * next;
} Node;

typedef struct linkedlist
{
  Node * head;
} List;

Node * createnode(char* data,unsigned int len);
List * init_list();
void insert_to_list(char* data,unsigned int len, List * L);
void display(List* L);
void destroy(List* L);
#endif // LINKEDLIST_H_INCLUDED
