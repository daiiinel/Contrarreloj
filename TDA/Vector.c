#include "vector.h"

void* buscarMenor(void* ini,void* fin,size_t tamElem,Cmp cmp);
void intercambiar(void* a,void* b,size_t tamElem);


bool tVectorCrear(tVector* vec,size_t tamElem)
{
    vec->ce = 0;
    vec->cap = 0;
    vec->tamElem = 0;

    vec->v = malloc(CAP_INI * tamElem);
    if(vec->v == NULL)
    {
        return false;
    }

    vec->cap = CAP_INI;
    vec->tamElem = tamElem;
    return true;
}

int tVectorOrdInsertar(tVector* vec,const void* elem,Cmp cmp)
{
    if(vec->ce == vec->cap)
    {
        if(!ampliarVector(vec))
        {
            return SIN_MEM;
        }
    }

    void *ult,*pVec;
    int i=0;
    pVec=vec->v;
    ult=(pVec+vec->ce)-1;
    while (i<vec->ce && cmp(elem,pVec) > 0)
    {
        pVec += vec->tamElem;
        i++;
    }
    if(pVec<=ult && cmp(elem,pVec) == 0)
        return DUPLICADO;

    return tVectorInsertarEnPos(vec,elem,(pVec-vec->v)/vec->tamElem);
}
int tVectorInsertarAlInicio(tVector* vec,const void* elem)
{
    if(vec->ce == vec->cap)
    {
        if(!ampliarVector(vec))
        {
            return SIN_MEM;
        }
    }
    void* *ult,*pVec;
    pVec=vec->v;
    ult=(pVec + (vec->ce-1) * vec->tamElem);
    for(void* j=ult;j >= pVec;j -= vec->tamElem)
    {
        memcpy(j+vec->tamElem,j,vec->tamElem);
        //*(ult+1-j)=*(ult-j);
    }
    //*pVec=elem;
    memcpy(pVec,elem,vec->tamElem);
    vec->ce++;
    return TODO_OK;
}
int tVectorInsertarAlFinal(tVector* vec,const void* elem)
{
    if(vec->ce == vec->cap)
    {
        if(!ampliarVector(vec))
        {
            return SIN_MEM;
        }
    }

    void *lugarIns,*pVec;
    pVec=vec->v;
    lugarIns=(pVec + vec->ce * vec->tamElem);

    memcpy(lugarIns,elem,vec->tamElem);
    vec->ce++;
    return TODO_OK;

}
int tVectorInsertarEnPos(tVector* vec,const void* elem,int pos)
{
    if(pos<0 || pos > vec->ce)
        return POS_INV;

    if(vec->ce == vec->cap)
    {
        if(!ampliarVector(vec))
        {
            return SIN_MEM;
        }
    }

    void *pVec,*ult,*dirIns;
    pVec = vec->v;
    ult = (pVec + (vec->ce - 1) * vec->tamElem);
    dirIns = pVec + pos * vec->tamElem;
    for(void* i=ult; i>=dirIns; i-= vec->tamElem)
    {
        memcpy(i + vec->tamElem,i,vec->tamElem);
    }
    memcpy(dirIns,elem,vec->tamElem);
    vec->ce++;
    return TODO_OK;
}
int tVectorOrdBuscar(const tVector* vec,void* elem,Cmp cmp)
{
    const void *li,*ls,*m;
    int pos=-1;
    bool encontrado=false;

    li=vec->v;
    ls=vec->v + (vec->ce-1)*vec->tamElem;

    while(ls>=li && !encontrado)
    {
         m=li+ (((ls-li)/vec->tamElem)/2) * vec->tamElem;

        if(cmp(elem,m) > 0)
        {
            li=m + vec->tamElem;
        }
        if(cmp(elem,m) < 0)
        {
            ls=m - vec->tamElem;
        }
        if(cmp(elem,m) == 0)
        {
            pos= (m - vec->v)/vec->tamElem;
            encontrado=true;
            memcpy(elem,m,vec->tamElem);
        }
    }
    return pos;
}
int tVectorDesordBuscar(const tVector* vec,void* elem,Cmp cmp)
{
    int pos=-1;
    const void* pVec=vec->v;
    const void* ult=(pVec + (vec->ce-1) * vec->tamElem);

    while(pos==-1 && pVec<=ult)
    {
        //if(elem==*pVec)
        if(cmp(elem,pVec) == 0)
        {
            pos = (pVec-vec->v)/vec->tamElem;
            memcpy(elem,pVec,vec->tamElem);
        }
        else
            pVec += vec->tamElem;

    }
    return pos;
}
bool tVectorOrdEleminar(tVector* vec,void* elem,Cmp cmp)
{
    int pos;
    pos=tVectorOrdBuscar(vec,elem,cmp);
    if(pos==-1)
    {
        printf("El elemento no existe.");
        return false;
    }
    tVectorEleminarPos(vec,pos);
    return true;

}
bool tVectorEleminarPos(tVector* vec,int pos)
{
    if(pos<0 || pos>=vec->ce)
    {
        printf("Posicion invalida");
        return false;
    }
    void* pVec,*ult,*posIni;

    pVec=vec->v;
    ult=(pVec + (vec->ce-1) * vec->tamElem);
    posIni=pVec + pos * vec->tamElem;
    for(void* i = posIni;i < ult;i += vec->tamElem)
    {
        memcpy(i,i+vec->tamElem,vec->tamElem);
    }
    vec->ce--;
    //Como size_t es un long long de eneteros es necesario que la division
    //sea entre float para admitir decimales
    if((float)vec->ce/vec->cap <= FACTOR_OCUP)
    {
        reducirVector(vec);
    }
    return true;
}

void tVectorMostrar(const tVector* vec,Print print)
{
    for(int i=0;i<vec->ce;i++)
    {
        print(vec->v + i*vec->tamElem);
    }
}

void tVectorVaciar(tVector* vec)
{
    vec->ce=0;
    vec->cap = CAP_INI;
    vec->v = realloc(vec->v,CAP_INI * sizeof(int));
}
void tVectorDestruir(tVector* vec)
{
    vec->ce = 0;
    vec->cap = 0;
    free(vec->v);
    vec->v = NULL;
}
int tVectorCE(tVector* vec)
{
    return vec->ce;
}

bool ampliarVector(tVector* vec)
{
    size_t nCap = vec->cap * FACTOR_INCR;
    void* nVec = realloc(vec->v, nCap * vec->tamElem);

    if(!nVec)
    {
        return false;
    }

    printf("Ampliacion de %lld a %lld\n",vec->cap,nCap);
    vec->v = nVec;
    vec->cap = nCap;
    return true;
}

void reducirVector(tVector* vec)
{
    size_t nCap = vec->cap * FACTOR_DECR;
    if(nCap < CAP_INI)
    {
        return;
    }
    vec->v = realloc(vec->v,nCap * vec->tamElem);
    printf("Reduccion de %lld a %lld\n",vec->cap,nCap);
    vec->cap = nCap;
}


