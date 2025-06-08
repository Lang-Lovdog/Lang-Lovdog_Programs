#ifndef __Lang_Archivero_dirman__
#define __Lang_Archivero_dirman__

// Directory exploration
/* This header is made for the library (archivero in spanish) program.
 * The program is aimed to categorize files, so there'll be easy to
 * reach them and keep track of how much files do we have on our dir
 * tree and giving the user a better way to manage them.
 */

// In order to access the posix way, we use this library
#include "lio_tools.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dirent dirent;

#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
/*Archivo Info*/
// La estructura está definida sin mayor detalle
// por lo que será únicamente requerido en caso de
// querer un apartado extra en la estructura archivo
typedef struct archivo_info archivo_info;
archivo_info* init_archivo_info();
void cout_archivo_info(archivo_info* info);
void cout_archivo_info_tab(archivo_info* info, size_t lcout_tab);
char mod_archivo_info(archivo_info* destino, archivo_info* nuevo);
char free_archivo_info(archivo_info* info);
/*Archivo Info*/
#endif

typedef struct archivo_desc {
  char* direccion;
  unsigned char tipo;
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
  archivo_info *info;
#endif
  size_t indice;
  struct archivo_desc *sig;
  struct archivo_desc *ant;
} archivo_desc;

typedef struct tabla_desc {
  archivo_desc *primer_archivo;
  archivo_desc *ultimo_archivo;
  archivo_desc *actual;
  size_t total;
} tabla_desc;

/* This function is for creating a new file list */
archivo_desc* crea_archivo();
archivo_desc* archivo_init(size_t size);
char tabla_archivo_add(tabla_desc* tabla, archivo_desc* arch);
char tabla_archivo_rm(tabla_desc* tabla, size_t* indice);
char tabla_archivo_rma(tabla_desc* tabla, archivo_desc* arch);
tabla_desc* tabla_init();

/* This function takes two parameters: the path to be searched
 * and a pointer which will contain the files retrieved
 */
char llena_tabla(char* path, tabla_desc** list);
char llena_tabla_recursivo(char* path, tabla_desc** list);
char archivo_mod(void *valor, const char atributo, archivo_desc* archivo);
char archivo_en_tabla(size_t indice, tabla_desc* tabla);
char archivo_existe(tabla_desc* tabla, char* path);
long int archivo_busca(tabla_desc* tabla, char* arch);
char archivo_tipo_str(unsigned char** d_type, char** str);
char circula_tabla(tabla_desc* tabla);
char circula_tablar(tabla_desc* tabla);
void regresa_tabla(tabla_desc* tabla, char reversa);
void archivo_cout(archivo_desc* arch);
void tabla_cout(tabla_desc* tabla);
char archivo_free(archivo_desc** arch);
long int tabla_archivo_busca(tabla_desc* tabla, char* arch);
char tabla_free(tabla_desc** tabla);
void define_tabulador(size_t tabulador);

#endif
