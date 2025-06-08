#!/bin/bash

case "$1" in
  c-main)
    gcc -g -c -Wall main.c
    ;;
  c-dirlist)
    gcc -g -c -Wall -D__LOVDOG_ACTIVA_ARCHIVO_INFO__ dirlist.c
    ;;
  c-database)
    gcc -g -c -Wall database.c #`xml2-config --cflags --libs --cflags`
    ;;
  c-lchar)
    gcc -g -c -Wall lovdog_char.c
    ;;
  c-liotools)
    gcc -g -c -Wall lio_tools.c
    ;;
  o)
    gcc -g -o -Wall lang_ls main.o dirlist.o database.o lovdog_char.o lio_tools.o
    ;;
  circula)
    gcc -g -c -Wall main_circula.c
    gcc -g -c -Wall dirlist.c
    gcc -g -c -Wall database.c
    gcc -g -c -Wall lovdog_char.c
    gcc -g -c -Wall lio_tools.c
    gcc -g -o lc_c *.o -Wall
    rm *.o 
    chmod +x lc_c
    ./lc_c
    ;;
  busqueda)
    gcc -g -c -Wall main_busqueda.c
    gcc -g -c -Wall dirlist.c
    gcc -g -c -Wall database.c
    gcc -g -c -Wall lovdog_char.c
    gcc -g -c -Wall lio_tools.c
    gcc -g -o lc_b *.o -Wall
    rm *.o 
    chmod +x lc_b
    ./lc_b
    ;;
  token)
    gcc -g -c -Wall main_token.c
    gcc -g -c -Wall dirlist.c
    gcc -g -c -Wall database.c
    gcc -g -c -Wall lovdog_char.c
    gcc -g -c -Wall lio_tools.c
    gcc -g -o lc_t *.o -Wall
    rm *.o 
    chmod +x lc_t
    ./lc_t
    ;;
  debug)
    gcc -g -c -Wall main.c
    gcc -g -c -Wall dirlist.c
    gcc -g -c -Wall database.c
    gcc -g -c -Wall lovdog_char.c
    gcc -g -c -Wall lio_tools.c
    gcc -g -o lang_ls *.o -Wall
    rm *.o 
    chmod +x lang_ls
    gdb lang_ls
    ;;
  *)
    gcc -g -c -Wall main.c
    gcc -g -c -Wall -D__LOVDOG_ACTIVA_ARCHIVO_INFO__ dirlist.c
    gcc -g -c -Wall database.c
    gcc -g -c -Wall lovdog_char.c
    gcc -g -c -Wall lio_tools.c
    gcc -g -o lang_ls *.o -Wall
    rm *.o 
    chmod +x lang_ls
    ./lang_ls
    ;;
esac
