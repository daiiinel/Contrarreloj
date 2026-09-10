#include "pila_dinamica.h"


void crearPila(tPila*p)
{
    *p=NULL;
}
void vaciarPila(tPila*p)
{
    while(*p)
    {
        tNodo*aux=*p;

        *p=aux->sig;
        free(aux->info);
        free(aux);
    }
}

int apilar(tPila*p, const void *d, unsigned t)
{
    tNodo*nue = (tNodo*) malloc(sizeof(tNodo));

    if(nue==NULL || (nue->info=malloc(t)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, d, t);
    nue->tam= t;
    nue->sig= *p;

    *p=nue;

    return TODO_OK;
}

int desapilar(tPila*p, void *d, unsigned t)
{
    tNodo*aux=*p;

    if(aux==NULL)
        return PILA_VACIA;

    *p= aux->sig;

    memcpy(d, aux->info, MIN(t, aux->tam));
    free(aux->info);
    free(aux);

    return TODO_OK;
}

int pilaLlena(const tPila*p, unsigned t)
{
    tNodo *aux= (tNodo*) malloc(sizeof(tNodo));
    void*inf= malloc(t);

    free(aux);
    free(inf);

    return aux==NULL || inf==NULL;
}
int pilaVacia(const tPila*p)
{
    return *p==NULL;
}

int verTope(const tPila*p, void *d, unsigned t)
{
    if(*p==NULL)
        return PILA_VACIA;

    memcpy(d, (*p)->info , MIN(t, (*p)->tam));

    return TODO_OK;
}
