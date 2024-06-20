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

 Cuando se define una actividad a través de `*def`, la id
 reemplazará al nombre de la actividad al momento de definir
 un horario y deberá ser escrito sin comillas:
 ```c++
   *def "Actividad"  1
   1: "5m 30s"
 ```
 

## Definición de actividades en el inicio del archivo
 Para definir actividades que se repetirán, el sistema deberá
 leer un prefijo (símbolo reservado de inicio):
 `*def "Actividad a realizar" id (int)`

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
 `*notif comando argumentos`
 Si se desea pasar el nombre de la actividad al comando, se
 puede hacer con la palabra reservada `__actividad__` de la 
 siguiente manera:
 `*notif comando argumentos !__actividad__!`

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
 *def "Mi actividad" 1
 *def "Mi actividad 2" 2
 *def __descanso__ "15s"
 *def "__descanso__" 3
 //*def __notif__ tts "!__actividad__!"
 *def __init__ tts "comienza !__actividad__!"
 *def __init__ tts "finaliza !__actividad__! comienza descanso"

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
