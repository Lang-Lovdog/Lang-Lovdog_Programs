#include <stdio.h>
#include <stdlib.h>
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
void  ch (dict* d,const char *key,char *value);
void  rm (dict* d,const char *key);
void  add(dict* d,char *key,char *value);
char* get(const dict* d,const char *key);
char  duplicate(const dict* d,const char *key);
void  dict_wo_keshimasu(dict* d);
unsigned int print_rec(dict_T *d);

unsigned int print_rec(dict_T *d) {
  if(!d) return 0;
  dict_T *current = d;
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
char duplicate(const dict* d,const char *key) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key)
      return 1;
    current = current->next;
  }
  return 0;
}

void create_dict(dict* d) {
  d->head = NULL;
  d->size = 0;
}

void add(dict* d,char *key,char *value) {
  if (duplicate(d,key)) return;
  dict_T *new = malloc(sizeof(dict_T));
  new->key = key;
  new->value = value;
  new->next = d->head;
  d->head = new;
  ++d->size;
}

char* get(const dict* d, const char *key) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key)
      return current->value;
    current = current->next;
  }
  return NULL;
}
void rm(dict* d,const char *key) {
  dict_T
    *current = d->head,
    *prev = NULL
  ;
  while (current){
    if (current->key == key){
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
void ch(dict* d,const char *key,char *value) {
  dict_T *current = d->head;
  while (current){
    if (current->key == key){
      current->value = value;
      return;
    }
    current = current->next;
  }
}
void dict_wo_keshimasu(dict* d) {
  dict_T *current = d->head;
  while (current){
    dict_T *next = current->next;
    free(current);
    current = next;
  }
}


int main(int argc, char *argv[]) {
  dict
    d
  ;
  create_dict(&d);
  add(&d,"a","1");
  add(&d,"b","2");
  add(&d,"c","3");
  add(&d,"d","4");
  add(&d,"e","5");
  print_rec(d.head);
  if(get(&d,"a"))
    printf("%s\n",get(&d,"a"));
  rm(&d,"a");
  if(get(&d,"a"))
    printf("%s\n",get(&d,"a"));
  ch(&d,"b","Hola");
  if(get(&d,"b"))
    printf("%s\n",get(&d,"b"));
  dict_wo_keshimasu(&d);
  return 0;
}
