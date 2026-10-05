#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED

#include "nodo.h"
#include "../constantes.h"

#define COLA_VACIA 1
typedef struct
{
    tNodo *pri,
          *ult;
}tCola;


void crearCola(tCola* pc);
int colaLlena(tCola* pc, unsigned cantBytes);
int ponerEnCola(tCola* pc, void* dato, unsigned tamDato);
int verPrimeroEnCola(tCola* pc, void* dato, unsigned tamDato);
int colaVacia(const tCola* pc);
int sacarDeCola(tCola* pc, void* dato, unsigned tamDato);
void vaciarCola(tCola* pc);


#endif // COLADINAMICA_H_INCLUDED
