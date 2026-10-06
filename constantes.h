#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

//FUNCIONES MACRO
#define MIN(X,Y) ((X)<=(Y)? (X):(Y))

//printsf
#define MENU_INICIO "\n\t --- Puerto de contenedores - Operacion Contrarreloj --- \n" \
                    "\t Ingrese la opcion que desea: \n" \
                    "\t 1- Iniciar una nueva jornada \n " \
                    "\t 2- Ver ranking de operadores \n" \
                    "\t 3- Salir \n" \
                    "\t Opcion--> "

//salidas
#define TODO_OK             0
#define ERROR_COMANDO       1
#define ERROR_ARCH          2
#define NO_SIMULABLE        3
#define SIN_MEM             -1

//cadenas
#define MAX_NOMBRE_OP       50
#define MAX_NOMBRE_MUELLE   4

#define TAM_LINEA           100

//nombres de archivos
#define PUERTO_ARCH     "puerto.txt"
#define CONF_ARCH       "config.txt"

//valores estaticos��
#define CAMIONES_A_MOSTRAR  3 //en item "mostrar los 3 primeros de la cola de K"

//Punteros a funcion
typedef int(*Cmp)(const void* a,const void* b);
typedef void(*Accion)(void* info,void* param);
typedef void(*Print)(const void* info);
typedef int(*Actualizar)(void* actualizado,const void* actualizador);


#endif // CONSTANTES_H_INCLUDED
