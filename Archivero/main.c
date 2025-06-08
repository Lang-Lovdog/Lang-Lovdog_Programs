#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
#include "lio_tools.h"
#include "dirlist.h"
#include "database.h"
#include "lovdog_char.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

  lovdog_texto* lt=NULL;
  char* basedatos=NULL;
  basedatos=buscar_archivo_recursivo_reversa(NOMBRE_BASE_DATOS);
  //lt=lt_archivo_padre(basedatos);
  lovdog_texto_strappend(&lt,basedatos);
  lovdog_texto_cout(lt);
  lovdog_textofree(&lt); free(basedatos);
  return 0;
}

void main_agrega_archivo(char* archivo){}
void main_agrega_etiqueta(char* etiqueta){}
void main_etiqueta_archivo(char* archivo, char* etiqueta){}
void main_analiza_directorios(){}


void lista_acciones(struct acciones* acciones){
  agrega_accion( &acciones, "agrega-archivo", NULL, main_agrega_archivo);
  agrega_accion(
    &acciones,
    "estado",
    NULL,
    main_analiza_directorios
  );
  agrega_accion(
    &acciones,
    "agrega-archivo",
    NULL,
    main_agrega_archivo
  );
  agrega_accion(
    &acciones,
    "agrega-archivo",
    NULL,
    main_agrega_archivo
  );
}



void maneja_argumentos(int argc, char* argv[], struct acciones* acciones){
  for(int a=1; a<argc; a++){
    busca_accion(acciones, argv[a]);
    if(acciones){
      if(acciones->actual->funcion)
        acciones->actual->funcion(acciones, argv[++a]);
      break;
    }
  }
}

void ejecuta_accion(
    void (*funcion)(),
    size_t nargs,
    int argc,
    char* argv[]
){
  if(nargs>argc) return;
  if(!funcion) return;
  funcion();
}

struct acciones* crea_acciones(){
  struct acciones* acciones=NULL;
  acciones=malloc(sizeof(struct acciones));
  acciones->cantidad=0;
  acciones->acciones_primer=
    acciones->acciones_ultimo=NULL;
  acciones->actual=NULL;
  return acciones;
}
char agrega_accion(
    struct acciones** _acciones,
    char* instruccion,
    void (*funcion)()
){
  if(!*_acciones) *_acciones=crea_acciones();
  if(!*_acciones) return -1;
  if(!instruccion) return -2;
  if(!funcion) return -3;
  /*   Crear acción  */
  struct opciones* accion=malloc(sizeof(struct opciones));
  accion->instruccion=strdup(instruccion);
  accion->funcion=funcion;
  accion->sig=accion->ant=NULL;
  /* --------------- */
  /* Conectar acción */
  if(!(*_acciones)->acciones_primer){
    (*_acciones)->acciones_primer=accion;
    (*_acciones)->acciones_ultimo=accion;
    return 0;
  }
  accion->ant=(*_acciones)->acciones_ultimo;
  (*_acciones)->acciones_ultimo->sig=accion;
  (*_acciones)->acciones_ultimo=accion;
  return 0;
}
void busca_accion(struct acciones* acciones, char* instruccion){
  struct opciones* actual=acciones->acciones_primer;
  while(actual){
    if(!strcmp(actual->instruccion, instruccion)){
      acciones->actual=actual;
      return;
    }
    actual=actual->sig;
  }
  acciones->actual=NULL;
}

void free_acciones(struct acciones** acciones){
  struct opciones* actual=(*acciones)->acciones_primer;
  struct opciones* siguiente=NULL;
  while(actual){
    siguiente=actual->sig;
    free(actual->instruccion);
    free(actual);
    actual=siguiente;
  }
  free(*acciones);
  *acciones=NULL;
}
