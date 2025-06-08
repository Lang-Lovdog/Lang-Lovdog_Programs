#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
#include "dirlist.h"
#include "database.h"
#include "lovdog_char.h"

int main(int argc, char** argv){
  lovdog_texto* lt = NULL;
  char cadena[20]="abcdef";
  // Añadimos el texto
  lovdog_strappend(&lt, cadena);
  // Reiniciamos el iterador
  lovdog_texto_circula(&lt, -1);
  if(!lt->actual) printf("Iterador reiniciado\n");
  // Circulamos en el texto
  lovdog_texto_circula(&lt, 0);// 0 Es hacia adelante
  if(lt->actual) printf("%c\n", lt->actual->c);
  lovdog_texto_circula(&lt, 0);
  if(lt->actual) printf("%c\n", lt->actual->c);
  lovdog_texto_circula(&lt, 1);// 1 Es hacia atras
  if(lt->actual) printf("%c\n", lt->actual->c);
  lovdog_textofree(&lt);
  return 0;
}
