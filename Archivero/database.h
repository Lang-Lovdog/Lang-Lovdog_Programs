#ifndef __Lang_Archivero_database__
#define __Lang_Archivero_database__

// Database
/* This header is made for the library (archivero in spanish) program.
 * The program is aimed to categorize files, so there'll be easy to
 * reach them and keep track of how much files do we have on our dir
 * tree and giving the user a better way to manage them.
 */

#include "lio_tools.h"
#include "lovdog_char.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#ifndef ETIQUETAS_DB_HEAD
#define ETIQUETAS_DB_HEAD "Etiquetas,"
#endif
#ifndef ETIQUETAS_ID_DB_HEAD
#define ETIQUETAS_ID_DB_HEAD "Etiquetas ID,"
#endif
#ifndef NOMBRE_BASE_DATOS
#define NOMBRE_BASE_DATOS ".archivero_fm"
#endif


#ifndef __LOVDOG_ACTIVA_ARCHIVO_INFO__
#define __LOVDOG_ACTIVA_ARCHIVO_INFO__
struct archivo_info{
  char *tipo;
  lovdog_texto *etiquetas; // ID de las etiquetas
  lovdog_texto *nombre;
};
#include "dirlist.h"
#endif

struct archivador{
  lovdog_texto *etiquetas;
  lovdog_texto *etiqueta_id;
  tabla_desc *tabla;
};

typedef struct archivador archivador;

// Inicializa base de datos en el disco
void init_database();
// Busca un el archivo en el disco, desde el directorio actual
char* buscar_archivo_recursivo_reversa(char* archivo);
// Obtiene el directorio padre
void directorio_anterior(char* direccion);
// Recupera toda la información de la base de datos
void lee_database(struct archivador **archivero);
// Crea una entidad archivador
struct archivador* init_archivador();
// Elimina archivador
void free_archivador(struct archivador **archivero);
/* Verifica que'l archivo contenga las etiquetas dadas.
 * El texto de etiquetas debe estar tokenizado.
 * El último argumento es para saber si se trata de 
 * etiqueta (1) o familia (2) */
char archivo_etiquetas(struct archivador *archivero, lovdog_texto *etiquetas, char modo);
// Imprimir etiquetas
void imprime_etiquetas(struct archivador *archivero);
// Etiqueta archivo
char etiqueta_archivo(struct archivador *archivero, lovdog_texto *etiqueta);
// Etiqueta archivo
char etiquetaid_archivo(struct archivador *archivero, lovdog_texto *_etiqueta_id);
// Emparenta archivo
char emparenta_archivo(struct archivador *archivero, lovdog_texto *etiqueta);
/* Esta función permite recibir una ID o una Etiqueta y asigna el 
 * valor correspondiente a su contraparte. La función recibe 3
 * argumentos: un archivador*, donde se realizará la búsqueda;
 * un lovdog_texto*, que corresponde a la ID;
 * un lovdog_texto*, que corresponde a la etiqueta; */
char etiqueta_id(struct archivador* archivero, lovdog_texto** id, lovdog_texto** etiqueta);
/* Después de saber dónde estoy, quiero saber la posición
 * relativa desde la base de datos */
lovdog_texto* ubicacion_relativa_desde_bd();
/* Agregar archivo a la base de datos */
char agrega_archivo(struct archivador* archivero, char* archivo);
/* Agregar etiqueta a la base de datos */
char agrega_etiqueta(struct archivador* archivero, char* etiqueta);
/* Agregar archivo a la base de datos lovdog_texto */
char agrega_archivo_lt(struct archivador* archivero, lovdog_texto* archivo);
/* Agregar etiqueta a la base de datos lovdog_texto */
char agrega_etiqueta_lt(struct archivador* archivero, lovdog_texto* etiqueta);
/* Obtener directorio padre */
lovdog_texto* lt_archivo_padre(char* archivo);
/* Obtener nombre de archivo */
lovdog_texto* lt_archivo_nombre_base(char* archivo);
/* Deduce la información del archivo */
archivo_info* archivo_info_define(char* archivo);
/* Define archivo */
archivo_desc* archivo_define(unsigned char d_type, char* archivo_nme);
/* Reescribe archivero */
char ReescribeBaseDeDatos(struct archivador* archivero);

#endif
