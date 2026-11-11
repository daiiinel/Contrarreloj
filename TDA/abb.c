#include "abb.h"


void crearArbol(tArbol*pa)
{
    *pa=NULL;
}

int recorridoPreorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*))
{
    if(*pa == NULL)
        return VACIO;

    accion((*pa)->info,(*pa)->tam, n, params);
    recorridoPreorden(&(*pa)->izq, n, params, accion);
    recorridoPreorden(&(*pa)->der, n, params, accion);

    return TODO_OK;
}

int recorridoInorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*))
{
    if(*pa == NULL)
        return VACIO;

    recorridoInorden(&(*pa)->izq, n, params, accion);
    accion((*pa)->info,(*pa)->tam, n, params);
    recorridoInorden(&(*pa)->der, n, params, accion);

    return TODO_OK;
}

int recorridoPosorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*))
{
    if(*pa == NULL)
        return VACIO;

    recorridoPosorden(&(*pa)->izq, n, params, accion);
    recorridoPosorden(&(*pa)->der, n, params, accion);
    accion((*pa)->info,(*pa)->tam, n, params);

    return TODO_OK;
}

int insertarArbolRec(tArbol*pa, const void* elem, unsigned tam, Cmp cmp)
{
    if(*pa==NULL)
    {
        tNodoA*nue=(tNodoA*)malloc(sizeof(tNodoA));

        if(!nue || (nue->info = malloc(tam))==NULL)
        {
            free(nue);
            return SIN_MEM;
        }

        memcpy(nue->info, elem, tam);

        nue->tam=tam;
        nue->der= nue->izq=NULL;
        *pa=nue;

        return TODO_OK;
    }
    int comp=cmp((*pa)->info, elem);

    if(comp==0)
        return DUPLICADO;
    if(comp>0)
        insertarArbolRec(&(*pa)->izq, elem, tam, cmp);
    else
        insertarArbolRec(&(*pa)->der, elem, tam, cmp);

    return TODO_OK;
}

int insertarArbolIte(tArbol*pa, const void* elem, unsigned tam, Cmp cmp)
{
    int comp;
    while(*pa)
    {
        comp= cmp((*pa)->info, elem);

        if(comp==0)
            return DUPLICADO;
        if(comp>0)
            pa= &(*pa)->izq;
        if(comp<0)
            pa= &(*pa)->der;
    }

    *pa=(tNodoA*)malloc(sizeof(tNodoA));
    if(!pa || ((*pa)->info = malloc(tam))==NULL)
    {
        free(pa);
        return SIN_MEM;
    }

    memcpy((*pa)->info, elem, tam);

    (*pa)->tam=tam;
    (*pa)->der= (*pa)->izq=NULL;

    return TODO_OK;
}

void vaciarArbol(tArbol*pa)
{
    if(*pa==NULL)
        return;

    vaciarArbol(&(*pa)->izq);
    vaciarArbol(&(*pa)->der);

    free((*pa)->info);
    free(*pa);
    *pa=NULL;
}
int arbolVacio(const tArbol *pa)
{
    return *pa == NULL;
}


int buscarElemArbol(const tArbol*pa, void *d, unsigned tam, Cmp cmp)
{
    int comp;

    while(*pa)
    {
        if((comp=cmp((*pa)->info, d))==0)
        {
            memcpy(d, (*pa)->info, tam);
            return TODO_OK;
        }
        else if(comp<0)
            pa= &(*pa)->der;
        else
            pa=&(*pa)->izq;
    }

    if(!*pa)
        return NO_EXISTE;

    return TODO_OK;
}

void* buscarEnArbol(const tArbol* pa, const void* clave, Cmp cmp)
{
    int comp;

    while(*pa)
    {
        if((comp=cmp((*pa)->info, clave))==0)
            return (*pa)->info;
        else if(comp<0)
            pa= &(*pa)->der;
        else
            pa=&(*pa)->izq;
    }

    return NULL;
}


int crearArbolDeArchivo(tArbol* pa, const char* nomArch, unsigned tam, Cmp cmp)
{
    FILE*pf=fopen(nomArch, "rb");
    int cantReg,res;

    if(!pf)
        return ERROR_ARCH;

    fseek(pf, 0, SEEK_END);
    cantReg = ftell(pf)/tam;

    if(cantReg==0)
    {
        fclose(pf);
        return VACIO;
    }

    res= crearArbolDeArchivoRec(pa, pf, 0, cantReg-1, tam, cmp);
    fclose(pf);
    return res;
}

int crearArbolDeArchivoRec(tArbol*pa, FILE* pf, int ini, int fin, unsigned tam, Cmp cmp)
{
    if (ini>fin)
        return TODO_OK;

    int medio, resIzq, resDer;
    void* dato = malloc(tam);
    if (!dato)
        return SIN_MEM;

    medio = (ini+fin)/2;
    fseek(pf, medio*tam, SEEK_SET);
    fread(dato, tam, 1, pf);

    insertarArbolRec(pa, dato, tam, cmp);
    free(dato);

    resIzq =crearArbolDeArchivoRec(pa, pf, ini, medio - 1, tam, cmp);
    if (resIzq != TODO_OK)
        return resIzq;

    resDer= crearArbolDeArchivoRec(pa, pf, medio + 1, fin, tam, cmp);
    if (resDer != TODO_OK)
        return resDer;

    return TODO_OK;
}

int buscarConFiltroEnArbol(const tArbol*pa, const void* clave, Cmp cmp, Accion accion, void*param)
{
    if(!*pa)
        return TODO_OK;

    buscarConFiltroEnArbol(&(*pa)->izq, clave, cmp,accion, param);

    if(cmp((*pa)->info, clave)==0)
        accion((*pa)->info, param);

    buscarConFiltroEnArbol(&(*pa)->der, clave, cmp,accion, param);

    return TODO_OK;
}

int guardarArbolEnArchivoRec(const tArbol* pa, FILE* pf)
{
    int estado;
    if (!*pa)
        return TODO_OK;

    if (fwrite((*pa)->info, (*pa)->tam, 1, pf) != 1)
        return ERROR_ARCH;

    estado = guardarArbolEnArchivoRec(&(*pa)->izq, pf);
    if (estado != TODO_OK)
        return estado;

    return guardarArbolEnArchivoRec(&(*pa)->der, pf);
}

int guardarArbolEnArchivo(const tArbol* pa, const char* nomArch)
{
    int estado;
    FILE* pf = fopen(nomArch, "wb");

    if(!pf)
        return ERROR_ARCH;

     estado= guardarArbolEnArchivoRec(pa, pf);

    fclose(pf);
    return estado;
}
