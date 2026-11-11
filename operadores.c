#include "operadores.h"

int actualizarOperador(tEstadisticasOperadores* datosOperador, tArbol*abb)
{
    FILE* pf=fopen(OPERADORES_ARCH, "r+b");

    if(!pf)
    {
        if(!(pf=fopen(OPERADORES_ARCH, "w+b")))
            return ERROR_ARCH;
    }

    tOperadorIdx opIdx;

    opIdx.id= datosOperador->id;
    strcpy(opIdx.nombre, datosOperador->nomOperador);

    buscarElemArbol(abb, &opIdx, sizeof(tOperadorIdx), cmpOperadorIdx);

    fseek(pf, opIdx.pos, SEEK_SET);

    fwrite(datosOperador, sizeof(tEstadisticasOperadores),1,pf);

    fclose(pf);
    return TODO_OK;
}

/*
Flujo operadores.c:
gestionarOperador rcibe el nombre del operador, lo busca en el abb previamente cargado
con las claves compuestas (nombre - id), BUSCA SOLO NOMBRE para ver multiples
 coincidencias. El el caso de haberlas, se arma una lista temporal y las muestra
 por consola (solo nombre e id, la pos la guardamos pero el operador no la visualiza).
  El operador escoge su clave
 --una vez la escoge, recuperamos los datos de esa pos y y se la mandamos a jornada
 para que se actualicen los movimientos y puntuaciones pos-jornada
 -- Tanto en el caso de que el operador no se identifique con ninguna clave
 de la lista, o que directamente no se encuentren similares, llamamos a crearOperador
 donde se inicializan sus movimientos, puntuaciones y se cargan en los archivos necesarios.

 Posteriormente,  le pasamos esa estructura seteada tambien a jornada para que haga
 sus actualizaciones correspondientes pos-jornada
*/

int gestionarOperador(char* nomOperador, tArbol* abb, tEstadisticasOperadores* datosOperador)
{
    FILE* pfOperadores = fopen(OPERADORES_ARCH, "r+b");
    int estado;

    if (!pfOperadores)
    {
        pfOperadores = fopen(OPERADORES_ARCH, "w+b");
        if (!pfOperadores)
            return ERROR_ARCH;
    }

    estado=buscarOperador(nomOperador, abb, pfOperadores, datosOperador);

    if(estado==NO_EXISTE)
    {
        estado = crearOperador(nomOperador, abb, pfOperadores, datosOperador);
        if(estado==TODO_OK)
        {
            printf("OPERADOR %s CON ID %u CREADO CON EXITO\n",
                   datosOperador->nomOperador, datosOperador->id);

            system("pause");
        }
    }

    fclose(pfOperadores);
    return estado;
}

/*
flujo buscarOperador:
tenemos una struct "Repetidos" q contiene una lista que se cargara
con las posiciones de los repetidos + un contador (inicialmente seteadas)

1- cargamos en una struct idx el nombre ingresado para usar nuestra cmpPorNombre y buscarla
2- buscamos en el abb comparando solo por nombres (sin incluir las Id para teer multiples coincidencias)
a las que iba encontrando, las iba ostrando por consola y colocandola  en la lista de repetidos(encontrdos)
3-  si no encuentra ninguno (cant nunca incremento) retorna que no existen coincidencias
    si si, llama a seeccionarOp brindandole el poder de escoger o crear un nombre nuevo
4- vacia lista y retorna
*/
int buscarOperador(const char* nomOperador, tArbol* abb, FILE* pf, tEstadisticasOperadores* datosOperador)
{
    tRepetidos encontrados;
    tOperadorIdx claveABuscar;
    int estado;

    tListaCrear(&encontrados.lNombres);
    encontrados.cant = 0;

    strcpy(claveABuscar.nombre, nomOperador);

    buscarConFiltroEnArbol(abb, &claveABuscar, cmpOperadorNombre, mostrarYEnlistar, &encontrados);

    if(encontrados.cant>0)
        estado = seleccionaOperador(datosOperador, &encontrados, pf);
    else
        estado=NO_EXISTE;

    tListaVaciar(&encontrados.lNombres);
    return estado;
}

/*si no selecciona ninguno de la lista, retornamos noexiste para crearle un nuevo perfil¿
si selecciona uno de la lista, accedemos a ese dato, obtenemos su pos en el .idx, buscamos con la posicion
en el archivo .bin, y lo cargamos a la struct de datosOperador para pasarselo a jornada.c
*/
int seleccionaOperador(tEstadisticasOperadores* datosOperador, tRepetidos* encontrados, FILE* pf)
{
    int opcion, c;
    tOperadorIdx* operadorElegido;

    printf(" [%d] Ninguno de los anteriores (Nuevo operador)\n", encontrados->cant+1);

    do
    {
        printf("\nSeleccione operador: ");
        scanf("%d", &opcion);

        while((c = getchar()) != '\n' && c != EOF);

        if(opcion<1 || opcion>encontrados->cant+1)
            printf("Opcion invalida, reingrese..");
    }while(opcion<1 || opcion>encontrados->cant+1);

    if(opcion==encontrados->cant+1)
        return NO_EXISTE;

    operadorElegido= (tOperadorIdx*)tListaBuscarPorPos(&(encontrados->lNombres), opcion-1);

    if(!operadorElegido)
        return NO_EXISTE;

    if(fseek(pf, operadorElegido->pos, SEEK_SET) != 0)
        return ERROR_ARCH;

    if(fread(datosOperador, sizeof(tEstadisticasOperadores), 1, pf) != 1)
        return ERROR_ARCH;

    return TODO_OK;
}

/*
los ID's se generan de forma secuencial -> al crear uno nevo, le sumamos 1 al ultimo id ya generado

seteamos todos losdatos que se pasaran a jornada.c
seteamos todos losdatos que se cargaran en el .idx

avanzamos al final de archivo, obtenemos su pos para guadarlo en la struct idx
escribimos en el .bim
insertamos en el arbol de idx
escribimos en el .idx (abierto como append)
*/
int crearOperador(const char* nomOperador, tArbol* abb, FILE* pfOperadores, tEstadisticasOperadores* datosOperador)
{
    tOperadorIdx nueIdx;
    unsigned nuevoId = obtenerCantidadRegFILE(pfOperadores, sizeof(tEstadisticasOperadores)) + 1;
    int estado;

    datosOperador->id = nuevoId;
    strcpy(datosOperador->nomOperador, nomOperador);
    datosOperador->puntuacion = 0;

    strcpy(nueIdx.nombre, nomOperador);
    nueIdx.id = nuevoId;

    if (fseek(pfOperadores, 0, SEEK_END) != 0)
        return ERROR_ARCH;

    nueIdx.pos = ftell(pfOperadores);

    if(nueIdx.pos<0)
        return ERROR_ARCH;

    if(fwrite(datosOperador, sizeof(tEstadisticasOperadores), 1, pfOperadores)!=1)
        return ERROR_ARCH;

    if(fflush(pfOperadores)!=0)
        return ERROR_ARCH;

    estado= insertarArbolRec(abb, &nueIdx, sizeof(tOperadorIdx), cmpOperadorIdx);

    if(estado!=TODO_OK)
        return estado;

    FILE* pfIdx = fopen(IDX_OPERADORES_ARCH, "ab");

    if(!pfIdx)
        return ERROR_ARCH;

    if(fwrite(&nueIdx, sizeof(tOperadorIdx), 1, pfIdx) != 1)
    {
        fclose(pfIdx);
        return ERROR_ARCH;
    }

    fclose(pfIdx);
    return TODO_OK;
}

//obtiene la cantidad de operadores existentes-> base para generar el sig ID
long obtenerCantidadRegFILE(FILE* pf, size_t tam)
{
    long posOriginal,bytes;
    if (!pf)
        return -1;

    posOriginal = ftell(pf); //guarda pos actual
    if (posOriginal<0)
        return -1;

    if (fseek(pf, 0, SEEK_END)!=0)
        return -1;

    bytes = ftell(pf);
    fseek(pf, posOriginal, SEEK_SET); //restaura pos orig

    if(bytes<0 || bytes%tam!=0)
        return -1;

    return bytes/tam;
}

void mostrarYEnlistar(void* info, void* param)
{
    const tOperadorIdx* reg = (const tOperadorIdx*)info;
    tRepetidos* opEnc = (tRepetidos*)param;

    tListaInsertarAlFinal(&(opEnc->lNombres), reg, sizeof(tOperadorIdx));

    opEnc->cant++;

    printf(" [%d] Nombre: %s | ID: %u\n", opEnc->cant, reg->nombre, reg->id);
}

long cantidadRegistros(const char* nomArch, unsigned tam)
{
    FILE* pf = fopen(nomArch, "rb");
    long bytes;

    if(!pf)
        return ERROR_ARCH;

    if(fseek(pf, 0, SEEK_END) != 0)
    {
        fclose(pf);
        return ERROR_ARCH;
    }

    bytes= ftell(pf);

    fclose(pf);

    if(bytes<0 || bytes%tam!=0)
        return ERROR_ARCH;

    return bytes/tam;
}

/*
cuando el archivo.idx ya existe por ejecuciones anteriores, necesitamos reconstruir el indice
usando "rb" para no perder los datos y mantener la persistencia

a medida q se lee el .bin, lo vamos insertando los datos de .idx
 en el abb para mantener su persistencia en memoria


luego de eso, guardamos ese mismo abb en el .idx
*/
int reconstruirIndice(tArbol* pa)
{
    FILE*pf = fopen(OPERADORES_ARCH, "rb");

    if(!pf)
        return ERROR_ARCH;

    tEstadisticasOperadores reg;
    tOperadorIdx idx;
    long pos = ftell(pf); //tam del arch en bytes, nos indica la "pos" d los regs en cada lectura

    while(fread(&reg, sizeof(tEstadisticasOperadores), 1, pf))
    {
        strcpy(idx.nombre, reg.nomOperador);
        idx.id = reg.id;
        idx.pos = pos;

        insertarArbolRec(pa, &idx, sizeof(tOperadorIdx), cmpOperadorIdx);

        pos = ftell(pf);
    }

    fclose(pf);
    return guardarArbolEnArchivo(pa, IDX_OPERADORES_ARCH);
}

/**/
int inicializarIndiceOperadores(tArbol* pa)
{
    long cantDatos = cantidadRegistros(OPERADORES_ARCH, sizeof(tEstadisticasOperadores));
    long cantIdx = cantidadRegistros(IDX_OPERADORES_ARCH, sizeof(tOperadorIdx));

    if(cantDatos==ERROR_ARCH)//no se contó ningun dato pq no existia el arch¿¿
    {
        FILE* pf = fopen(OPERADORES_ARCH, "wb");

        if(!pf)
            return ERROR_ARCH;

        fclose(pf);
        cantDatos=0;
    }

    if(cantIdx!=cantDatos)
        return reconstruirIndice(pa);

    if(cantDatos==0)//el idx ya esta inicializado, guardamos nuesro abb
        return guardarArbolEnArchivo(pa, IDX_OPERADORES_ARCH);

    //sino (cantdatos>0), creamos nuestro abb a partir de ese idx
    return crearArbolDeArchivo(pa, IDX_OPERADORES_ARCH, sizeof(tOperadorIdx), cmpOperadorIdx);
}


//cmp's
int cmpOperadorIdx(const void* a, const void* b)
{
    const tOperadorIdx* op1 = (const tOperadorIdx*)a;
    const tOperadorIdx* op2 = (const tOperadorIdx*)b;

    int cmp = strcmpi(op1->nombre, op2->nombre);

    if (cmp != 0)
        return cmp;

    return (op1->id > op2->id) - (op1->id < op2->id);
}

int cmpOperadorNombre(const void* a, const void* b)
{
    const tOperadorIdx* op1 = a;
    const tOperadorIdx* op2 = b;

    return strcmpi(op1->nombre, op2->nombre);
}



/**RANKING*/
//accion para arbol
void accionGuardarIdx(void* infoNodo, unsigned tamNodo, unsigned nivel, void* params)
{
    tLista* pListaIdx = (tLista*)params;
    tListaInsertarAlFinal(pListaIdx, infoNodo, tamNodo);
}

int cmpPorPuntaje(const void* a, const void* b)
{
    const tEstadisticasOperadores* opA = (const tEstadisticasOperadores*)a;
    const tEstadisticasOperadores* opB = (const tEstadisticasOperadores*)b;

    //orden descente por puntaje, y sino, ascente por nombre y/o id
    int compPunt= opB->puntuacion - opA->puntuacion;

    if(compPunt!=0)
        return compPunt;

    int compNomb= strcmpi(opA->nomOperador, opB->nomOperador);

    if(compNomb!=0)
        return compNomb;

    return opA->id - opB->id;
}

int verRanking(tArbol* arbolIdx)
{
    int pos=1;
    tOperadorIdx idxTemp;
    tEstadisticasOperadores operadorTmp;


    tLista lIdx, listaRanking;
    tListaCrear(&lIdx);
    tListaCrear(&listaRanking);

    recorridoInorden(arbolIdx, 0, &lIdx, accionGuardarIdx);

    FILE* pfOperadores = fopen(OPERADORES_ARCH, "rb");
    if(!pfOperadores)
    {
        tListaVaciar(&lIdx);
        return ERROR_ARCH;
    }

    //proceso de lIdx-> sacar primero, leer archivo, e insertar ordenado en listaRanking
    while(!tListaVacia(&lIdx))
    {
        tListaSacarInicio(&lIdx, &idxTemp, sizeof(tOperadorIdx));

        fseek(pfOperadores, idxTemp.pos, SEEK_SET);
        fread(&operadorTmp, sizeof(tEstadisticasOperadores), 1, pfOperadores);

        tListaInsertarOrdSinDupli(&listaRanking, &operadorTmp, sizeof(tEstadisticasOperadores), cmpPorPuntaje);
    }

    fclose(pfOperadores);

    printf("\n----------- RANKING DE OPERADORES ------------\n");
    printf("\nPUESTO\tNOMBRE \t\tID \tPUNTUACION\n");

    while(!tListaVacia(&listaRanking))
    {
        tListaSacarInicio(&listaRanking, &operadorTmp, sizeof(tEstadisticasOperadores));

        printf(" [%d] \t%-13s | %-5u | %d\n", pos, operadorTmp.nomOperador, operadorTmp.id, operadorTmp.puntuacion);

        pos++;
    }
    printf("---------------------------------------------------\n");

    return TODO_OK;
}
