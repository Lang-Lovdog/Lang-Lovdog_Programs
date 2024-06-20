#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glob.h>
#include "lovdog_dict.h"

void GetFileList(const char* archivo, dict* Dict);
void KeyValueFromLine(dict* Elemento, char* line, void(addLine)(dict*,char*,char*));
void ExtensionFromName(char* name, char** extension);
void SelectFile(char* extension, dict* Dict);
char* SearchConfFile();



int main(int argc, char* argv[]) {
  if(argc!=2) {
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
  char* ConfFile = SearchConfFile();
  GetFileList((const char*)ConfFile,&Lista);
  free(ConfFile);
  SelectFile(argv[1],&Lista);
  lovdog_dict_wo_keshimasu(&Lista);
  return 0;
}


char* SearchConfFile(){
  // *.mmcfg files should be found in current dir
  // only the first one will be used
  glob_t glob_result;
  char* name = (char*)malloc(250*sizeof(char));
  glob("[mM]ain*.cfg", 0, NULL, &glob_result);
  if(glob_result.gl_pathc==0){
    perror("No proyect config *.mmcfg file found");
    globfree(&glob_result);
    exit(1);
  }
  strcpy(name,*glob_result.gl_pathv);
  globfree(&glob_result);
  return name;
}

void SelectFile(char* file, dict* Dict) {
  char* name = NULL;
  char* extension = NULL;
  ExtensionFromName(file,&extension);
  if(!strcmp(extension,"")) return;
  if((name=lovdog_dict_get(Dict,extension))){
    if(!strcmp(name,"*")){ printf("%s\n",file); return; }
    printf("%s\n",name);
  }
  if((name=lovdog_dict_get(Dict,"main"))) printf("%s\n",name);
  return;
}

void ExtensionFromName(char* name, char** extension) {
  char lLine[250], lExtension[250];
  strcpy(lLine,name);
  printf("%s",lLine);
  if(sscanf(lLine,"%*[^.].%s",lExtension)!=1) {
    strcpy(*extension,"");
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
