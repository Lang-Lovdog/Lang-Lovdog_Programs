#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
#include "lio_tools.h"
#include "dirlist.h"
#include "database.h"
#include "lovdog_char.h"

int main(int argc, char** argv){
  if(argc>1){
    if(!strcmp(argv[1], "init")){
      init_database();
      return 0;
    }
  }
  tabla_desc* tabla_elementos = NULL;
  archivador* archivero = NULL;
  lovdog_texto *mi_archivo=NULL;
  lovdog_texto *mi_archivo1=NULL;
  lovdog_texto *etiqueta1=NULL;
  lovdog_texto *etiqueta2=NULL;
  lee_database(&archivero);
  if(!archivero) return 1;
  char *_mi_archivo=NULL;
  char arch1[PATH_MAX]="main.c";
  char arch2[PATH_MAX]="OtroDirectorio/Tarea1.docx/";
  char tag1[PATH_MAX]="Tareas";
  char tag2[PATH_MAX]="EA_2025";

  lovdog_texto_strappend(&mi_archivo, "main.c");
  lovdog_texto_strappend(&mi_archivo1, "OtroDirectorio/Tarea1.docx/");
  lovdog_texto_strappend(&etiqueta1, "Tarea");
  lovdog_texto_strappend(&etiqueta2, "MIE.BA04.09 ELECTRICIDAD Y MAGNETISMO");


  _mi_archivo=lovdog_texto_str(&mi_archivo);
  agrega_archivo(archivero, _mi_archivo);
  tabla_archivo_busca(archivero->tabla, _mi_archivo);
  tabla_cout(archivero->tabla);
  imprime_etiquetas(archivero);
  agrega_etiqueta(archivero, tag1);
  imprime_etiquetas(archivero);
  agrega_etiqueta(archivero, tag2);
  imprime_etiquetas(archivero);
  free(_mi_archivo);

  printf("\nAñadiendo etiqueta "); lovdog_texto_cout(etiqueta1);
  printf(" a "); lovdog_texto_cout(mi_archivo); printf("\n\n");
  _mi_archivo=lovdog_texto_str(&mi_archivo1);
  tabla_archivo_busca(archivero->tabla, _mi_archivo);
  char salida_err=0; salida_err=etiqueta_archivo(archivero,etiqueta1);
  if(salida_err) printf("Error al etiquetar archivo: %d\n", salida_err);
  salida_err=0; salida_err=etiqueta_archivo(archivero,etiqueta1);
  if(salida_err) printf("Error al etiquetar archivo ,: %d\n", salida_err);
  lovdog_textofree(&etiqueta1);

  printf("\nAñadiendo etiqueta "); lovdog_texto_cout(etiqueta2);
  printf(" a "); lovdog_texto_cout(mi_archivo); printf("\n\n");
  salida_err=etiqueta_archivo(archivero,etiqueta2);
  if(salida_err) printf("Error al etiquetar archivo .: %d\n", salida_err);
  lovdog_textofree(&etiqueta2);

  //tabla_cout(archivero->tabla);
  lovdog_textofree(&mi_archivo);


  mi_archivo=ubicacion_relativa_desde_bd();
  //printf("\n\nEstoy en: ");
  //lovdog_texto_cout(mi_archivo);
  //printf("\n\n");
  //lovdog_textofree(&mi_archivo);

  agrega_archivo(archivero,_mi_archivo);
  agrega_archivo(archivero,_mi_archivo);

  printf("\n");
  ReescribeBaseDeDatos(archivero); 
  printf("\n");

  free(_mi_archivo); _mi_archivo=lovdog_texto_str(&mi_archivo1);
  lovdog_textofree(&mi_archivo1);
  free(_mi_archivo);
  free_archivador(&archivero);
  return 0;
}
