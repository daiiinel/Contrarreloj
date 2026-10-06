#ifndef LISTADINAMICA_H_INCLUDED
#define LISTADINAMICA_H_INCLUDED

#include "nodo.h"
#include "../constantes.h"

#define POS_INVALIDA        4
#define DUPLICADO           5
#define LISTA_VACIA         6
#define NO_EXISTE           7

typedef tNodo* tLista;


void tListaCrear(tLista* pList);
void tListaVaciar(tLista* pList);
void tListaRecorrer(tLista* pList,Accion accion,void* param);
void tListaMostrar(tLista* pList,Print print);

int tListaInsertarAlFinal(tLista* pList,const void* elem,unsigned tamElem);
int tListaInsertarEnPos(tLista *pList,const void* elem,unsigned tamElem,unsigned pos);
int tListaInsertarOrdSinDupli(tLista *pList,const void* elem,unsigned tamElem,Cmp cmp);
int tListaInsertarOrdConDupli(tLista *pList,const void* elem,unsigned tamElem,Cmp cmp,Actualizar actualizar);


int tListaSacarElem(tLista* pList,void* elem,unsigned tamElem,Cmp cmp);
int tListaSacarElemOrd(tLista* pList,void* elem,unsigned tamElem,Cmp cmp);
int tListaOrdEliminarDupli(tLista* pList,Cmp cmp,Actualizar actualizar);

void tListaIntercambiarDatoNodo(tLista* pList1,tLista* pList2);



#endif // LISTADINAMICA_H_INCLUDED
