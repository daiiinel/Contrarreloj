#include "ListaDinamica.h"


void tListaCrear(tLista* pList)
{
    *pList = NULL;
}

void tListaVaciar(tLista* pList)
{
    tNodo* elim = *pList;
    while(*pList != NULL)
    {
        pList = &elim->sig;
        free(elim->info);
        free(elim);
        elim = *pList;
    }
}

void tListaRecorrer(tLista* pList,Accion accion,void* param)
{
    while(*pList)
    {
        accion((*pList)->info,param);
        pList = &(*pList)->sig;
    }
}

void tListaMostrar(tLista* pList,Print print)
{
    while(*pList)
    {
        print((*pList)->info);
        pList = &(*pList)->sig;
    }
}

int tListaInsertarInicio(tLista* pList,const void* elem,unsigned tamElem)
{
    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(nueNodo == NULL)
    {
        return SIN_MEM;
    }
    nueNodo->info = malloc(tamElem);
    if(nueNodo->info == NULL)
    {
        free(nueNodo);
        return SIN_MEM;
    }

    memcpy(nueNodo->info,elem,tamElem);
    nueNodo->tam = tamElem;
    nueNodo->sig = *pList;
    *pList = nueNodo;

    return TODO_OK;
}

int tListaInsertarAlFinal(tLista* pList,const void* elem,unsigned tamElem)
{
    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(nueNodo == NULL)
    {
        return SIN_MEM;
    }
    nueNodo->info = malloc(tamElem);
    if(nueNodo->info == NULL)
    {
        free(nueNodo);
        return SIN_MEM;
    }

    memcpy(nueNodo->info,elem,tamElem);
    nueNodo->tam = tamElem;
    nueNodo->sig = NULL;

    while(*pList != NULL)
    {
        pList = &(*pList)->sig;
    }

    *pList = nueNodo;
    return TODO_OK;
}

int tListaInsertarEnPos(tLista *pList,const void* elem,unsigned tamElem,unsigned pos)
{
    while(*pList != NULL && pos)
    {
        pList = &(*pList)->sig;
        pos--;
    }
    if(!pos)
    {
        return POS_INVALIDA;
    }

    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(nueNodo == NULL)
    {
        return SIN_MEM;
    }
    nueNodo->info = malloc(tamElem);
    if(nueNodo->info == NULL)
    {
        free(nueNodo);
        return SIN_MEM;
    }

    memcpy(nueNodo->info,elem,tamElem);
    nueNodo->tam = tamElem;
    nueNodo->sig = *pList;

    *pList = nueNodo;
    return TODO_OK;
}

int tListaInsertarOrdSinDupli(tLista *pList,const void* elem,unsigned tamElem,Cmp cmp)
{

    tNodo* nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(nueNodo == NULL)
    {
        return SIN_MEM;
    }
    nueNodo->info = malloc(tamElem);
    if(nueNodo->info == NULL)
    {
        free(nueNodo);
        return SIN_MEM;
    }

    while(*pList != NULL && cmp(elem,(*pList)->info) > 0)
    {
        pList = &(*pList)->sig;
    }

    if(*pList != NULL && cmp(elem,(*pList)->info) == 0)
    {
        return DUPLICADO;
    }


    memcpy(nueNodo->info,elem,tamElem);
    nueNodo->tam = tamElem;
    nueNodo->sig = *pList;

    *pList = nueNodo;
    return TODO_OK;
}

int tListaInsertarOrdConDupli(tLista *pList,const void* elem,unsigned tamElem,Cmp cmp,Actualizar actualizar)
{
    tNodo* nueNodo;

    while(*pList != NULL && cmp(elem,(*pList)->info) > 0)
    {
        pList = &(*pList)->sig;
    }

    if(*pList != NULL && cmp(elem,(*pList)->info) == 0)
    {
        if(actualizar)
        {
            actualizar((*pList)->info,elem);
            return TODO_OK;
        }
    }

    nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(nueNodo == NULL)
    {
        return SIN_MEM;
    }
    nueNodo->info = malloc(tamElem);
    if(nueNodo->info == NULL)
    {
        free(nueNodo);
        return SIN_MEM;
    }

    memcpy(nueNodo->info,elem,tamElem);
    nueNodo->tam = tamElem;
    nueNodo->sig = *pList;

    *pList = nueNodo;
    return TODO_OK;
}


int tListaSacarInicio(tLista* pList,void* elem,unsigned tamElem)
{
    tNodo* elim = *pList;
    if(*pList == NULL)
    {
        return LISTA_VACIA;
    }

    memcpy(elem,(*pList)->info,MIN((*pList)->tam,tamElem));

    *pList = elim->sig;
    free(elim->info);
    free(elim);
    return TODO_OK;
}

int tListaSacarElem(tLista* pList,void* elem,unsigned tamElem,Cmp cmp)
{
    tNodo* elim;
    while(*pList != NULL && cmp(elem,(*pList)->info))
    {
        pList = &(*pList)->sig;
    }

    if(*pList == NULL)
    {
        return NO_EXISTE;
    }

    elim = *pList;
    memcpy(elem,(*pList)->info,MIN(tamElem,(*pList)->tam));

    *pList = elim->sig;
    free(elim->info);
    free(elim);
    return TODO_OK;
}

int tListaSacarElemOrd(tLista* pList,void* elem,unsigned tamElem,Cmp cmp)
{
    tNodo* elim;
    while(*pList != NULL && cmp(elem,(*pList)->info) > 0)
    {
        pList = &(*pList)->sig;
    }

    if(*pList == NULL || cmp(elem,(*pList)->info) != 0)
    {
        return NO_EXISTE;
    }

    elim = *pList;
    memcpy(elem,(*pList)->info,MIN(tamElem,(*pList)->tam));

    *pList = elim->sig;
    free(elim->info);
    free(elim);
    return TODO_OK;
}

int tListaOrdEliminarDupli(tLista* pList,Cmp cmp,Actualizar actualizar)
{
    tNodo* elim;
    tNodo* auxNodo;
    int comp;
    if(*pList == NULL)
    {
        return LISTA_VACIA;
    }

    auxNodo = *pList;
    pList = &(*pList)->sig;
    while(*pList != NULL)
    {
        comp = cmp(auxNodo->info,(*pList)->info);

        if(comp != 0)
        {
            auxNodo = *pList;
            pList = &(*pList)->sig;
        }
        if(comp == 0)
        {
            if(actualizar)
            {
                actualizar(auxNodo->info,(*pList)->info);
            }
            elim = *pList;
            *pList = elim->sig;
            free(elim->info);
            free(elim);
        }
    }

    return TODO_OK;
}

void tListaIntercambiarDatoNodo(tLista* pList1,tLista* pList2)
{
    unsigned auxTamDato;

    void* auxDato = (*pList1)->info;
    (*pList1)->info = (*pList2)->info;
    (*pList2)->info = auxDato;

    auxTamDato = (*pList1)->tam;
    (*pList1)->tam = (*pList2)->tam;
    (*pList2)->tam = auxTamDato;
}


