#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
#include "lovdog_char.h"

int main(int argc, char** argv){
  lovdog_texto* lt = NULL;
  char cadena[200]="1,2,3,4,5,63248297,,,,,,,,";
  lovdog_strappend(&lt, cadena); // Añadir el texto
  lovdog_texto_circula(&lt, -1); // Reiniciar iterador
  if(!lt->actual) printf("Iterador reiniciado\n");
  int error_busqueda=0;
  if(lovdog_texto_token(&lt, ",")) return error_return("Error", -1);
  printf("%ld tokens encontrados\n", lt->token_cantidad);
  for(int a=0; a<lt->token_cantidad; a++){
    printf("%2d: de %ld caracteres ", a, lt->token_longitud[a]);
    printf("\n");
  }
  error_busqueda=lovdog_textok_cout(lt, -1);
  switch(error_busqueda){
    case -1: printf("No existe el texto\n"); break;
    case -2: printf("Indice fuera de rango\n"); break;
    case -3: printf("No hay tokens\n"); break;
  }
  lovdog_textofree(&lt);
  return 0;
}

