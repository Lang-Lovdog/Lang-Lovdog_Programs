#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
#include "dirlist.h"
#include "database.h"
#include "lovdog_char.h"

int main(int argc, char** argv){
  lovdog_texto* lt = NULL;
  char cadena[20]="1,2,3,4,5,6";
  // Añadimos el texto
  lovdog_strappend(&lt, cadena);
  // Reiniciamos el iterador
  lovdog_texto_circula(&lt, -1);
  if(!lt->actual) printf("Iterador reiniciado\n");
  // Buscamos carácter en el texto
  int error_busqueda=0;
  error_busqueda=lovdog_texto_busca(&lt, ',', 0);
  switch(error_busqueda){
    case -2: printf("No hubo ocurrencia\n"); break;
    case -1: printf("No existe el texto\n"); break;
    case 0: printf("Se encontro la ocurrencia\n"); break;
    case 1: printf("Final del texto\n"); break;
    case 2: printf("Principio del texto\n"); break;
  }
  lovdog_textofree(&lt);
  return 0;
}
