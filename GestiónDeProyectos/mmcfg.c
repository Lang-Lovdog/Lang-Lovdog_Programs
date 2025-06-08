#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glob.h>
#include "../PasatiemposUtilidades/lovdog_dict.h"

void GetFileList(const char* archivo, dict* Dict);
void KeyValueFromLine(dict* Elemento, char* line, void(addLine)(dict*,char*,char*));
void ExtensionFromName(const char* name, char** extension);
void SelectFile(const char* extension,const dict* Dict);
void ParseArgumentos(char* argv[], int argc, char* Opciones, char** inFile);
void ProcesarOpciones(const char*,const dict* Dict,const char* file);
char* SearchConfFile();


int main(int argc, char* argv[]) {
  char
    Opciones=0, // Cada bit considera una bandera activa
                // Lista de bits
                // 1 -> Imprime diccionario (-pd)
                // 2 -> Imprime archivo de entrada (-pf)
                // 3 -> Imprime extensión de archivo de entrada (-pe)
   *inFile = NULL,
   *ConfFile = NULL
  ;
  if(argc<2 || !strcmp(argv[1],"-h")){ 
    printf("Usage: %s <file>\n"
        "Programa para gestión de archivos principales de proyectos\n\n"
        "\tDebe existir un archivo con el nombre main*.cfg"
        "\ten el\ndirectorio actual.\n\n"
        "\tEl documento de configuración deberá tener la"
        "\tsiguiente\nestructura:\n"
        "\t\t'main' : 'Archivo por defecto general'\n"
        "\t\t'extension' : 'Archivo por defecto para la extension'\n"
        "\t\t'extension' : '*'\n"
        "\tSi el valor del archivo es '*', entonces regresará el\n"
        "nombre del archivo indicado como argumento.\n",argv[0]);
    return 1;
  }
  dict Lista={NULL,0};
  /*---Procesamiento de argumentos---*/
  ParseArgumentos(argv,argc,&Opciones,&inFile);
  /*---Carga de datos---*/
  ConfFile = SearchConfFile();
  /*---Si no hay archivo de configuración, regresa el archivo de entrada---*/
  if(!ConfFile){
    printf("%s",inFile);
    free(inFile);
    return 0;
  }
  GetFileList((const char*)ConfFile,&Lista);
  free(ConfFile);
  /*---Procesamiento de banderas---*/
  ProcesarOpciones(&Opciones,&Lista,inFile);
  /*---Procesamiento de archivo de entrada---*/
  SelectFile(inFile,&Lista);
  /*---Liberar memoria---*/
  free(inFile);
  lovdog_dict_wo_keshimasu(&Lista);
  return 0;
}

void ParseArgumentos(char* argv[], int argc, char* Opciones, char** inFile) {
  int i=1;
  while(i<argc){
    if(!strcmp(argv[i],"-pd")) { *Opciones |= 0b001; ++i; continue; }
    if(!strcmp(argv[i],"-pf")) { *Opciones |= 0b010; ++i; continue; }
    if(!strcmp(argv[i],"-pe")) { *Opciones |= 0b100; ++i; continue; }
    (*inFile)=(char*)malloc(strlen(argv[i])*sizeof(char));
    strcpy(*inFile,argv[i]);
    ++i;
  }
}

void ProcesarOpciones(const char* o,const dict* Dict, const char* file) {
  if ((*o) & 0b001) lovdog_dict_print_rec(Dict);
  if ((*o) & 0b010) printf("%s\n",file);
  if ((*o) & 0b100){
    char* extension = NULL;
    ExtensionFromName(file,&extension);
    printf("%s\n",extension);
    free(extension);
  }
}

char* SearchConfFile(){
  // *.mmcfg files should be found in current dir
  // only the first one will be used
  glob_t glob_result;
  char* name = NULL;
  glob("[mM]ain*.cfg", 0, NULL, &glob_result);
  if(glob_result.gl_pathc==0){
    perror("No proyect config *.mmcfg file found");
    globfree(&glob_result);
    return NULL;
  }
  name = (char*)malloc(250*sizeof(char));
  strcpy(name,*glob_result.gl_pathv);
  globfree(&glob_result);
  return name;
}

void SelectFile(const char* file,const dict* Dict) {
  char* name = NULL;
  char* extension = NULL;
  ExtensionFromName(file,&extension);
  if(!extension) return;
  if((name=lovdog_dict_get(Dict,extension))){
    free(extension);
    if(!strcmp(name,"*")){ printf("%s\n",file); return; }
    printf("%s\n",name); return;
  }
  if((name=lovdog_dict_get(Dict,"main"))) printf("%s\n",name);
  return;
}

void ExtensionFromName(const char* name, char** extension) {
  char lLine[250];
  char* lExtension=NULL;
  strcpy(lLine,name);
  //printf("%s",lLine);
  if(!(lExtension=strrchr(lLine,'.')+1)) {
    (*extension)=NULL;
    char Error[550];
    sprintf(Error,"Error parsing name: %s\n"
        "ExtensionFromName: No extension found",name);
    perror(Error);
  }
  (*extension)=(char*)malloc(sizeof(char)*(strlen(lExtension)+1));
  strcpy(*extension,lExtension);
}


void KeyValueFromLine(dict* Elemento, char* line, void(addLine)(dict*,char*,char*)) {
  char 
    key[250], 
    value[250],
    lLine[250],
    contadorPattern=0
  ;
  strcpy(lLine,line);
  char* PatternList[8]={
    "%*[^'\"]%*['\"]%[^'\"]%*['\"]%*[^:]:%*[^'\"]%*['\"]%[^'\"]%*['\"]",
    "%*['\"]%[^'\"]%*['\"]%*[^:]:%*[^'\"]%*['\"]%[^'\"]%*['\"]",

    "%*['\"]%[^'\"]%*['\"]%*[^:]:%*['\"]%[^'\"]%*['\"]",
    "%*['\"]%[^'\"]%*['\"]:%*[^'\"]%*['\"]%[^'\"]%*['\"]",
    "%*['\"]%[^'\"]%*['\"]:%*['\"]%[^'\"]%*['\"]",

    "%*[^'\"]%*['\"]%[^'\"]%*['\"]%*[^:]:%*['\"]%[^'\"]%*['\"]",
    "%*[^'\"]%*['\"]%[^'\"]%*['\"]:%*[^'\"]%*['\"]%[^'\"]%*['\"]",
    "%*[^'\"]%*['\"]%[^'\"]%*['\"]:%*['\"]%[^'\"]%*['\"]"
  };
  while(contadorPattern<8){
    if(sscanf(lLine,*(PatternList+contadorPattern),key,value)==2) break;
    ++contadorPattern;
  }
  if(contadorPattern==8){
    char Error[550];
    sprintf(Error,"Error parsing line: %s\n"
        "KeyValueFromLine: Not paired key:value",line);
    perror(Error);
    return;
  };
  addLine(Elemento,key,value);
}

void GetFileList(const char* archivo, dict* Dict) {
  FILE* fp;
  char* line = NULL;
  size_t len = 250;
  char Anuncio[550];
  sprintf(Anuncio,"Archivo cfg: %s",archivo);
  perror(Anuncio);
  fp = fopen(archivo, "r");

  if (fp == NULL){ 
    perror("fopen: Could not open file");
    exit(1);
  }
  lovdog_dict_create_dict(Dict);
  while (getline(&line, &len, fp) != -1)
    KeyValueFromLine(Dict, line, lovdog_dict_add);
  free(line);
  fclose(fp);
}
