# TIME ACTIVITY LIST
Este programa está diseñado para leer una lista de actividades
de la forma
"Actividad": "Tiempo (con sufijos s,m,h)"

## Ejemplo:
```c++
 "Dormir": "8h"
 "Comer": "3h 30m"
 "Trabajar": "1h 30m"
```

# Macros y palabras reservadas

## Sobre las palabras reservadas
 Las palabras reservadas se deben escribir sin comillas y su
 forma es `__palabra__`.

 Si por algún motivo, el usuario
 define `"__palabra__"`, el sistema lo reconocerá como una
 actividad.
 

## Definición de actividades en el inicio del archivo
 Para definir actividades que se repetirán, el sistema deberá
 leer un prefijo (símbolo reservado de inicio):

 Cuando se define una actividad a través de `*def`, la id
 reemplazará al nombre de la actividad al momento de definir
 un horario y deberá ser escrito sin comillas:

 `*def id "Actividad a realizar"`

 ```c++
   *def 1 "Actividad" 
   1: "5m 30s"
 ```

## Descanso
 Por defecto el descanso es de 15 segundos.

 El descanso se puede indicar de manera manual en el archivo
 o de manera interactiva en la línea de comandos.
 `*def __descanso__ "Xs Ym Zh"`

## Notificaciones
 Para que el programa pueda notificar al usuario del término o
 del inicio de alguna actividad (o descanso), es posible
 utilizar un comando personalizado a través de la palabra
 reservada `__notif__` (para ambos) `__iniv__` (finalización)
 o `__init__` (inicio).
 `__iniv__` e `__init__` tienen mayor prioridad que `__notif__`.

 El formato de notificación es:
 `*notif comando, argumento, argumento`
 Si se desea pasar el nombre de la actividad al comando, se
 puede hacer con la palabra reservada `__actividad__` de la 
 siguiente manera:
 `*notif comando, argumentos, !__actividad__!`

## Lista de palabras reservadas
  ```c++
  __notif__: Comando de notificación (lista)
  __init__: Comando de inicio de actividad (lista)
  __iniv__: Comando de finalización de actividad (lista)
  !__actividad__!: Macro que indica Nombre de Actividad dentro de notif, init e iniv
  __descanso__: Tiempo de descanso (entero con sufijo: 1s,1m,1h)
  __shell__: En caso de que notif, init o iniv requieran ser ejecutados como shell (bool)
  ```
  *init e iniv tienen mayor prioridad que notif, sin embargo, toda configuración del
  archivo tiene menor prioridad que los argumentos de la línea de comandos.

# Comentarios
 Finalmente, para comentar un elemento de la lista de
 actividades, se realiza de la misma forma que en c:
```c
 // Comentario
 /* Comentario
 en varias líneas*/
```

# Ejemplo del archivo de actividades

```c++
 *def 1 "Mi actividad"
 *def 2 "Mi actividad 2"
 *def __descanso__ "15s"
 *def 3 "__descanso__"
 //*def __notif__ tts "!__actividad__!"
 *def __init__ tts, "comienza !__actividad__!"
 *def __init__ tts, "finaliza !__actividad__! comienza descanso"

 1: "5m 30s"
 "Actividad 3": "8m"
 2: "3m 12s"
 "Actividad 4": "20m"
 1: "15s"
 "Actividad 5": "20m"
 2: "3m 12s"
 /*3: "15s"
 1: "15s"*/
```
