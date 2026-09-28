#include "colaDinamica.h"

void crearCola(tCola* pc)
{
    pc->pri=NULL;
    pc->ult=NULL;
}

int colaLlena(tCola* pc, unsigned cantBytes)
{
    tNodo* aux=(tNodo*)malloc(sizeof(tNodo));
    void* info=malloc(cantBytes);
    free(aux);
    free(aux->info);
    return((aux==NULL || info==NULL)?SIN_MEM:TODO_OK);
}

int ponerEnCola(tCola* pc, void* dato, unsigned tamDato)
{
    tNodo* aux=(tNodo*)malloc(sizeof(tNodo));
    if(aux==NULL || (aux->info=malloc(tamDato))==NULL)
    {
        free(aux);
        return SIN_MEM;
    }
    memcpy(aux->info,dato,tamDato);
    aux->sig=NULL;
    aux->tam=tamDato;

    if(pc->pri==NULL)
        pc->pri=aux;
    else
        pc->ult->sig=aux;

    pc->ult= aux;
    return TODO_OK;
}

int verPrimeroEnCola(tCola* pc, void* dato, unsigned tamDato)
{
    if(pc->pri==NULL)
        return COLA_VACIA;

    memcpy(dato,pc->pri->info, MIN(tamDato,pc->pri->tam));
    return TODO_OK;
}

int colaVacia(const tCola* pc)
{
    return (pc->pri==NULL?COLA_VACIA:-1);
}

int sacarDeCola(tCola* pc, void* dato, unsigned tamDato)
{
    tNodo *aux=pc->pri;
    if(pc->pri==NULL)
        return COLA_VACIA;

    memcpy(dato,aux->info,MIN(aux->tam,tamDato));
    free(aux->info);
    free(aux);
    if(pc->pri==NULL)
        pc->ult= NULL;

    return TODO_OK;
}

void vaciarCola(tCola* pc)
{
    while(pc->pri)
    {
        tNodo* aux=pc->pri;
        pc->pri=aux->sig;
        free(aux->info);
        free(aux);
    }
    pc->ult=NULL;
}
