#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

//FUNCIONES MACRO
#define MIN(X,Y) ((X)<=(Y)? (X):(Y))
#define A_MAYUSCULA(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 32) : (c))

//printsf
#define MENU_INICIO "\n\t --- Puerto de contenedores - Operacion Contrarreloj --- \n" \
                    "\t Ingrese la opcion que desea: \n" \
                    "\t 1- Iniciar una nueva jornada \n " \
                    "\t 2- Ver ranking de operadores \n" \
                    "\t 3- Salir \n" \
                    "\t Opcion--> "

#define MSG_NO_SIMULABLE "\n\t --- Simulacion fallida --- \n "\
                         "\t Verifique que el archivo config.txt \n"\
                         "\t tenga valores simulables y reintente\n"

#define MSG_ERROR_OPERADOR "\n\t --- Simulacion fallida --- \n "\
                            "\t Error al procesar Operador \n"\
                            "\t Reintente nuevamente\n"

#define MSG_FIN_JORNADA     "\n ------ FIN DE JORNADA ------\n"\
                            "\t Tiempo utilizado: %d / %d \t Puntaje obtenido: %d\n"\
                            "\t Camiones pendientes de atender: %d\n"\
                            "\t Contenedores atendidos: %d\n"\
                            "\t Buques descargados: %d\n"

//salidas

#define SIN_MEM             -2
#define TODO_OK             0
#define ERROR_COMANDO       1
#define ERROR_ARCH          2
#define NO_SIMULABLE        3
#define NO_EXISTE           -1
#define FIN_JORNADA         4
#define DUPLICADO           5
#define VACIO               6
#define BLOQUE_OPERATIVO    9
#define SIN_TIEMPO          10
#define SIGUE               11

//cadenas
#define MAX_NOMBRE_OP       50
#define MAX_NOMBRE_MUELLE   4
#define MAX_NOMBRE_ZONA     4
#define MAX_NOMBRE_CONT     5
#define MAX_NOMBRE_BUQUE    5
#define MAX_NOMBRE_CAMION   5
#define MAX_NOMBRE_CONT     5

//tamaños
#define TAM_LINEA           100
#define TAM_BUQUE sizeof(tBuque)
#define TAM_CAMION sizeof(tCamiones)
#define TAM_ZONA sizeof(tZona)
#define TAM_MUELLE sizeof(tMuelle)
#define TAM_CONTENEDOR sizeof(tContenedor)

//nombres de archivos
#define PUERTO_ARCH             "puerto.txt"
#define CONF_ARCH               "config.txt"
#define IDX_OPERADORES_ARCH     "operadores.idx"
#define OPERADORES_ARCH         "operadores.dat"
#define JORNADA_ARCH            "jornadas.txt"


//valores estaticos��
#define CAMIONES_A_MOSTRAR  3 //en item "mostrar los 3 primeros de la cola de K"

//Puntajes
#define PUNTAJE_DESEMBARCO  5
#define PUNTAJE_PENALIZADOR 2
#define PUNTAJE_ENTRGA      10

//Punteros a funcion
typedef int(*Cmp)(const void* a,const void* b);
typedef void(*Accion)(void* info,void* param);
typedef void(*Print)(const void* info);
typedef void(*Actualizar)(void* actualizado,const void* actualizador);


#endif // CONSTANTES_H_INCLUDED
