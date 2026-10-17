#include "comandos.h"

//propuestaPEDROOOOOOOOOOOOOOOOOOOOOO
/*
int pedirYEjecutarComando( )
{

    //printf("OPERADOR> ");
    //pedir cadena
    //1ro: validar sintaxis
    //2do: validar si tiempoDisp-tiempoOper>0

    //switch con opciones
    //dependiendo la opcion, crear funciones auxiliares [por ejemplo ejecutarDES(&muelle, &zona), etc]

    return TODO_OK;
}
*/

int ejecutarComandoOperacion(tLista* zona,tVector* muelle,tCola* camion,tParametros* parametros,tJornada* jornada)
{
    tOperacion operacion;
    tMuelle muelle;
    tCamiones camion;
    tZona zona1,zona2;

    //El usuario ingresa un comando
    ingresarComandoOperacion(&operacion);

    switch (operacion.codOperacion)
    {
    case DES:
        strcpy(zona1.nom,operacion.argumento2);
        tListaSacarElemOrd(zona,&zona1,sizeof(tZona),cmpZona);
        break;
    case REU:
        /* code */
        break;
    case ENT:
        /* code */
        break;
    case ESP:
        /* code */
        break;
    case VER:
        /* code */
        break;
    
    default:
        break;
    }
}

int ingresarValidar ()
{
    return TODO_OK;
}

