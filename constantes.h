#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//FUNCIONES MACRO
#define MIN(X,Y) ((X)<=(Y)? (X):(Y))

//salidas
#define TODO_OK             0
#define ERROR_COMANDO       1
#define ERROR_ARCH          2
#define NO_SIMULABLE        3

#define SIN_MEM             -1

//cadenas
#define MAX_NOMBRE_OP       50

#define TAM_LINEA           100

//nombres de archivos
#define PUERTO_ARCH     "puerto.txt"
#define CONF_ARCH       "config.txt"

//valores estaticos¿¿
#define CAMIONES_A_MOSTRAR  3 //en item "mostrar los 3 primeros de la cola de K"


#endif // CONSTANTES_H_INCLUDED
