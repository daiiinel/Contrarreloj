#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "constantes.h"

//propuesta

typedef struct
{
    int minJornada;
    int cantMuelle;
    int cantZona;
    int capPila;
    int maxBuques;
    int maxContPorBuques;
    int maxCamiones;
    int tiempoDescargaCont;
    int tiempoReubicacion;
    int tiempoCargaCamion;
}tParametros;


int generarPuerto(tParametros*param);
void cargarParametros(tParametros* param, FILE*pConf);


#endif // ARCHIVOS_H_INCLUDED
