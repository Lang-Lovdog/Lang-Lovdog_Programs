#ifndef __ARCHIVERO_MIAN__
#define __ARCHIVERO_MIAN__
#include "lovdog_char.h"

struct opciones{
  char* instruccion;
  void (*funcion)();
  size_t nargs;
  struct opciones* sig;
  struct opciones* ant;
};

struct acciones{
  size_t cantidad;
  struct opciones* acciones_primer;
  struct opciones* acciones_ultimo;
  struct opciones* actual;
};

void maneja_argumentos(int argc, char* argv[], struct acciones* acciones);
void main_agrega_archivo(char* archivo);
void main_agrega_etiqueta(char* etiqueta);
void main_etiqueta_archivo(char* archivo, char* etiqueta);
void main_analiza_directorios();

struct acciones* crea_acciones();
char agrega_accion(
    struct acciones** acciones,
    char* instruccion,
    void (*funcion)()
);
void ejecuta_accion(
    void (*funcion)(),
    size_t nargs,
    int argc,
    char* argv[]
);
void busca_accion(struct acciones* acciones, char* instruccion);
void free_acciones(struct acciones** acciones);
#endif
