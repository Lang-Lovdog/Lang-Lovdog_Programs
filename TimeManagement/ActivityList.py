import os
import sys
import subprocess
from time import sleep

########################################
reservados=[
    '__descanso__',
    '__init__',
    '__iniv__',
    '__notif__',
    '__shell__'
]
reservados_commands=[
    '__init__',
    '__iniv__',
    '__notif__'
]
definiciones={
    '__descanso__': '15s',
    '__shell__': True,
    '__init__': [],
    '__iniv__': [],
    '__notif__': []
}
listaActividades_array=[]
comentario_bool=False
subprocess_stdout_bool=True
verbose_commands=True
verbose_list=True
########################################

########################################
class Activity:
    Name = ''
    Time = 0
    
    def __init__ (self, name, time):
        self.Name = name
        self.str2time(time)

    def str2time(self, t):
        timeSuffixes = {
            'h' : 3600,
            'm' : 60,
            's' : 1
        }
        tiemp=0
        # from a Xs Ym Zh format to seconds
        tokenized = t.split(' ')
        for tok in tokenized:
            if tok[-1] in timeSuffixes:
                tiemp += \
                int(tok[:-1]) * timeSuffixes[tok[-1]]
        if tiemp<1:
            raise Exception("Formato de tiempo incorrecto")
            return
        self.Time=tiemp
########################################
def printList(ActivityList):
    for a in ActivityList:
        print("%s en %d segundos" % (a.Name, a.Time) )

def agregaActividad(line):
    global listaActividades_array
    global reservados
    global definiciones
    # Separamos la línea por el :
    try:
        lName=line.split(':',1)[0].strip()
        lTime=line.split(':',1)[1].strip()
    except Exception as e:
        print("Ignorando actividad %s\n\tformato incorrecto" % line)
        print(e)
        return

    if lName in reservados:
        print("Ignorando actividad %s\n\tpalabra reservada" % lName)
        return

    if lName.startswith('"') and lName.endswith('"'):
        lName=lName[1:-1]
    if lTime.startswith('"') and lTime.endswith('"'):
        lTime=lTime[1:-1]

    if lName in definiciones:
        listaActividades_array.append(Activity(definiciones[lName], lTime))
        return

    listaActividades_array.append(Activity(lName, lTime))
    

def definicionesList(line):
    global definiciones
    global verbose_list
    # Separamos el elemento de la definición
    # el primer elemento es la ID
    lId=line.split(' ',2)[1]
    lValor=line.split(' ',2)[2]
    if lId == '__shell__':
        if lValor == 'False':
            definiciones['__shell__']=False
        return
        definiciones['__shell__']=True
        return
    if lId in reservados_commands:
        if lValor.startswith('"') and lValor.endswith('"'):
            lValor=lValor[1:-1]
        definiciones[lId]=lValor.split(',')
        if verbose_list:
            print("Redefinición de %s como %s" % (lId, lValor))
        return
    if lId in definiciones:
        definiciones[lId]=lValor
        if verbose_list:
            print("Redefinición de %s como %s" % (lId, lValor))
        return
    definiciones[lId]=lValor
    if verbose_list:
        print("Definición de %s como %s" % (lId, lValor))

def getListElement(line):
    global comentario_bool
    #Quitamos los espacios del inicio y del final
    line = line.strip()
    # Si la linea empieza por *def, se envía a la
    # función de tabla de definiciones:
    if not line:
        return
    if line.startswith('//'):
        return
    if line.startswith('/*'):
        comentario_bool=True
        return
    if line.endswith('*/'):
        comentario_bool=False
        return
    if line.startswith('*def'):
        definicionesList(line)
        return
    agregaActividad(line)
    return

def ReadFile(path):
    global listaActividades_array
    with open(path, 'r') as f:
        for line in f:
            if comentario_bool:
                continue
            getListElement(line)
    if definiciones['__descanso__'].startswith('"') and definiciones['__descanso__'].endswith('"'):
        definiciones['__descanso__']=definiciones['__descanso__'][1:-1]
    listaActividades_array.append(Activity("Descanso", definiciones['__descanso__']))

def commandReplacement(commandList, actividad):
    salida=[]
    if actividad.startswith('"') and actividad.endswith('"'):
        actividad=actividad[1:-1]
    for command in commandList:
        salida.append(command.replace('!__actividad__!',actividad))
    return salida


def Notificacion(msg="",inicio=True):
    global definiciones
    global subprocess_stdout_bool
    global verbose_commands

    if inicio and len(definiciones['__init__']):
        comando=commandReplacement(definiciones['__init__'], msg)
        if verbose_commands:
            print(comando)
        if subprocess_stdout_bool:
            subprocess.run(comando,shell=definiciones['__shell__'])
            return
        subprocess.run(comando, stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,shell=definiciones['__shell__'])
        return

    if not inicio and len(definiciones['__iniv__']):
        comando=commandReplacement(definiciones['__iniv__'], msg)
        if verbose_commands:
            print(comando)
        if subprocess_stdout_bool:
            subprocess.run(comando,shell=definiciones['__shell__'])
            return
        subprocess.run(comando, stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,shell=definiciones['__shell__'])
        return

    if len(definiciones['__notif__']):
        comando=commandReplacement(definiciones['__notif__'], msg)
        print(comando)
        if subprocess_stdout_bool:
            subprocess.run(comando,shell=definiciones['__shell__'])
            return
        subprocess.run(comando, stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,shell=definiciones['__shell__'])
        return

    if inicio:
      print (msg,"inicio")
      return

    if not inicio:
      print (msg,"final")
      return
    
def ImprimeContador(segundos):
    for i in range(segundos,0,-1):
        print(i,end="\r")
        sleep(1)
        print(" "*len(str(i)),end="\r")

def iniciaLista(listaActividades_array):
    descanso=listaActividades_array[-1]
    for a in listaActividades_array:
        if a.Name=="Descanso":
            continue
        Notificacion(a.Name)
        sleep(a.Time)
        Notificacion(a.Name, False)
        if descanso.Time>0:
            Notificacion(descanso.Name)
            sleep(descanso.Time)
            Notificacion(descanso.Name, False)
    Notificacion("lista de actividades", False)

def iniciaLista_verbose(listaActividades_array):
    descanso=listaActividades_array[-1]
    for a in listaActividades_array:
        if a.Name=="Descanso":
            continue
        Notificacion(a.Name)
        ImprimeContador(a.Time)
        Notificacion(a.Name, False)
        if descanso.Time>0:
            Notificacion(descanso.Name)
            ImprimeContador(descanso.Time)
            Notificacion(descanso.Name, False)
    Notificacion("lista de actividades", False)

########################################

def imprimeAyuda():
    print("Temporizador de lista de actividades\n",
          "Uso: ", 
          os.path.basename(sys.argv[0]),
          "archivo [opciones]\n",
          "Opciones:\n",
          "  -d <segundos>  Duración de cada actividad\n",
          "  -no            No mostrar salida estándar del comando de notificación\n",
          "  -o             *Mostrar salida estándar del comando de notificación\n",
          "  -sh            *Usar shell subprocess\n",
          "  -nsh           No usar shell subprocess\n",
          "  -nvl           No mostrar preparación de lista\n",
          "  -vc            *Mostrar comandos en ejecución\n",
          "  -nvc           No mostrar comandos en ejecución\n",
          "  -vl            *Mostrar preparación de lista\n",
          "  -v             Mostrar lista de actividades cargada\n",
          "  -c             Mostrar contador de tiempo (segundos)\n",
          "  -h             Muestra esta ayuda\n",
          "  -version       Muestra la versión\n",
          "*: Valor por defecto")


########################################

if __name__ == '__main__':
  if len(sys.argv)<2:
      imprimeAyuda()
      exit(0)
  for arg in sys.argv:
      if arg=='-no':
          subprocess_stdout_bool=False
          continue
      if arg=='-o':
          subprocess_stdout_bool=True
          continue
      if arg=='-sh':
          definiciones['__shell__']=True
          continue
      if arg=='-nsh':
          definiciones['__shell__']=False
          continue
      if arg=='-nvl':
          verbose_list=False
          continue
      if arg=='-nvc':
          verbose_commands=False
          continue
      if arg=='-vl':
          verbose_list=True
          continue
      if arg=='-vc':
          verbose_commands=True
          continue
      if arg=='-h':
          imprimeAyuda()
          exit(0)
      if arg=='-version':
          print("Version 0.0.1")
          exit(0)

  ReadFile(sys.argv[1])
  for arg in sys.argv[2:]:
      if arg=='-d':
          listaActividades_array[-1].Time= \
             int(sys.argv[sys.argv.index(arg)+1])
          continue

  for arg in sys.argv[2:]:
      if sys.argv[2] == '-v':
        printList(listaActividades_array)
        iniciaLista(listaActividades_array)
        exit(0)
      if sys.argv[2] == '-c':
        iniciaLista_verbose(listaActividades_array)
        exit(0)
      if sys.argv[2] == '-vc' or sys.argv[2] == '-cv':
        printList(listaActividades_array)
        iniciaLista_verbose(listaActividades_array)
        exit(0)
  iniciaLista(listaActividades_array)
  exit(0)
