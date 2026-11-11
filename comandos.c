#include "comandos.h"

void copiarCadenaAMayus(char* destino,char* origen);
void aMayusCadena(char* cadena);

int cargarVectorComandos(tLista* pList,tParametros* param)
{
    int codRet=0,cantComandos,i=0;
    tComando listComandos[] =
    {
        {DES,CANT_ARG_OP,param->tiempoDescargaCont,procesarComandoDES},
        {REU,CANT_ARG_OP,param->tiempoReubicacion,procesarComandoREU},
        {ENT,CANT_ARG_COM,param->tiempoCargaCamion,procesarComandoENT},
        {ESP,CANT_ARG_COM,TIEMPO_ESP,procesarComandoESP},
        {VER,CANT_ARG_COM,0,procesarComandoVER}

    };
    cantComandos = sizeof(listComandos)/sizeof(tComando);
    tListaCrear(pList);
    while(codRet == 0 && i < cantComandos)
    {
        codRet = tListaInsertarOrdSinDupli(pList,listComandos+i,sizeof(tComando),cmpComando);
        i++;
    }
    if(codRet)
    {
        return codRet;
    }
    return TODO_OK;
}

int ejecutarComandoOperacion(tJornada* jornada,tLista* comandos,tLista* historial)
{
    char linea[TAM_LINEA];
    char* argumentos[TAM_ARG];
    tComando comando;

    if(leerComandoOperacion(linea,argumentos,&comando,comandos))
        return ERROR_INGRESO_COMANDO;

    procesarEnMedio(jornada, comando.tiempoUso);

    comando.procesarComando(argumentos,jornada,&comando,historial);

    return TODO_OK;
}

int leerComandoOperacion(char* linea,char** palabras,tComando* comando,tLista* comandos)
{
    char* act;
    int cant = 0;

    printf("OPERADOR> ");
    do
    {
        if(fgets(linea, TAM_LINEA+1, stdin) == NULL)
            return ERROR_INGRESO_COMANDO;

    }while(linea[0] == '\n');

    act = strchr(linea,'\n');
    if(act == NULL)
    {
        return ERROR_INGRESO_COMANDO;//linea larga
    }
    *act = '\0';

    //Didido la cadena en subcadenas con strtok y las guardo en "palabras"
    act = strtok(linea," ");
    while(act != NULL && cant <= TAM_ARG)
    {
        *(palabras + cant) = act;
        cant++;
        act = strtok(NULL," ");
    }

    copiarCadenaAMayus(comando->nomComando,*palabras);

    if(tListaBuscarElemOrd(comandos,comando,sizeof(tComando),cmpComando) < 0)
    {
        puts("No existe el comando ingresado");
        return ERROR_INGRESO_COMANDO;
    }

    //Valido cantidad de argumentos
    if(cant != comando->cantArg)
    {
        printf("Error en la cantidad de argumentos");
        return ERROR_INGRESO_COMANDO;
    }

    return TODO_OK;
}

int procesarComandoDES(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial)
{
    tMuelle muelle;
    tZona zona;
    tContenedor conte;
    if(validarTiempo(jornada,comando->tiempoUso))
    {
        puts("Tiempo insuficiente");
        return ERROR_COMANDO;
    }
    copiarCadenaAMayus(muelle.nom,argv[arg1]);
    copiarCadenaAMayus(zona.nom,argv[arg2]);

    if(validarDES(&muelle,&zona,jornada))
    {
        return ERROR_COMANDO;
    }

    //Si hay un evento en el medio antes de que termine la operacion
    //en este momento se actualiza
    procesarEnMedio(jornada, comando->tiempoUso);

    operacionDES(&muelle,&zona, &conte, jornada);
    guardarComando(comando->nomComando,argv[arg1],argv[arg2],conte.id,jornada->tiempo-comando->tiempoUso,jornada->tiempo,historial);

    //actualizo puntos
    zona.cantContenedores++;
    jornada->tiempo+=comando->tiempoUso;
    tListaInsertarOrdConDupli(&jornada->lZonas,&zona,sizeof(tZona),cmpZona,actualizarZona);

    return TODO_OK;
}

int validarDES(tMuelle* muelle,tZona* zona,tJornada* jornada)
{
    if(tListaBuscarElemOrd(&jornada->lMuelles,muelle,sizeof(tMuelle),cmpMuelle) < 0)
    {
        puts("Error argumento 1");
        return ERROR_COMANDO;
    }
    else if(tListaBuscarElemOrd(&jornada->lZonas,zona,sizeof(tZona),cmpZona) < 0)
    {
        puts("Error en argumento 2.");
        return ERROR_COMANDO;
    }
    //Valido que el muelle este ocupado
    else if(muelle->estado == false)
    {
        puts("Muelle sin carga pendiente");
        return ERROR_COMANDO;
    }
    //Valido que hay espacio
    else if(zona->capacidad == zona->cantContenedores)
    {
        puts("Zona sin espacio");
        return ERROR_COMANDO;
    }

    return TODO_OK;
}

int operacionDES(tMuelle* muelle,tZona* zona,  tContenedor* cont, tJornada* jor)
{
    tBuque* buqueAux;
    tPila* pilaConteBuque;
    tPila* pilaConteZona;
    buqueAux = tMuelleVerBuque(muelle);
    pilaConteBuque = tBuqueVerPila(buqueAux);
    pilaConteZona = tZonaVerPila(zona);

    desapilar(pilaConteBuque,cont,sizeof(tContenedor));
    if(pilaVacia(pilaConteBuque))
        liberarMuelle(jor, muelle);

    apilar(pilaConteZona,cont->id,sizeof(tContenedor));

    return TODO_OK;
}

int procesarComandoREU(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial)
{
    tZona zona1;
    tZona zona2;
    tContenedor conte;
    if(validarTiempo(jornada,comando->tiempoUso))
    {
        puts("Tiempo insuficiente");
        return ERROR_COMANDO;
    }
    copiarCadenaAMayus(zona1.nom,argv[arg1]);
    copiarCadenaAMayus(zona2.nom,argv[arg2]);

    if(validarREU(&zona1,&zona2,jornada))
        return ERROR_COMANDO;

    procesarEnMedio(jornada, comando->tiempoUso);

    operacionREU(&zona1,&zona2,&conte);
    guardarComando(comando->nomComando,argv[arg1],argv[arg2],conte.id,jornada->tiempo-comando->tiempoUso,jornada->tiempo,historial);


    zona1.cantContenedores--;
    zona2.cantContenedores++;
    jornada->tiempo+=comando->tiempoUso;
    tListaInsertarOrdConDupli(&jornada->lZonas,&zona1,sizeof(tZona),cmpZona,actualizarZona);
    tListaInsertarOrdConDupli(&jornada->lZonas,&zona2,sizeof(tZona),cmpMuelle,actualizarZona);
    return TODO_OK;
}

int validarREU(tZona* zona1,tZona* zona2,tJornada* jornada)
{
    if(tListaBuscarElemOrd(&jornada->lZonas,zona1,sizeof(tMuelle),cmpZona) < 0)
    {
        puts("Error argumento 1");
        return ERROR_COMANDO;
    }
    else if(tListaBuscarElemOrd(&jornada->lZonas,zona2,sizeof(tZona),cmpZona) < 0)
    {
        puts("Error en argumento 2.");
        return ERROR_COMANDO;
    }
    else if(strcmp(zona1->nom,zona2->nom) == 0)
    {
        printf("Mismo origen que destino");
        return ERROR_COMANDO;
    }
    else if(zona1->cantContenedores == 0)
    {
        puts("Zona origen vacio");
        return ERROR_ARCH;
    }
    else if(zona2->capacidad == zona2->cantContenedores)
    {
        puts("Zona destino sin espacio");
        return ERROR_ARCH;
    }

    return TODO_OK;
}

int operacionREU(tZona* pZona1,tZona* pZona2,tContenedor* conte)
{
    tPila* pilaZona1,*pilaZona2;
    pilaZona1 = tZonaVerPila(pZona1);
    pilaZona2 = tZonaVerPila(pZona2);

    desapilar(pilaZona1,conte,sizeof(tContenedor));
    apilar(pilaZona2,conte,sizeof(tContenedor));

    return TODO_OK;
}

int procesarComandoENT(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial)
{
    tZona zona;
    tCamiones camion;
    tContenedor conte;
    if(validarTiempo(jornada,comando->tiempoUso))
    {
        puts("Tiempo insuficiente");
        return ERROR_COMANDO;
    }

    if(validarENT(&jornada->lZonas,&jornada->camionesEnTiempo,&zona))
        return ERROR_ARCH;

    procesarEnMedio(jornada, comando->tiempoUso);
    operacionENT(&zona,&camion,&conte,&jornada->camionesEnTiempo);
    guardarComando(argv[nomArg],zona.nom,camion.num,conte.id,jornada->tiempo-comando->tiempoUso,jornada->tiempo,historial);
    zona.cantContenedores--;
    jornada->tiempo+=comando->tiempoUso;
    jornada->puntaje+=PUNTAJE_ENTRGA;
    jornada->contenedoresEntregados++;
    tListaInsertarOrdConDupli(&jornada->lZonas,&zona,sizeof(tZona),cmpZona,actualizarZona);

    return TODO_OK;
}

int validarENT(tLista* lZonas,tCola* cCamionesEsp,tZona* zona)
{
    tCamiones camion;
    tContenedor contenedor;
    if(verPrimeroEnCola(cCamionesEsp,&camion,sizeof(tCamiones)))
    {
        puts("No hay camiones en cola de espera");
        return ERROR_ARCH;
    }
    strcpy(contenedor.id,camion.pedido);
    if(buscarContenedorEnZonas(lZonas,zona,&contenedor) == NO_EXISTE)
    {
        puts("El contenedor no esta en el tope de alguna zona");
        return ERROR_ARCH;
    }

    return TODO_OK;
}

int operacionENT(tZona* pZona,tCamiones* camion,tContenedor* conte,tCola* pCamiones)
{
    tPila* pPila;
    pPila = tZonaVerPila(pZona);

    desapilar(pPila,conte,sizeof(tContenedor));
    sacarDeCola(pCamiones,camion,sizeof(tCamiones));
    return TODO_OK;
}

int procesarComandoVER(char* argv[],tJornada* pJornada,const struct tComando* comando,tLista* historial)
{
    printf("Tiempo: %d / %d \t Puntuacion(de momento): %d", pJornada->tiempo, pJornada->tiempo_maximo, pJornada->puntaje);
    verKPrimeros(&(pJornada->camionesEnTiempo));
    mostrarColaBuqesEspera(&(pJornada->buquesEnTiempo));
    mostrarTodasLasPilasDeZonas(&(pJornada->lZonas));
    return TODO_OK;
}
int procesarComandoESP(char* argv[],tJornada *pJornada,const struct tComando* comando,tLista* historial)
{
    pJornada->tiempo++;
    guardarComando(argv[nomArg],"","","",pJornada->tiempo-comando->tiempoUso,pJornada->tiempo,historial);

    avanceAutomatico(pJornada);
    return TODO_OK;
}


int cmpZona(const void* a,const void* b)
{
    const tZona* zona1 = (tZona*)a;
    const tZona* zona2 = (tZona*)b;

    return strcmp(zona1->nom,zona2->nom);
}

int cmpMuelle(const void* a,const void* b)
{
    const tMuelle* muelle1 = (tMuelle*)a;
    const tMuelle* muelle2 = (tMuelle*)b;
    return strcmp(muelle1->nom,muelle2->nom);
}

int cmpComando(const void* a,const void* b)
{
    const tComando* comando1 = (tComando*)a;
    const tComando* comando2 = (tComando*)b;

    return strcmp(comando1->nomComando,comando2->nomComando);
}

void actualizarZona(void* actualizado,const void* actualizador)
{
    tZona* zona1 = (tZona*)actualizado;
    const tZona* zona2 = (const tZona*)actualizador;

    zona1->cantContenedores = zona2->cantContenedores;
    zona1->contenedores = zona2->contenedores;
}

void actualizarMuelle(void* actualizado,const void* actualizador)
{
    tMuelle* muelle1 = (tMuelle*)actualizado;
    const tMuelle* muelle2 = (const tMuelle*)actualizador;

    muelle1->estado = muelle2->estado;
}

int validarTiempo(const tJornada* jornada,const int tiempoAgregado)
{
    int nueTiempo = tiempoAgregado + jornada->tiempo;
    return jornada->tiempo_maximo < nueTiempo;
}

void guardarComando(const char* nom,char*orig,char*dest,char*conte,int ini,int fin,tLista* historial)
{
    tHistoriaComando hist;
    strcpy(hist.codComando,nom);
    strcpy(hist.origen,orig);
    strcpy(hist.destino,dest);
    strcpy(hist.contenedor,conte);
    hist.tiempoInicio = ini;
    hist.tiempoFinal = fin;

    tListaInsertarAlFinal(historial,&hist,sizeof(tHistoriaComando));
}

int buscarContenedorEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor)
{
    int pos = -1, i=0;
    tPila* pPila;
    tZona* pZona;
    tContenedor auxContenedor;
    pZona = tListaBuscarPorPos(zonas,i);


    while(pZona != NULL && pos == -1)
    {
        pPila = tZonaVerPila(pZona);
        if(verTope(pPila,&auxContenedor,sizeof(tContenedor)))
        {
            i++;
            pZona = tListaBuscarPorPos(zonas,i);
        }
        else if(strcmp(contenedor->id,auxContenedor.id) == 0)
        {
            pos = i;
            memcpy(zona,pZona,sizeof(tZona));
        }
        else
        {
            i++;
            pZona = tListaBuscarPorPos(zonas,i);
        }
    }
    return pos;
}

void copiarCadenaAMayus(char* destino,char* origen)
{
    aMayusCadena(origen);
    strncpy(destino,origen,sizeof(TAM_ARGUMENTO));
    *(destino + TAM_ARG-1) = '\0';
}

void aMayusCadena(char* cadena)
{
    int i=0;
    while (*(cadena + i) != '\0')
    {
        *(cadena + i) = A_MAYUSCULA(*(cadena + i));
        i++;
    }
}
