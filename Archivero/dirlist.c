#include "dirlist.h"

int cout_tab=0;

archivo_desc* crea_archivo(){
  /* Creación de nodos*/
  archivo_desc* nuevo_archivo=NULL;
  nuevo_archivo = (archivo_desc*)malloc(sizeof(archivo_desc));
  if(!nuevo_archivo) return NULL;
  nuevo_archivo->sig = NULL;
  nuevo_archivo->ant = NULL;
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
  nuevo_archivo->info = NULL;
#endif
  nuevo_archivo->direccion = NULL;
  nuevo_archivo->tipo = 0;
  return nuevo_archivo;
}

archivo_desc* archivo_init(size_t size){
  if(size<1) return NULL;
  archivo_desc
    *primer_elemento=NULL,
    *aux=NULL;
  --size;
  primer_elemento = crea_archivo();
  aux = primer_elemento;
  aux->indice = 0;
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
    aux->info = init_archivo_info();
#endif
  while(size){
    --size;
    aux->sig = crea_archivo();
    aux->sig->indice = aux->indice+1;
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
    aux->info = init_archivo_info();
#endif
    aux->sig->ant = aux;
    aux = aux->sig;

  }
  return primer_elemento;
}

char tabla_archivo_add(tabla_desc* tabla, archivo_desc* arch){
  if(!tabla || !arch) return -1;
  if(!tabla->ultimo_archivo && tabla->primer_archivo) return -1;
  if(!tabla->primer_archivo && tabla->ultimo_archivo) return -1;
  if(!tabla->primer_archivo) tabla->primer_archivo = arch;
  else tabla->ultimo_archivo->sig = arch;
  arch->ant = tabla->ultimo_archivo;
  arch->sig = NULL;
  arch->indice = tabla->total;
  tabla->ultimo_archivo = arch;
  ++tabla->total;
  return 0;
}

char archivo_rm(tabla_desc* t, char i){
  if(!t) return -1;
  if(!t->total) return -1;
  if(!t->primer_archivo) return -1;
  if(!t->ultimo_archivo) return -1;
  archivo_desc
    *actual=t->actual,
    *objetivo=NULL,
    *anterior=NULL,
    *siguient=NULL;
  if(actual->indice != i)
    archivo_en_tabla(i, t);
  objetivo = t->actual;
  anterior = objetivo->ant;
  siguient = objetivo->sig;
  anterior->sig = siguient;
  siguient->ant = anterior;
  objetivo->sig = NULL;
  objetivo->ant = NULL;
  archivo_free(&objetivo);
  // Reacomodar indices de la tabla
  while(siguient){
    --siguient->indice;
    siguient = siguient->sig;
  }
  --t->total; // Un elemento menos
  if(!objetivo) return 0;
  t->actual=actual;
  return 0;
}

char archivo_rma(tabla_desc* t, archivo_desc* a){
  archivo_desc
    *objetivo=NULL,
    *anterior=NULL,
    *siguient=NULL;
  objetivo = a;
  anterior = objetivo->ant;
  siguient = objetivo->sig;
  anterior->sig = siguient;
  siguient->ant = anterior;
  objetivo->sig = NULL;
  objetivo->ant = NULL;
  archivo_free(&objetivo);
  // Reacomodar indices de la tabla
  while(siguient){
    --siguient->indice;
    siguient = siguient->sig;
  }
  --t->total; // Un elemento menos
  return 0;
}

tabla_desc* tabla_init(){
  tabla_desc* nueva_tabla=NULL;
  nueva_tabla = (tabla_desc*)malloc(sizeof(tabla_desc));
  if(!nueva_tabla) return NULL;
  nueva_tabla->primer_archivo = NULL;
  nueva_tabla->ultimo_archivo = NULL;
  nueva_tabla->actual = NULL;
  nueva_tabla->total = 0;
  return nueva_tabla;
}

char llena_tabla(char* path, tabla_desc** list){
  DIR *dir;            // Apuntador al directorio en path
  dirent *entidad;        // Tabla de direcciones
  tabla_desc *la_tabla; 

  if(!path)
    return error_return("La dirección es NULL", -1);

  // Creación de tabla de elementos
  if(!*list){
    la_tabla = tabla_init();
    if(!la_tabla)
      return error_return("No se pudo crear tabla", -2);
    *list = la_tabla;
  }
  // Acceso al directorio
  dir = opendir(path);
  if(!dir){
    tabla_free(list);
    return error_return("No se pudo abrir el directorio", -3);
  }
  // Reservamos memoria para cada elementos del directorio
  if (la_tabla->primer_archivo || la_tabla->ultimo_archivo)
    return error_return("Tabla no vacía", -1);
  archivo_desc* _archivo=NULL;
  // Recorremos el directorio y guardamos uno a uno los elementos
  // encontrados dentro de la tabla
  while ((entidad=readdir(dir))){
    // Ignoramos . y ..
    if(!strcmp(entidad->d_name, ".") || !strcmp(entidad->d_name, "..")) continue;
    // Creamos el elemento
    _archivo = crea_archivo();
    if(!_archivo)
      return error_return("No se pudo crear tabla", -4);
    // Definimos la dirección del elemento
    archivo_mod(&entidad->d_name,'d',_archivo);
    // Definimos el tipo del elemento
    archivo_mod(&entidad->d_type,'t',_archivo);
    // Anexamos el elemento a la tabla
    if(tabla_archivo_add(la_tabla, _archivo))
      error_return("No se pudo anexar elemento", -5);
    _archivo = NULL;
  }
  // Cerramos el directorio (liberamos recursos)
  closedir(dir); 
  // Regresamos el recuento d'elementos nl'directorio
  return 0;
}

char llena_tabla_recursivo(char* path, tabla_desc** list){
  DIR *dir;            // Apuntador al directorio en path
  dirent *entidad;        // Tabla de direcciones
  tabla_desc *la_tabla; 

  if(!path)
    return error_return("La dirección es NULL", -1);

  // Creación de tabla de elementos
  if(!*list){
    la_tabla = tabla_init();
    if(!la_tabla)
      return error_return("No se pudo crear tabla", -2);
    *list = la_tabla;
  }
  // Acceso al directorio
  dir = opendir(path);
  if(!dir){
    tabla_free(list);
    return error_return("No se pudo abrir el directorio", -3);
  }
  // Reservamos memoria para cada elementos del directorio
  if (la_tabla->primer_archivo || la_tabla->ultimo_archivo)
    return error_return("Tabla no vacía", -1);
  archivo_desc* _archivo=NULL;
  // Recorremos el directorio y guardamos uno a uno los elementos
  // encontrados dentro de la tabla
  while ((entidad=readdir(dir))){
    // Ignoramos . y ..
    if(!strcmp(entidad->d_name, ".") || !strcmp(entidad->d_name, "..")) continue;
    // Creamos el elemento
    _archivo = crea_archivo();
    if(!_archivo)
      return error_return("No se pudo crear tabla", -4);
    // Definimos la dirección del elemento
    archivo_mod(&entidad->d_name,'d',_archivo);
    // Definimos el tipo del elemento
    archivo_mod(&entidad->d_type,'t',_archivo);
    // Anexamos el elemento a la tabla
    if(tabla_archivo_add(la_tabla, _archivo))
      error_return("No se pudo anexar elemento", -5);
    // Si es un directorio, llamamos recursivamente
    if(entidad->d_type==DT_DIR)
      llena_tabla_recursivo(entidad->d_name, list);
    _archivo = NULL;
  }
  // Cerramos el directorio (liberamos recursos)
  closedir(dir); 
  // Regresamos el recuento d'elementos nl'directorio
  return 0;
}

/* Modificar un archivo, segun el parámetro 'a'
 * d: direccion, t: type, i: info (únicamente si se activa)
*/
char archivo_mod(void *v, const char a, archivo_desc* f){
  char* valor=NULL;
  switch(a){
    case 'd': // Para la dirección del elemento
      // Solo, que sea legible la variable
      valor=(char*)v;
      // Y la longitud de la cadena, incluyendo el '\0'
      size_t len=strlen(valor)+1;
      // Si existía un valor previo, se libera
      if(f->direccion) free(f->direccion);
      // Reserva de memoria para la nueva cadena
      f->direccion = (char*)malloc(sizeof(char)*len);
      // Asignación del valor (copiamos la cadena para evitar errores
      // de acceso dados por una dirección de memoria no explícitamente
      // gestionada; sí, pasa eso).
      strcpy(f->direccion, valor);
      // Y, ¡listo! :3
      break;
    case 't': // Para el tipo de entidad
      // Una simple asignación, el valor es para una
      // variable de tipo entero, nada que ver con apuntadores.
      // Únicamente, se requiere el cast a (int*) porque el
      // apuntador originalmente es void*
      f->tipo = *((unsigned char*)v);
      break;
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
    case 'i':// Para la información del elemento
      // La información también es un apuntador void
      // eso quiere decir que debe ser alterado
      // únicamente por métodos que lo definan
      // por lo tanto... pues, nada que hacer 
      // Pero, aún si no se hace gran cosa, se debe
      // realizar una buena gestión de memoria.
      free_archivo_info(f->info);
      f->info = init_archivo_info();
      mod_archivo_info(f->info, (archivo_info*)v);
      break;
#endif
  }
  return 0;
}

char archivo_en_tabla(size_t i, tabla_desc* t){
  if(!t) return -1;
  if(!t->total) return -1;
  if(!t->primer_archivo) return -1;
  if(!t->ultimo_archivo) return -1;
  if(i >= t->total) return -1;
  if(i < 0) return -1;
  while(1){
    if(i<t->actual->indice) circula_tablar(t);
    if(i>t->actual->indice) circula_tabla(t);
    break;
  }
  return 0;
}

char circula_tabla(tabla_desc* t){
  if(!t) return -1;
  if(!t->total) return -1;
  if(!t->primer_archivo) return -1;
  if(!t->ultimo_archivo) return -1;
  if(!t->actual) return (t->actual = t->primer_archivo)? 0 : -1;
  if(!t->actual->sig) return 1;
  t->actual = t->actual->sig;
  return 0;
}

char circula_tablar(tabla_desc* t){
  if(!t) return -1;
  if(!t->total) return -1;
  if(!t->primer_archivo) return -1;
  if(!t->ultimo_archivo) return -1;
  if(!t->actual) return (t->actual = t->ultimo_archivo)? 0 : -1;
  if(!t->actual->ant) return 1;
  t->actual = t->actual->ant;
  return 0;
}

void regresa_tabla(tabla_desc* t, char r){
  t->actual = r? t->ultimo_archivo : t->primer_archivo;
}

char tabla_free(tabla_desc** tabla){
  if(!*tabla) return -1;
  if((*tabla)->total){
    archivo_desc* objetivo=(*tabla)->primer_archivo;
    archivo_desc* siguient=objetivo->sig;
    char err=0;
    while(1){
      err=archivo_free(&objetivo);
      if(err) printf("Error al liberar archivo: %d\n", err);
      if(!siguient) break;
      objetivo = siguient;
      siguient = objetivo->sig;
    }
  }
  free(*tabla);
  *tabla = NULL;
  return 0;
}

char archivo_free(archivo_desc** a){
  if(!*a) return -1;
  if((*a)->direccion) free((*a)->direccion);
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
  free_archivo_info((*a)->info);
#endif
  free(*a);
  *a = NULL;
  return 0;
}

long int tabla_archivo_busca(tabla_desc* tabla, char *arch){
  if(!tabla) return -1;
  if(!tabla->total) return -2;
  if(!tabla->primer_archivo) return -2;
  if(!tabla->ultimo_archivo) return -2;
  archivo_desc* objetivo=tabla->primer_archivo;
  while(objetivo){
    if(!strcmp(arch, objetivo->direccion)){
      tabla->actual = objetivo;
      return objetivo->indice;
    }
    objetivo = objetivo->sig;
  }
  return -3;
}

void archivo_cout(archivo_desc* archivo){
  if(!archivo) return;
  char *tipo=NULL;
  unsigned char *_tipo=&archivo->tipo;
  archivo_tipo_str(&_tipo, &tipo);
  TABULA;
  printf("%4s: %s\n", tipo, archivo->direccion);
  free(tipo);
#ifdef __LOVDOG_ACTIVA_ARCHIVO_INFO__
  cout_archivo_info_tab(archivo->info, cout_tab);
#endif
}

void tabla_cout(tabla_desc* tabla){
  if(!tabla) return;
  archivo_desc* actual=tabla->primer_archivo;
  TABULA;
  printf("Tabla[%zd]: {\n", tabla->total);
  cout_tab+=2;
  while(actual){ archivo_cout(actual); actual=actual->sig; }
  cout_tab-=2;
  TABULA;
  printf("}\n");
}

/* Busca la primera ocurrencia de un archivo con nombre (direacción)
 * y tipo (d_type) dentro de la tabla, regresa su indice y pone el
 * define el iterador en su posición */
long int archivo_busca(tabla_desc* tabla, char* arch){
  if(!tabla) return -1;
  if(!tabla->total) return -2;
  if(!tabla->primer_archivo) return -2;
  if(!tabla->ultimo_archivo) return -2;
  archivo_desc* actual=tabla->actual;
  archivo_desc* objetivo=tabla->primer_archivo;
  while(objetivo){
    if(!strcmp(arch, objetivo->direccion)){
      tabla->actual = objetivo;
      return objetivo->indice;
    }
    objetivo = objetivo->sig;
  }
  return -3;
}

// Convierte d_type a string y viceversa
char archivo_tipo_str(unsigned char** d_type, char** str){
  if(!d_type && !str) return -1;
  char tipos_str[8][5] =
    { "dir", "reg", "lnk", "chr", "blk", "fifo", "sock", "???" };
  unsigned char tipos_d_type[8] =
    { DT_DIR, DT_REG, DT_LNK, DT_CHR, DT_BLK, DT_FIFO, DT_SOCK, 0 };
  if(!*str){
    *str=(char*)malloc(5*sizeof(char));
    for(size_t i=0; i<8; ++i)
     if(tipos_d_type[i]==**d_type) strcpy(*str,tipos_str[i]);
    return error_return("Pide ID",0);
  }
  if(!*d_type){
    *d_type=(unsigned char*)malloc(sizeof(unsigned char));
    for(size_t i=0; i<8; ++i)
     if(!strcmp(tipos_str[i],*str)) **d_type=tipos_d_type[i];
    return error_return("Pide nombre",0);
  }
  return -2;
  
}
