#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../includes/paragraph.h"
#include "../includes/file_struct.h"

#define NUMBER_OF_PARAGRAPHS 100

int my_max(int a, int b){ return (a > b) ? a : b; }

paragraph* paragraph_constructor(char** content, int begin, int end){
  paragraph* para = (paragraph*)malloc(sizeof(paragraph));
  para->master_content = content;
  para->begin = begin;
  para->end = end;
  para->size = (end-begin)+1;
  return para;
}

paragraph_network* paragraph_network_constructor(){
  paragraph_network* network = (paragraph_network*)malloc(sizeof(paragraph_constructor));
  network->size = 0;
  network->stack_pointer = 0;
  network->paragraph_nodes = (paragraph*)malloc(NUMBER_OF_PARAGRAPHS * sizeof(paragraph));
  return network;
}

void paragraph_destructor(paragraph* para){
  free(para);
}

int paragraph_size(paragraph* p){ return (p->end - p->begin)+2; }

void paragraph_network_destructor(paragraph_network* network){
  for(int i = 0; i < network->size; ++i){
    paragraph_destructor(network->paragraph_nodes[i]);
  }
}

void paragraph_network_add_paragraph(paragraph_network* network, paragraph* para){
  if(network->size > NUMBER_OF_PARAGRAPHS){
    fprintf(stderr, "cannot insert into network, too many nodes\n");
    return;
  }
  network->paragraph_nodes[network->size++] = para;
}

void paragraph_print(paragraph* para, void (*fp)(const char*)){
  for(int i = para->begin; i < para->end; ++i){
    fp(para->master_content[i]);
  }
  printf("\n");
}

bool paragraph_equal(paragraph* p, paragraph* q){
  if(paragraph_size(p) != paragraph_size(q)){ return false; }
  int i = p->begin, j = q->begin;
  for(i, j; i < p->end || j < q->end; ++i, ++j){
    if(i >= p->end){ return false; }
    else if(strcmp(p->master_content[i], q->master_content[j]) != 0){ return false; }
  }
  return true;
}

bool paragraph_network_equal(paragraph_network* p, paragraph_network* q){
  if(p->size != q->size){ return false; }
  for(int i = 0, j = 0; i < p->size && j < q->size; ++i, ++j){
    if(!paragraph_equal(p->paragraph_nodes[i], q->paragraph_nodes[j])){ return false; }
  }
  return true;
}

paragraph* para_next(struct file_* a, paragraph* p) {
  if (p == NULL || p->end == a->length) { return NULL; }
  
  int i;
  paragraph* pnew = paragraph_constructor(p->master_content, p->end + 1, p->end + 1);
  for (i = pnew->begin; i < a->length && strcmp(p->master_content[i], "\n") != 0; ++i) { }
  pnew->end = i;
  
  if (pnew->begin >= a->length) {
    free(pnew);
    pnew = NULL;
  }
  return pnew;
}

paragraph* para_first(struct file_* a) {
  paragraph* p = paragraph_constructor(a->contents, 0, -1);
  return para_next(a, p);
}

void slice(struct file_* a){
  paragraph* current = para_first(a);
  while(current != NULL){
    paragraph_network_add_paragraph(a->para_network, current);
    current = para_next(a, current);
  }
}
