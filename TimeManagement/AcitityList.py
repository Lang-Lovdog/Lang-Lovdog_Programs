import os
import sys
import subprocess


definiciones={
    '__descanso__': '15',
}


def definicionesList(line):
    global definiciones
   # Separamos el elemento de la definición
   # el primer elemento delimitado por comillas
   # "elemento 1" o delimitado por guiones bajos
   # __elemento1__
   Valor=line.split()[1]
   if PrimerElemento in definiciones:
       
   else:
       definiciones[PrimerElemento]=line

def getListElement(line):
    #Quitamos los espacios del inicio y del final
    line = line.strip()
    # Si la linea empieza por *def, se envía a la
    # función de tabla de definiciones:
    if line.startswith('*def'):
        definicionesList(line)
    else:
        return None

def ReadFile(path):
    with open(path, 'r') as f:
        for line in f:
            print(line)
