################################################################
# Este programa está diseñado para leer una lista de actividades
# de la forma
# "ACtividad": "Tiempo (con sufijos s,m,h)"
################################################################
################################################################
# Ejemplo:
################################################################
# Dormir: 8h
# Comer: 3h 30m
# Trabajar: 1h 30m
# El descanso se fija de manera manual en el archivo o de manera
# interactiva en la línea de comandos.
# Para definir actividades que se repetirán, el sistema deberá
# leer un prefijo (símbolo reservado de inicio):
# *def "Actividad a realizar" id (int)
# *def __descanso__ "Xs Ym Zh"
# Nótese que __descanso__ es una palabra reservada y que se debe
# escribir sin comillas, mientras que el tiempo de descanso se
# indica entre comillas. Así mismo, todo elemento entre comillas
# se considerará una actividad y su id debe ser un número entero
# escrito sin comillas.
################################################################
# Para que el programa pueda notificar al usuario del término o
# del inicio de alguna actividad (o descanso), es posible
# utilizar un comando personalizado a través de la palabra
# reservada __notif__ (para ambos) __fin__ o __inicio__.
# __fin__ e __inicio__ tienen mayor prioridad que __notif__.
# El formato de notificación es:
# *notif comando argumentos
# Si se desea pasar el nombre de la actividad al comando, se
# puede hacer de esta manera:
# *notif comando argumentos !__actividad__!
################################################################
# Finalmente, para comentar un elemento de la lista de
# actividades, se realiza de la misma forma que en c:
# // Comentario
# /* Comentario
# en varias líneas*/
################################################################
