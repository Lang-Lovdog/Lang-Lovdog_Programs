#ifndef Lovdog_C_DICT_H
#define Lovdog_C_DICT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR 2


typedef struct dict_T {
  char *key;
  char *value;
  struct dict_T *next;
} dict_T;

typedef struct dict {
  dict_T *head;
  size_t size;
} dict;
void  lovdog_dict_ch (dict* d,const char *key,char *value);
void  lovdog_dict_rm (dict* d,const char *key);
void  lovdog_dict_add(dict* d,char *key,char *value);
char* lovdog_dict_get(const dict* d,const char *key);
char  lovdog_dict_duplicate(const dict* d,const char *key);
void  lovdog_dict_wo_keshimasu(dict* d);
void  lovdog_dict_create_dict(dict* d);
unsigned int lovdog_dict_print_rec(dict *d);

/*_________________________________________________________*/
unsigned int lovdog_dict_print_rec(dict *d) {
  if(!d->head) return 0;
  if(!d->size) return 0;
  dict_T *current = d->head;
  unsigned int count = 0;
  printf("{\n");
  while(current){
    printf("  %s: %s\n",current->key,current->value);
    current = current->next;
    ++count;
  }
  printf("}");
  return count;
}
char lovdog_dict_duplicate(const dict* d,const char *key) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key)
      return 1;
    current = current->next;
  }
  return 0;
}
void lovdog_dict_create_dict(dict* d) {
  d->head = NULL;
  d->size = 0;
}
void lovdog_dict_add(dict* d,char *key,char *value) {
  if(!key) return;
  if(!value) return;
  if(!strcmp(key,"")) return;
  if(!strcmp(value,"")) return;
  if(lovdog_dict_duplicate(d,key)) return;

  dict_T *new = malloc(sizeof(dict_T));
  new->key=(char*)malloc(sizeof(char)*(strlen(key)+1));
  new->value=(char*)malloc(sizeof(char)*(strlen(value)+1));

  strcpy(new->key,key);
  strcpy(new->value,value);
  new->next = d->head;
  d->head = new;
  ++d->size;
}
char* lovdog_dict_get(const dict* d, const char *key) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key)
      return current->value;
    current = current->next;
  }
  return NULL;
}
void lovdog_dict_rm(dict* d,const char *key) {
  dict_T
    *current = d->head,
    *prev = NULL
  ;
  while (current){
    if (current->key == key){
      if(current->key) free(current->key);
      if(current->value) free(current->value);
      if (prev){
        prev->next = current->next;
        current->next = NULL;
        current->key = NULL;
        current->value = NULL;
        --d->size;
        free(current);
        break;
      }
      d->head = current->next;
      break;
    }
    prev = current;
    current = current->next;
  }
}
void lovdog_dict_ch(dict* d,const char *key,char *value) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key){
      current->value = value;
      return;
    }
    current = current->next;
  }
}
void lovdog_dict_wo_keshimasu(dict* d) {
  dict_T *current = d->head;
  dict_T *next = NULL;
  while (current){
    next = current->next;
    if(current->key) free(current->key);
    if(current->value) free(current->value);
    if(current) free(current);
    current = next;
  }
}
#endif
