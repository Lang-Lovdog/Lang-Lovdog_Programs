#include "database.h"
#include "dirlist.h"
#include "lio_tools.h"
#include "lovdog_char.h"

// Busca un el archivo en el disco, desde el directorio actual
char* buscar_archivo_recursivo_reversa(char* archivo){
  char* path=NULL;
  char currentdir[PATH_MAX];
  char* homedir;
  char control=1;
  DIR *dir; // Directorio actual
  dirent *entidad; // Entidad

  // Obtenemos las cadenas que corresponden a los directorios
  getcwd(currentdir, PATH_MAX);
  strcat(currentdir, "/");
  homedir = getenv("HOME");
  strcat(homedir, "/");

  while (control){
    // Acceso al directorio
    dir = opendir(currentdir);
    if(!dir){
      error_return("No se pudo abrir el directorio", -1); return NULL;
    }
    while ((entidad=readdir(dir))){
      if(!strcmp(entidad->d_name, ".") || !strcmp(entidad->d_name, ".."))
        continue;
      if(!strcmp(entidad->d_name, archivo)){
        path=(char*)malloc(strlen(entidad->d_name)+strlen(currentdir)+1);
        strcpy(path, "");
        strcat(path, currentdir);
        strcat(path, entidad->d_name);
        closedir(dir);
        return path;
      }
    }
    closedir(dir);
    directorio_anterior(currentdir);
    if(!strcmp(currentdir, homedir)){
      control=0;
    }
  }
  return NULL;
}

// Obtiene el directorio padre
void directorio_anterior(char* direccion){
  char* ultimo_backsalsh=NULL;
  ultimo_backsalsh = strrchr(direccion, '/');
  /*Si el backslash corresponde al directorio raíz, no hacemos nada*/
  if(ultimo_backsalsh == direccion) return;
  /* Si no hay más directorios previos, no hacemos nada*/
  if(!ultimo_backsalsh) return;
  /* Si es último backslash, recorremos al siguiente*/
  if(ultimo_backsalsh == (direccion+strlen(direccion)-1)){
    *(ultimo_backsalsh) = '\0';
    ultimo_backsalsh=strrchr(direccion, '/');
  }
  *(ultimo_backsalsh+1) = '\0';
}

// Inicializa la base de datos en el disco, en el directorio actual
void init_database(){
  FILE* db=NULL;
  db = fopen(NOMBRE_BASE_DATOS, "r");
  if(db){
    printf("Base de datos ya inicializada\n");
    fclose(db);
    return;
  }
  db = fopen(NOMBRE_BASE_DATOS, "w");
  if(!db){
    error_return("No se pudo crear base de datos", -1);
    return;
  }
  fprintf(db, "%s\n", ETIQUETAS_DB_HEAD);
  fprintf(db, "%s\n", ETIQUETAS_ID_DB_HEAD);
  fclose(db);
}

void lee_database(struct archivador **archivero){
  char* bd_direccion=NULL;
  FILE *bd=NULL;
  lovdog_texto* bd_fila=NULL;

  bd_direccion = buscar_archivo_recursivo_reversa(NOMBRE_BASE_DATOS);
  if(!bd_direccion){
    error_return("No se pudo encontrar la base de datos\n"
        "Puede inicializarlo con init", -1);
    return;
  }

  if(!*archivero){
    *archivero = init_archivador();
    if(!*archivero){
      error_return("No se pudo crear el archivador", -1);
      return;
    }
    (*archivero)->tabla=tabla_init();
  }

  bd=fopen(bd_direccion, "r");
  if(!bd){
    error_return("No se pudo abrir la base de datos", -1);
    return;
  }

  char *auxiliar_string=NULL;
  // Primer línea del archivo se define de la forma:
  // Etiquetas, tag1, tag2, tag3, ...
  lovdog_getlined(bd,&(*archivero)->etiquetas, "\n");
  lovdog_texto_token(&(*archivero)->etiquetas, ",");
  lovdog_textok_trim(&(*archivero)->etiquetas, ' ');
  auxiliar_string=lovdog_textok_str(&(*archivero)->etiquetas, 0);
  if(strcmp(auxiliar_string, "Etiquetas")){
    error_return("Error de síntaxis, etiquetas no encontradas en [1,1]: ", 0);
    printf(">> "); lovdog_textok_cout((*archivero)->etiquetas, 0); printf("\n");
  }
  if(auxiliar_string) free(auxiliar_string);

  // Segunda línea del archivo se define de la forma:
  // Etiquetas ID, tag1 id, tag2 id, tag3 id, ...
  lovdog_getlined(bd,&(*archivero)->etiqueta_id, "\n");
  lovdog_texto_token(&(*archivero)->etiqueta_id, ",");
  lovdog_textok_trim(&(*archivero)->etiqueta_id, ' ');
  auxiliar_string=lovdog_textok_str(&(*archivero)->etiqueta_id, 0);
  if(strcmp(auxiliar_string, "Etiquetas ID")){
    error_return("Error de síntaxis, etiquetas no encontradas en [2,1]: ", 0);
    printf(">> "); lovdog_textok_cout((*archivero)->etiqueta_id, 0); printf("\n");
  }
  if(auxiliar_string)  free(auxiliar_string);

  // Las líneas siguientes son de registro 
  // y se listan de la forma
  // "archivo", "tipo", etiqueta_id1, etiqueta_id2, etiqueta_id3, ...
  archivo_desc* auxiliar_archivo=NULL;
  lovdog_texto* auxiliar_texto=NULL;
  unsigned char auxiliar_uchar=0;
  while(!lovdog_getlined(bd, &bd_fila, "\n")){
    lovdog_texto_token(&bd_fila, ",");
    lovdog_textok_trim(&bd_fila, ' ');
    auxiliar_archivo = archivo_init(1);
    // Extraer la dirección
    auxiliar_string=lovdog_textok_str(&bd_fila,0);
    archivo_mod(auxiliar_string, 'd', auxiliar_archivo);
    free(auxiliar_string);
    // Extraer el tipo
    auxiliar_string=lovdog_textok_str(&bd_fila,1);
    auxiliar_uchar=(unsigned char)atoi(auxiliar_string);
    archivo_mod(&auxiliar_uchar, 't', auxiliar_archivo);
    free(auxiliar_string);
    // Extraer las id de las etiquetas
    for (size_t n=2; n<bd_fila->token_cantidad; ++n){
      auxiliar_string=lovdog_textok_str(&bd_fila,n),
      lovdog_texto_strappend(&auxiliar_archivo->info->etiquetas,auxiliar_string);
      lovdog_texto_charappend(&auxiliar_archivo->info->etiquetas,',');
      free(auxiliar_string); auxiliar_string=NULL;
    }
    lovdog_texto_token(&auxiliar_archivo->info->etiquetas, ",");
    lovdog_textok_trim(&auxiliar_archivo->info->etiquetas, ' ');
    // Extraer el nombre
    auxiliar_texto =lovdog_textok_texto(&bd_fila, 0);
    lovdog_texto_token(&auxiliar_texto, "/");
    if(auxiliar_texto->token_cantidad)
      auxiliar_string=lovdog_textok_str(&auxiliar_texto, auxiliar_texto->token_cantidad-1);
    else auxiliar_string=lovdog_texto_str(&auxiliar_texto);
    lovdog_texto_strappend(&auxiliar_archivo->info->nombre, auxiliar_string);
    free(auxiliar_string); auxiliar_string=NULL;
    // Obtenemos la extensión
    lovdog_texto_token(&auxiliar_texto, ".");
    if(auxiliar_texto->token_cantidad>1)
      auxiliar_string=lovdog_textok_str(&auxiliar_texto, auxiliar_texto->token_cantidad-1);
    else {
      auxiliar_string=(char*)malloc(14*sizeof(char));
      strcpy(auxiliar_string,"sin extension");
    }
    auxiliar_archivo->info->tipo=auxiliar_string;
    auxiliar_string=NULL;
    lovdog_textofree(&auxiliar_texto);
    // Adjuntar el archivo
    tabla_archivo_add((*archivero)->tabla, auxiliar_archivo);
    lovdog_textofree(&bd_fila);
    bd_fila=NULL; auxiliar_archivo=NULL;
    auxiliar_texto=NULL; auxiliar_string=NULL;
  }

  fclose(bd);
  free(bd_direccion);
}

// Crea una entidad archivador
struct archivador* init_archivador(){
  struct archivador* archivador;
  archivador = (struct archivador*)malloc(sizeof(struct archivador));
  archivador->etiquetas = NULL;
  archivador->etiqueta_id=NULL;
  return archivador;
}

// Elimina archivador
void free_archivador(struct archivador **archivador){
  lovdog_textofree(&(*archivador)->etiquetas);
  lovdog_textofree(&(*archivador)->etiqueta_id);
  char err=tabla_free(&(*archivador)->tabla);
  if(err) printf("Error al liberar tabla: %d\n", err);
  free(*archivador);
  *archivador=NULL;
  return;
}

/* Verifica que'l archivo contenga las etiquetas dadas
 * el texto de etiquetas debe estar tokenizado
 * El último argumento es para saber si se trata de 
 * etiqueta o familia */
char archivo_etiquetas(struct archivador *archivero, lovdog_texto *etiquetas, char modo){
  if(!archivero) return -1;
  if(!etiquetas) return -1;
  if(modo != 1) return -1;
  if(modo != 2) return -1;
  return 0;
}

// Imprimir etiquetas
void imprime_etiquetas(struct archivador *archivero){
  if(!archivero) return;
  size_t cantidad=archivero->etiquetas->token_cantidad, i=0;
  char limit=1;
  printf("Etiquetas en base de datos [%zd]: {\n", cantidad-1);
  i=1;
  while(i<cantidad){
    TABULAR(2);
    lovdog_textok_cout(archivero->etiqueta_id,i);
    printf(":");
    lovdog_textok_cout(archivero->etiquetas,i);
    if(limit>1) limit=0&printf("\n");
    else printf(", ");
    ++i; ++limit;
  }
  printf("}\n");
  return;
}

// Etiqueta archivo
char etiqueta_archivo(struct archivador *archivero, lovdog_texto *etiqueta){
  if(!archivero) return -1;
  if(!etiqueta) return -2;
  if(!archivero->etiquetas) return -3;
  if(!archivero->etiqueta_id) return -4;
  if(!archivero->tabla) return -5;
  if(!archivero->tabla->actual) return -6;
  // Revisamos que la etiqueta exista en la base de datos
  if(!lovdog_textok_existe(etiqueta, archivero->etiquetas)) return -7;
  //Definimos elementos auxiliares
  lovdog_texto *_etiqueta_id=NULL;
  archivo_desc *_archivo=archivero->tabla->actual;
  // Obtenemos la ID de la etiqueta
  etiqueta_id(archivero,&_etiqueta_id,&etiqueta);
  if(!_etiqueta_id) return -6;
  // Adjuntamos la etiqueta al archivo
  lovdog_texto_textoappend(&_archivo->info->etiquetas, _etiqueta_id);
  lovdog_texto_charappend(&_archivo->info->etiquetas, ',');
  lovdog_textok_reset(&_archivo->info->etiquetas);
  lovdog_texto_token(&_archivo->info->etiquetas, ",");
  lovdog_textok_trim(&_archivo->info->etiquetas, ' ');
  lovdog_textofree(&_etiqueta_id);
  return 0;
}

// Etiqueta archivo
char etiquetaid_archivo(struct archivador *archivero, lovdog_texto *_etiqueta_id){
  if(!archivero) return -1;
  if(!_etiqueta_id) return -2;
  if(!archivero->etiquetas) return -3;
  if(!archivero->etiqueta_id) return -4;
  if(!archivero->tabla) return -5;
  if(!archivero->tabla->actual) return -6;
  // Revisamos que la etiqueta exista en la base de datos
  if(!lovdog_textok_existe(_etiqueta_id, archivero->etiqueta_id)) return -7;
  //Definimos elementos auxiliares
  lovdog_texto *etiqueta=NULL;
  archivo_desc *_archivo=archivero->tabla->actual;
  // Obtenemos la ID de la etiqueta
  etiqueta_id(archivero,&_etiqueta_id,&etiqueta);
  if(!etiqueta) return -6;
  // Adjuntamos la etiqueta al archivo
  lovdog_texto_textoappend(&_archivo->info->etiquetas, _etiqueta_id);
  lovdog_texto_charappend(&_archivo->info->etiquetas, ',');
  lovdog_textok_reset(&_archivo->info->etiquetas);
  lovdog_texto_token(&_archivo->info->etiquetas, ",");
  lovdog_textok_trim(&_archivo->info->etiquetas, ' ');
  lovdog_textofree(&etiqueta);
  return 0;
}

/* Esta función permite recibir una ID o una Etiqueta y asigna el 
 * valor correspondiente a su contraparte. La función recibe 3
 * argumentos: un archivador*, donde se realizará la búsqueda;
 * un lovdog_texto*, que corresponde a la ID;
 * un lovdog_texto*, que corresponde a la etiqueta; */
char etiqueta_id(
    struct archivador* archivero,
    lovdog_texto** id, lovdog_texto** etiqueta
){
  if(!id && !etiqueta) return -1;
  if(!archivero) return -2;
  if(!archivero->etiqueta_id) return -3;
  if(!archivero->etiquetas) return -4;
  size_t _token=0;
  if(!*id){
    _token=lovdog_textok_busca(*etiqueta,archivero->etiquetas);
    (*id)=lovdog_textok_texto(&archivero->etiqueta_id, _token);
  }
  if(!*etiqueta){
    _token=lovdog_textok_busca(*id,archivero->etiqueta_id);
    (*etiqueta)=lovdog_textok_texto(&archivero->etiquetas, _token);
  }
  return 0;
}

/* Después de saber dónde estoy, quiero saber la posición
 * relativa desde la base de datos */
lovdog_texto* ubicacion_relativa_desde_bd(){
  char
    *base_de_datos_path=NULL,
    *ud_esta_aqui=(char*)malloc(sizeof(char)*PATH_MAX);
  lovdog_texto
    *base_de_datos_lt=NULL,
    *ud_esta_aqui_lt=NULL,
    *salida=NULL;
  lovdog_char
    *lc=NULL,
    *lk=NULL;
  base_de_datos_path=buscar_archivo_recursivo_reversa(NOMBRE_BASE_DATOS);
  if(!base_de_datos_path) return NULL;
  getcwd(ud_esta_aqui,PATH_MAX);
  lovdog_texto_strappend(&base_de_datos_lt,base_de_datos_path);
  lovdog_texto_strappend(&ud_esta_aqui_lt,ud_esta_aqui);
  free(base_de_datos_path); free(ud_esta_aqui);
  // Preparamos la comparación
  lc=base_de_datos_lt->primer_letra;
  lk=ud_esta_aqui_lt->primer_letra;
  while(lc && lk){
    if(lc->c!=lk->c) break;
    lc=lc->sig; lk=lk->sig;
  } 
  if(!lk){
    lovdog_textofree(&ud_esta_aqui_lt);
    lovdog_textofree(&base_de_datos_lt);
    lovdog_texto_strappend(&salida,"");
    return salida;
  }
  if(!lc && (base_de_datos_lt->longitud==ud_esta_aqui_lt->longitud)){
    lovdog_textofree(&ud_esta_aqui_lt);
    lovdog_textofree(&base_de_datos_lt);
    lovdog_texto_strappend(&salida,NOMBRE_BASE_DATOS);
    return salida;
  }
  if(lk->c=='/') lk=lk->ant;
  while(lk->c!='/') lk=lk->ant;
  lk=lk->sig;
  while(lk){
    lovdog_texto_charappend(&salida, lk->c);
    lk=lk->sig;
  }
  lovdog_textofree(&ud_esta_aqui_lt);
  lovdog_textofree(&base_de_datos_lt);
  lovdog_texto_charappend(&salida, '/');
  return salida;
}

/* Agregar archivo a la base de datos */
char agrega_archivo(struct archivador* archivero, char* archivo){
  if(!archivero) return -1;
  if(!archivo) return -2;
  if(!archivero->tabla) return -3;
  lovdog_texto
    *dir_parente=NULL,
    *dir_relativ=NULL,
    *archivo_dlt=NULL;
  tabla_desc
    *lista_archivos=NULL;
  char
    *dir_parente_str=NULL,
    *archivo_relativ=NULL;
  long int
    err=0,
    err1=0;
  dir_parente=lt_archivo_padre(archivo);
  dir_relativ=ubicacion_relativa_desde_bd();

  dir_parente_str=lovdog_texto_str(&dir_parente);
  llena_tabla(dir_parente_str,&lista_archivos);
  free(dir_parente_str);
  if(!lista_archivos){
    printf("No se puede acceder al directorio padre: ");
    lovdog_texto_cout(dir_parente);
    printf("\n");
    lovdog_textofree(&dir_parente);
    lovdog_textofree(&dir_relativ);
    return -4;
  }
#ifdef __LOVDOG_DEBUG_LOG__
  tabla_cout(lista_archivos);
  printf("\n");
#endif

  lovdog_texto_textoappend(&archivo_dlt,dir_relativ);
  lovdog_texto_strappend(&archivo_dlt,archivo);
  lovdog_texto_rm_si(&archivo_dlt,archivo_dlt->longitud-1,'/');
  archivo_relativ=lovdog_texto_str(&archivo_dlt);
  err=archivo_busca(lista_archivos,archivo);
  err1=archivo_busca(archivero->tabla,archivo);
  if(err>-1 && err1<0){
    tabla_archivo_add(archivero->tabla,
        archivo_define(lista_archivos->actual->tipo,archivo_relativ));
    printf("Archivo agregado con exito\n");
  }else printf("No existe el archivo o ya está registrado\n");
  free(archivo_relativ);
#ifdef __LOVDOG_DEBUG_LOG__
  tabla_cout(lista_archivos);
#endif
  tabla_free(&lista_archivos);

  lovdog_textofree(&archivo_dlt);
  lovdog_textofree(&dir_parente);
  lovdog_textofree(&dir_relativ);
  return 0;
}

/* Agregar etiqueta a la base de datos */
char agrega_etiqueta(struct archivador* archivero, char* etiqueta){
  if(!archivero) return -1;
  if(!etiqueta) return -2;
  if(!archivero->tabla) return -3;
  if(!archivero->etiquetas) return -4;
  if(!archivero->etiquetas->tokens) return -4;
  if(!archivero->etiqueta_id) return -5;
  if(!archivero->etiqueta_id->tokens) return -5;
  lovdog_texto
    *etiqueta_dlt=NULL;
  char
    *auxiliar_char=NULL,
    etiqueta_idl[PATH_MAX];
  strcpy(etiqueta_idl,"");
  long int
    err1=0;
  size_t
    etiqueta_id=0,
    maximum=0;

  // Añadimos la etiqueta si aún no existe
  err1=lovdog_textok_busca(etiqueta_dlt,archivero->etiquetas);
  if(err1<0){
    // Obtenemos el id más alto
    for(int n=0; n<archivero->etiqueta_id->token_cantidad; ++n){
      auxiliar_char=lovdog_textok_str(&archivero->etiqueta_id,n);
      etiqueta_id=atol(auxiliar_char);
      if(etiqueta_id>maximum) maximum=etiqueta_id;
      free(auxiliar_char);
    } ++maximum; // Se asigna el valor siguiente
    sprintf(etiqueta_idl,"%zd",maximum);
    // Agregamos la etiqueta
    lovdog_textok_append(&archivero->etiquetas,etiqueta);
    // Agregamos la id
    lovdog_textok_append(&archivero->etiqueta_id,etiqueta_idl);
    printf("Etiqueta agregada con exito\n");
  }else printf("Etiqueta ya registrada\n");

  lovdog_textofree(&etiqueta_dlt);
  return 0;
}

/* Agregar archivo a la base de datos lovdog_texto */
char agrega_archivo_lt(struct archivador* archivero, lovdog_texto* archivo){
  char* archivo_str=lovdog_texto_str(&archivo);
  char salida=agrega_archivo(archivero,archivo_str);
  free(archivo_str);
  return salida;
}

/* Agregar etiqueta a la base de datos lovdog_texto */
char agrega_etiqueta_lt(struct archivador* archivero, lovdog_texto* etiqueta){
  char* archivo_str=lovdog_texto_str(&etiqueta);
  char salida=agrega_etiqueta(archivero,archivo_str);
  free(archivo_str);
  return salida;
}

/* Obtener directorio padre */
lovdog_texto* lt_archivo_padre(char* archivo){
  if(!archivo) return NULL;
  lovdog_texto* archivo_tl=NULL;
  lovdog_texto_strappend(&archivo_tl, archivo);
  // Rechazamos el directorio actual.
  if(archivo_tl->longitud<4){
    if (archivo[0]=='.' && archivo[1]=='.' && archivo[2]=='/')
      lovdog_textofree(&archivo_tl);
    if (archivo[0]=='.' && archivo[1]=='/')
      lovdog_textofree(&archivo_tl);
    if (archivo_tl->longitud<3)
      if(archivo[0]=='.' && archivo[1]=='.')
        lovdog_textofree(&archivo_tl);
    if (archivo_tl->longitud<2)
      if(archivo[0]=='.')
        lovdog_textofree(&archivo_tl);
    if(!archivo_tl) return NULL;
  }
  // Quitamos el ./ que puede preceder ala ubicación del
  // archivo
  if(archivo[0]=='.' && archivo[1]=='/'){
    lovdog_texto_rm_si(&archivo_tl,0,'.');
    lovdog_texto_rm_si(&archivo_tl,1,'/');
  }
  // Tokenizamos la dirección
  lovdog_texto_token(&archivo_tl, "/");
  // Si el directorio o archivo a añadir se encuentra en ./
  // Entonces, no habrá tokens
  char *directorio=NULL;
  if(archivo_tl->token_cantidad<2){
    directorio=(char*)malloc(sizeof(char)*2);
    strcpy(directorio,".");
  }else directorio=lovdog_texto_str_rango(
    &archivo_tl,0,
      archivo_tl->tokens[archivo_tl->token_cantidad-1]->ant->indice);
  lovdog_textofree(&archivo_tl);
  lovdog_texto_strappend(&archivo_tl, directorio);
  free(directorio);
  return archivo_tl;
}

/* Obtener directorio padre absoluto */
lovdog_texto* lt_archivo_abs_padre(char* archivo){
  if(!archivo) return NULL;
  lovdog_texto* archivo_tl=NULL;
  lovdog_texto_strappend(&archivo_tl, archivo);
  // Rechazamos el directorio actual.
  if(archivo_tl->longitud<4){
    if (archivo[0]=='.' && archivo[1]=='.' && archivo[2]=='/')
      lovdog_textofree(&archivo_tl);
    if (archivo[0]=='.' && archivo[1]=='/')
      lovdog_textofree(&archivo_tl);
    if (archivo_tl->longitud<3)
      if(archivo[0]=='.' && archivo[1]=='.')
        lovdog_textofree(&archivo_tl);
    if (archivo_tl->longitud<2)
      if(archivo[0]=='.')
        lovdog_textofree(&archivo_tl);
    if(!archivo_tl) return NULL;
  }
  // Tokenizamos la dirección
  lovdog_texto_token(&archivo_tl, "/");
  // Si el directorio o archivo a añadir se encuentra en ./
  // Entonces, no habrá tokens
  char *directorio=NULL;
  if(archivo_tl->token_cantidad<2){
    directorio=(char*)malloc(sizeof(char)*2);
    strcpy(directorio,".");
  }else directorio=lovdog_texto_str_rango(
    &archivo_tl,0,
      archivo_tl->tokens[archivo_tl->token_cantidad-1]->ant->indice);
  lovdog_textofree(&archivo_tl);
  lovdog_texto_strappend(&archivo_tl, directorio);
  free(directorio);
  return archivo_tl;
}

/* Obtener nombre de archivo */
lovdog_texto* lt_archivo_nombre_base(char* archivo){
  if(!archivo) return NULL;
  lovdog_texto* archivo_tl=NULL;
  lovdog_texto_strappend(&archivo_tl, archivo);
  // Rechazamos el directorio actual.
  if(archivo_tl->longitud<1) if (archivo[0]=='.') return NULL;
  // Quitamos el ./ que puede preceder ala ubicación del
  // archivo
  if(archivo[0]=='.' && archivo[1]=='/'){
    lovdog_texto_rm_si(&archivo_tl,0,'.');
    lovdog_texto_rm_si(&archivo_tl,1,'/');
  }
  // Tokenizamos la dirección
  lovdog_texto_token(&archivo_tl, "/");
  // Si el directorio o archivo a añadir se encuentra en ./
  // Entonces, no habrá tokens
  char *directorio=NULL;
  if(archivo_tl->token_cantidad<2){
    directorio=lovdog_texto_str(&archivo_tl);
  }else directorio=lovdog_textok_str(
    &archivo_tl,archivo_tl->token_cantidad-1);
  lovdog_textofree(&archivo_tl);
  lovdog_texto_strappend(&archivo_tl, directorio);
  free(directorio);
#ifdef __LOVDOG_DEBUG_LOG___
  printf(">> ");
  lovdog_texto_cout(archivo_tl);
#endif
  return archivo_tl;
}

/* Deduce la información del archivo */
archivo_info* archivo_info_define(char* archivo){
  if(!archivo) return NULL;
  archivo_info* nueva_info=init_archivo_info(); 
  lovdog_texto* base_nombre=lt_archivo_nombre_base(archivo);
  lovdog_texto_textoappend(&nueva_info->nombre,base_nombre);
  lovdog_texto_token(&base_nombre, ".");
  if(base_nombre->token_cantidad>1){
    nueva_info->tipo=
      lovdog_textok_str(&base_nombre,base_nombre->token_cantidad-1);
  }else{
    nueva_info->tipo=(char*)malloc(14*sizeof(char));
    if(!nueva_info->tipo) strcpy(nueva_info->tipo,"sin extension");
  }
  // Anexar el archivo a la base de datos
  lovdog_textofree(&base_nombre);
  return nueva_info;
}

/* Define archivo */
archivo_desc* archivo_define(unsigned char d_type, char* archivo_nme){
  archivo_desc* nuevo_archivo=crea_archivo();
  // Anexar el archivo a la base de datos
  archivo_mod(&d_type,'t',nuevo_archivo);
  archivo_mod(archivo_nme,'d', nuevo_archivo);
  // Anexar archivo info
  nuevo_archivo->info=archivo_info_define(archivo_nme);
  return nuevo_archivo;
}

/* Reescribe archivero */
char ReescribeBaseDeDatos(struct archivador* archivero){
  if(!archivero) return -1;
  if(!archivero->etiquetas) return -2;
  if(!archivero->etiqueta_id) return -3;
  // Definimos las primeras líneas con un lovdog_texto
  lovdog_texto
    *Linea_Archivo=NULL,
    *auxiliar_lt=NULL;
  tabla_desc
    *descriptor=archivero->tabla;
  archivo_desc
    *archivo=NULL;
  archivo_info
    *auxiliar_info=NULL;
  char
    auxiliar_char[PATH_MAX];
  strcpy(auxiliar_char,"");
  // Concatenamos las líneas de etiqueta
  for( size_t nt=0; nt<archivero->etiquetas->token_cantidad; ++nt){
    auxiliar_lt=lovdog_textok_texto(&archivero->etiquetas, nt); 
    lovdog_texto_textoappend(&Linea_Archivo,auxiliar_lt);
    lovdog_texto_strappend(&Linea_Archivo,",");
    lovdog_textofree(&auxiliar_lt);
    auxiliar_lt=NULL;
  }
  lovdog_texto_strappend(&Linea_Archivo,"\n");
  // Concatenamos las líneas de etiquetaID
  for( size_t nt=0; nt<archivero->etiqueta_id->token_cantidad; ++nt){
    auxiliar_lt=lovdog_textok_texto(&archivero->etiqueta_id, nt); 
    lovdog_texto_textoappend(&Linea_Archivo,auxiliar_lt);
    lovdog_texto_strappend(&Linea_Archivo,",");
    lovdog_textofree(&auxiliar_lt);
    auxiliar_lt=NULL;
  }
  // Si archivos en la tabla, se procede
  if(descriptor && descriptor->primer_archivo ){
    archivo=descriptor->primer_archivo;
    size_t asa=0;
    while(archivo){
      error_return("Guardando archivo %zd\n",asa++);
      auxiliar_info=archivo->info;
      if( archivo->tipo || archivo->direccion )
        lovdog_texto_strappend(&Linea_Archivo,"\n");
      if(archivo->direccion){
        lovdog_texto_strappend(&Linea_Archivo,archivo->direccion);
        lovdog_texto_strappend(&Linea_Archivo,",");
      }else lovdog_texto_strappend(&Linea_Archivo, "/N/A,");
      if(archivo->tipo){
        sprintf(auxiliar_char,"%d,",archivo->tipo);
        lovdog_texto_strappend(&Linea_Archivo,auxiliar_char);
        strcpy(auxiliar_char,"");
      }else lovdog_texto_strappend(&Linea_Archivo, "0,");
      // Imprime los elementos de archivo info
      if(auxiliar_info)
        if(auxiliar_info->etiquetas){
          for( size_t nt=0; nt<auxiliar_info->etiquetas->token_cantidad; ++nt){
            auxiliar_lt=lovdog_textok_texto(&auxiliar_info->etiquetas, nt); 
            lovdog_texto_textoappend(&Linea_Archivo,auxiliar_lt);
            lovdog_texto_strappend(&Linea_Archivo,",");
            lovdog_textofree(&auxiliar_lt);
            auxiliar_lt=NULL;
          }
        }
      archivo=archivo->sig;
    }
  }
  FILE* basedatos=NULL;
  basedatos=fopen(NOMBRE_BASE_DATOS, "w");
  if(!basedatos){ return error_return("No se pudo abrir la base de datos", -1); }
  lovdog_texto_fwrite(basedatos, &Linea_Archivo);
  fclose(basedatos);

  lovdog_texto_cout(Linea_Archivo);
  lovdog_textofree(&Linea_Archivo);
  return 0;
}

// init ARCHIVO INFO - EXTENSIÓN D'A ESTRUCTURA ARCHIVO_DEF

// Inicializar ARCHIVO INFO
archivo_info* init_archivo_info(){
  archivo_info* info=NULL;
  info = (archivo_info*)malloc(sizeof(archivo_info));
  info->tipo=NULL;
  info->nombre=NULL;
  info->etiquetas=NULL;
  return info;
}

// Imprimir ARCHIVO INFO
void cout_archivo_info(archivo_info* info){
  if(!info) return;
  printf("Nombre[%zd]: ", info->nombre->longitud);
  lovdog_texto_cout(info->nombre);
  printf("\n");
  printf("Tipo: %s\n", info->tipo);
  if(info->etiquetas){
    printf("Etiquetas[%zd]: ", info->etiquetas->token_cantidad);
    lovdog_textok_cout(info->etiquetas,-1);
  }else printf("Etiquetas[0]: N/A");
  printf("\n");
  return;
}

// Imprimir ARCHIVO INFO TABULADO
void cout_archivo_info_tab(archivo_info* info, size_t cout_tab){
  if(!info) return;
  TABULA;
  printf("Nombre[%zd]: ", info->nombre->longitud);
  lovdog_texto_cout(info->nombre);
  printf("\n");
  TABULA;
  printf("Tipo: %s\n", info->tipo);
  if(info->etiquetas){
    TABULA;
    printf("Etiquetas[%zd]: ", info->etiquetas->token_cantidad);
    lovdog_textok_cout(info->etiquetas,-2-cout_tab);
  }else{ TABULA; printf("Etiquetas[0]: N/A"); }
  printf("\n");
  return;
}

// Modificar ARCHIVO INFO
char mod_archivo_info(archivo_info* destino, archivo_info* nuevo){
  if(!destino) return 1;
// Editar tipo
  if(!nuevo->tipo)
    goto mod_archivo_info_editar_etiquetas;
  if(destino->tipo){
    if(!strcmp(destino->tipo,nuevo->tipo))
      goto mod_archivo_info_editar_etiquetas;
    free(destino->tipo);
    destino->tipo = NULL;
  }
  destino->tipo = (char*)malloc(strlen(nuevo->tipo)+1);
  strcpy(destino->tipo, nuevo->tipo);
mod_archivo_info_editar_etiquetas:
  if(nuevo->etiquetas) lovdog_texto_cpy(&destino->etiquetas, &nuevo->etiquetas,1);
  if(nuevo->nombre) lovdog_texto_cpy(&destino->nombre, &nuevo->nombre, 1);
  return 0;
}

// Liberar ARCHIVO INFO
char free_archivo_info(archivo_info* info){
  if(!info) return 1;
  if(info->tipo) free(info->tipo);
  if(info->etiquetas) lovdog_textofree(&info->etiquetas);
  if(info->nombre) lovdog_textofree(&info->nombre);
  free(info);
  return 0;
}
// iniv ARCHIVO INFO - EXTENSIÓN D'A ESTRUCTURA ARCHIVO_DEF
