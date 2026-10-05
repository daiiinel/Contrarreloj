#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "constantes.h"
#include "TDA/colaDinamica.h"

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


int validaConfigYGeneraPuerto(tParametros*param);
int generarPuertoTxT(tParametros*param);
void cargarParametros(tParametros* param, FILE*pConf);
int randAlterado(int maximo);


#endif // ARCHIVOS_H_INCLUDED
