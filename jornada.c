#include "jornada.h"
#include "constantes.h"
#include "TDA/colaDinamica.h"
#include "TDA/ListaDinamica.h"
#include "TDA/colaDinamica.h"

#include "archivos.h"

///para probar que se bajaron correctamente�

/**
void mostrarCamion(tCamiones*k)
{
    printf("ID: %s - T: %d - PED: %s\n", k->num, k->tiempoLlegada, k->pedido);
}
void mostrarBuque(tBuque*k)
{
    printf("ID: %s - T: %d -", k->nomBuque , k->tiempoLlegada);
}
void mostrarPilaContenedores(tPila* p)
{
    tPila aux;
    crearPila(&aux);
    char cad[5];

    printf("[ ");

    while(pilaVacia(p)!=PILA_VACIA)
    {
        desapilar(p, cad, sizeof(cad));
        printf("%s ", cad);
        apilar(&aux, cad, sizeof(cad));
    }
    printf("]\n");

    while(pilaVacia(&aux) != PILA_VACIA)
    {
        desapilar(&aux, cad, sizeof(cad));
        apilar(p, cad, sizeof(cad));
    }
}
*/

//propuesta/pseudocodigo
int nuevaJornada(tEstadisticasOperadores* datosOperador, tJornada* jor)
{
    //estructura que contiene datos de puerto.txt y config.txt
    tParametros param;

    tLista lMuelles;
    tCola cCamiones;
    tCola camionesEnTiempo;

    tMuelle auxMuelle;

    tZona auxZona;
    tLista lZonas;

    tCola cBuques;
    tCola buquesEnTiempo;

    unsigned i=1;

    if(validaConfigYGeneraPuerto(&param)!=TODO_OK)
        return NO_SIMULABLE;

    ///Muelles
    tListaCrear(&lMuelles);

    while(i< param.cantMuelle + 1)
    {
        sprintf(auxMuelle.nom,"M%d",i);
        auxMuelle.buque=NULL;
        auxMuelle.estado=0;

        if(tListaInsertarAlFinal(&lMuelles, &auxMuelle,TAM_MUELLE)!= TODO_OK)
            return NO_SIMULABLE;
        i++;
    }

    ///Camiones
    crearCola(&cCamiones);
    crearCola(&camionesEnTiempo);
    ///buques
    crearCola(&cBuques);
    crearCola(&buquesEnTiempo);

    ///Zonas
    tListaCrear(&lZonas);
    i=1;// lo vuelvo a setear
    while(i < param.cantZona)
    {
        sprintf(auxZona.nom, "Z%d", i);
        auxZona.capacidad=param.capPila;
        crearPila(&(auxZona.contenedores));
        if(tListaInsertarAlFinal(&lZonas,&auxZona,sizeof(tZona))!= TODO_OK)
             return NO_SIMULABLE;
        i++;
    }

    //Jornada
   // strcpy(jor->operador, nomOperador);
    jor->tiempo=0;
    jor->tiempo_maximo= param.minJornada;
    jor->puntaje=0;
    jor->lMuelles= lMuelles;
    jor->cCamiones= cCamiones;
    jor->buquesEnTiempo= buquesEnTiempo;
    jor->camionesEnTiempo= camionesEnTiempo;
    jor->lZonas= lZonas;
    jor->cBuques= cBuques;


    bajarPuertoTxt(jor);
    ///para probar que se bajaron correctamente� (auxiliar para probar cosas para nosotros, no para el tp�)

    //ver camiones
    /**
    tCamiones caux;
    while(colaVacia(&jor->cCamiones)!=COLA_VACIA)
    {
        sacarDeCola(&jor->cCamiones, &caux,TAM_CAMION);

        mostrarCamion(&caux);
        system("pause");
    }
    */

    //ver buques con sus pilas
    /**
    tBuque baux;
    while(colaVacia(&jor->cBuques)!=COLA_VACIA)
    {
        sacarDeCola(&jor->cBuques, &baux,TAM_BUQUE);

        mostrarBuque(&baux);
        mostrarPilaContenedores(&baux.contenedores);

        system("pause");
    }
    */

    // una cola auzliar donde pongo todos los que buques que van a llegar
    // cuando sea el tiempo de llegada
    // lo desencolo de la auxiliar y lo encolo en la cola POSTA de espera para un muelle libre


    //while t<puerto.durMax y no haya bloqueo y no haya terminado todos los procesos pendientes
        /*if pedirYEjecutarComando(parametros_necesarios) != TODO_OK
            return ERROR_J;
        actualizar tiempo dependiendo el tipo de operacion
        procesar lo q suceda en el nuevo tiempo
*/
    //al salir del while, actualizar estadisticas y guardarlas
    //actOperador(&operador); //funcion ajena, mandariamos a operadores.c

    printf("termine");
    system("pause");
    return TODO_OK;
}

//esperar dentro de comandos
//arrvis de camiones y buques por separado
//hago el printeo de la jornada finalizada por k razon;

int desarrolloJornada(tJornada* jor)
{
    int estado;
    // arrivo los que llegan en el momento 0;
    procesarArrivos(jor);
    printf("---------Inicio de una nueva jornada---------\n");

//&& hayBloque()==TODO_OK (dentro del while)
    while((estado=estadoDeJornada(jor))==SIGUE)
    {
        puts("toy en el while wiiii");

/*       if(validarComandoa(&cm, jornadam, &listaMoviento))todo ok;
        else
            hiceste mal pibe;
        else if ==bloque
            return porBLoque
*/
        // el proceso de los arrivos lo hago al final? despues de hacer la accion
        // dentro de la accion pregunto si hay arrivos en el medio
    }


   // paso al hstorial del operador, lo hace dai por el arbol
   //    archivoFinJornada(jor);
   //    historialDelOperadores();
    return TODO_OK;
}



// podria necesita el tiempo del movimiento asi puedo ver si esta en el medio
// si esta en medio avanzo el tiempo yo hasta el arrivo y lo hago y que avance hasta el tiempo de finalizar?
// si no esta en el medio que simplemente avance y hace el arrivo

int estadoDeJornada(tJornada *jor)
{
    if(jor->tiempo >= jor->tiempo_maximo)
        return SIN_TIEMPO;

    // si todas las colas estan vacias es porque
    // no hay mas buques en ningun lado  y  no hay mas camiones
    if(colaVacia(&(jor->buquesEnTiempo)) && colaVacia(&(jor->camionesEnTiempo)) && colaVacia(&(jor->cBuques)) && colaVacia(&(jor->cCamiones)))
        return FIN_JORNADA;

    return SIGUE;
}

int hayBloqueoOperativo(tJornada* jor)
{
    int capZonas;
    int band;
    tZona auxZona;
    tCamiones auxCamion;
    tContenedor auxConte;
    int pos;
    capZonas = capacidadDeZonas(&jor->lZonas);
    band = hayAtracosDeMuelles(&jor->lMuelles);
    if(band && capZonas != 0)
    {
        return TODO_OK;
    }
    if(colaVacia(&jor->camionesEnTiempo) && colaVacia(&jor->cCamiones))
    {
        return BLOQUE_OPERATIVO;
    }
    else
    {
        verPrimeroEnCola(&jor->camionesEnTiempo,&auxCamion,sizeof(tCamiones));
        strcpy(auxConte.id,auxCamion.pedido);
        if(buscarContenedorEnZonas(&jor->lZonas,&auxZona,&auxConte) < 0)
        {
            return BLOQUE_OPERATIVO;
        }
        else
        {
            pos = buscarEnPila(&auxZona.contenedores,&auxConte,sizeof(tContenedor),cmpContenedor);

        }
    }
}


int procesarArrivos(tJornada *jor)
{
    tCamiones auxCamion;
    tBuque auxBuque;

    // tengo que ver los primeros de las dos colas
    // si hay algo en:
        //Camiones: 1.Lo pongo en la cola de camiones a atender
        verPrimeroEnCola(&(jor->cCamiones), &auxCamion, TAM_CAMION);
        if(auxCamion.tiempoLlegada <= jor->tiempo)
            arrivoCamiones(jor);

        //Buques: 1. lo pongo en la cola de los buques a atendesr
        verPrimeroEnCola(&(jor->cCamiones), &auxBuque, TAM_BUQUE);
        if(auxBuque.tiempoLlegada<= jor->tiempo)
            arrivoBuques(jor);
                //2. veo el primero, si hay un muelle libre,
                    //2.1.1 lo adelanto
                    //2.1.2 actualizo el estado del muelle en la lista,
                    //2.2 si no, paso de largo
        acercarBuques(jor);
    return TODO_OK;
}

// saco de la jornada lo que tengo
int arrivoCamiones(tJornada *jor)
{
    tCamiones atender;
    if(sacarDeCola(&(jor->cCamiones), &atender, TAM_CAMION)==SIN_MEM)
        return SIN_MEM;
    ponerEnCola(&(jor->camionesEnTiempo), &atender, TAM_CAMION);
    return TODO_OK;
}

int arrivoBuques(tJornada *jor)
{
    tBuque atender;
    if(sacarDeCola(&(jor->cBuques), &atender, TAM_BUQUE)==SIN_MEM)
        return SIN_MEM;
    else
        ponerEnCola(&(jor->buquesEnTiempo), &atender, TAM_BUQUE);

    return TODO_OK;
}

int acercarBuques(tJornada* jor)
{
    tMuelle *auxMuelle=NULL;
    tBuque auxBuque;
    tBuque *nuevoBuque=(tBuque*)malloc(TAM_BUQUE);
    bool refe=0;
    while(!colaVacia(&(jor->buquesEnTiempo)))
    {
        if(tListaEncontrar(&(jor->lMuelles),&auxMuelle, &refe,TAM_MUELLE,cmpMuelleLibre)==1)
        {
            // en el muelle voy a tener el muelle libre
            sacarDeCola(&(jor->buquesEnTiempo),&auxBuque, TAM_BUQUE);
            *nuevoBuque=auxBuque;
            auxMuelle->buque=nuevoBuque;
            auxMuelle->estado=1;
            tListaEncontrarHacer(&(jor->lMuelles),auxMuelle,TAM_MUELLE,cmpMuelleNom,actualizarMuelleMartu);
        }else// no hay muelles libres de momento
            break;
    }

    return TODO_OK;
}


//funcion de martu, de ultima puede llamarse actualizarMuelleBqque¿ o nose
void actualizarMuelleMartu(void* viejo,void* nuevo)
{
    tMuelle *vie=(tMuelle*)viejo, *nue=(tMuelle*)nuevo;
    nue->buque=vie->buque;
    nue->estado=vie->estado;
}

int cmpMuelleNom(const void* reg1,const void* reg2)
{
    tMuelle *mue1= (tMuelle*)reg1,
            *mue2=(tMuelle*)reg2;
    return(strcmp(mue1->nom, mue2->nom));
}

int cmpMuelleLibre(const void* reg1,const void* estado)
{
    tMuelle *mue= (tMuelle*)reg1;
    bool valor=*(unsigned*)estado;
    return (mue->estado == valor)?1:0;
}

void verKPrimeros(tCola* camionesEspera)
{
    tCola colaAuxiliar;
    tCamiones camion;
    int cont=0;
    if(colaVacia(camionesEspera)==VACIO)
        return;
    crearCola(&colaAuxiliar);

    printf("\nCamiones en espera:\n");
    /// Descolo todo y solo muestro los k primeros
    while(!colaVacia(camionesEspera))
    {
        sacarDeCola(camionesEspera, &camion,TAM_CAMION);
        if(cont< CAMIONES_A_MOSTRAR)
        {
            printf("%d) %s solicita -> %s ", cont+1,camion.num, camion.pedido);
            cont++;
        }
        ponerEnCola(&colaAuxiliar, &camion, TAM_CAMION);
    }

    /// Vuelvo a armarla la cola principal
    while(!colaVacia(&colaAuxiliar))
    {
        sacarDeCola(&colaAuxiliar, &camion, TAM_CAMION);
        ponerEnCola(camionesEspera, &camion, TAM_CAMION);
    }
}
// a todas las pilas les aplico mostrar una zona
// se puede usar para mostrar siempre
void mostrarTodasLasPilasDeZonas(tLista* lisZonas)
{
    tListaRecorrer(lisZonas, NULL, mostrarUnaZona);
}
void mostrarUnaZona(void *datoZona, void* extra)
{
    tPila auxPila;
    tZona *zona= (tZona*)datoZona;
    char nomCont[MAX_NOMBRE_CONT];

    crearPila(&auxPila);
    printf("%s: [", zona->nom);

    while(!(pilaVacia(&(zona->contenedores))))
    {
        desapilar(&(zona->contenedores), &nomCont, sizeof(nomCont));
        printf("%s ", nomCont);
        apilar(&auxPila, nomCont, sizeof(nomCont));
    }
    //esta todo dado vuelta en la pila auxiliar

    printf("]");
    while(!(pilaVacia(&auxPila)))
    {
        desapilar(&auxPila, nomCont, sizeof(nomCont));
        apilar(&(zona->contenedores), nomCont, sizeof(nomCont));
    }
}

void mostrarColaBuqesEspera(tCola *colaBuquesEspera)
{
    tCola auxCola;
    tBuque auxBuque;

    crearCola(&auxCola);

    if(colaVacia(colaBuquesEspera))
    {    printf("\n No hay buques atracados en la cola esperando muelle!!\n");
        return;
    }
    printf("Los buques que que aun esperan un muelle son ->");
    while(!(colaVacia(colaBuquesEspera)))
    {
        sacarDeCola(colaBuquesEspera,&auxBuque,TAM_BUQUE);
        printf("%s ", auxBuque.nomBuque);
        ponerEnCola(&auxCola, &auxBuque, TAM_BUQUE);
    }

    while(!colaVacia(&auxCola))
    {
        sacarDeCola(&auxCola, &auxBuque, TAM_BUQUE);
        ponerEnCola(colaBuquesEspera, &auxBuque, TAM_BUQUE);
    }

}

void operacionVer(tJornada *jor)
{
    printf("Tiempo: %d / %d \t Puntuacion(de momento): %d", jor->tiempo, jor->tiempo_maximo, jor->puntaje);
    verKPrimeros(&(jor->camionesEnTiempo));
    mostrarColaBuqesEspera(&(jor->buquesEnTiempo));
    mostrarTodasLasPilasDeZonas(&(jor->lZonas));
}
// que es n bloque operativo
// si el espacio que queda en las zonas hace que sea imposible hacer los
// movimientos necesarios para obtener el contenedor que se pide


// pedro me tendria que traer la lista de movimientos
// Historial de jornada puede que no este terminado si hay que aregar algo
// funciona


int archivoFinJornada(tJornada *jor)
{
    FILE *pf=fopen(JORNADA_ARCH,"r+t");
    if(!pf)
    {
        pf=fopen(JORNADA_ARCH, "w+t");
        if(!pf)
            return ERROR_ARCH;
        //si entro aca es porque tuvo que crear el archivo entonces no habia nada antes
        fprintf(pf,"\t---------Historial de Jornadas---------\n");
        fprintf(pf, "%-15s %-10s %-20s %s\n","OPERADOR", "PUNTAJE","CANT. MOVIMIENTOS","TIEMPO FINAL/DISPONIBLE");
    }
    else
        fseek(pf,0,SEEK_END);

    fprintf(pf,"%-15s %-10u %-20d %d/%d\n",jor->operador, jor->puntaje,jor->puntaje,jor->tiempo, jor->tiempo_maximo);
    fclose(pf);

    return TODO_OK;
}

int bajarPuertoTxt(tJornada*jor)
{
    FILE* pPuerto= fopen(PUERTO_ARCH, "rt");
    char linea[TAM_LINEA];

    int tiempoJornada, cantMuelles, cantZonas, capPila; //innecesarias�

    tBuque buq;
    tCamiones cam;

    char cadCont[MAX_NOMBRE_CONT];

    char*pInicioCont; //puntero al inicio de contenedores en una cadena ->despues del ';'
    char* pCont;

    if(!pPuerto)
        return ERROR_ARCH;

    fscanf(pPuerto, "%*[^:]:%d\n%*[^:]:%d\n%*[^:]:%d\n%*[^:]:%d\n",
           &tiempoJornada, &cantMuelles, &cantZonas, &capPila);

    /* //--variante sin necesidad de asignar variables inusables�
    fgets(linea, TAM_LINEA, pPuerto); //[tiempo jornad
    fgets(linea, TAM_LINEA, pPuerto); //[cantmuelle
    fgets(linea, TAM_LINEA, pPuerto); //[zonas
    fgets(linea, TAM_LINEA, pPuerto); //[capila
    fgets(linea, TAM_LINEA, pPuerto); //\n*/

    fgets(linea, TAM_LINEA, pPuerto); //[BUQUES]


    fgets(linea, TAM_LINEA, pPuerto); //B001;T=4;C=C101,C=C102
    do
    {
        sscanf(linea, "%[^;];T=%d;", buq.nomBuque, &buq.tiempoLlegada);

        pInicioCont= strrchr(linea, ';');
        pCont= strrchr(linea, ',');

        crearPila(&buq.contenedores);

        while (pCont != NULL)
        {
            sscanf(pCont+1, "C=%s", cadCont);
            apilar(&buq.contenedores, cadCont, 5);

            *pCont = '\0';
            pCont = strrchr(linea, ',');
        }
        if(pInicioCont!=NULL)
        {
            sscanf(pInicioCont+1, "C=%s", cadCont);
            apilar(&buq.contenedores, cadCont, 5);
        }

        ponerEnCola(&(jor->cBuques), &buq, TAM_BUQUE);

        fgets(linea, TAM_LINEA, pPuerto); //B001;T=4;C=C101,C=C102

    }while(strcmpi(linea, "\n")!=0);


     fgets(linea, TAM_LINEA, pPuerto); //[CAMIONES]

    while(fgets(linea, TAM_LINEA, pPuerto))
    {  //K001;T=0;C=C101

        sscanf(linea, "%[^;];T=%d;C=%s\n", cam.num, &cam.tiempoLlegada, cam.pedido);

        cam.atendido= false;

        ponerEnCola(&(jor->cCamiones), &cam, TAM_CAMION);
    }

    fclose(pPuerto);
    return TODO_OK;
}



tBuque* tMuelleVerBuque(tMuelle* muelle)
{
    return muelle->buque;
}
tPila* tBuqueVerPila(tBuque* pBuque)
{
    return &(pBuque->contenedores);
}

tPila* tZonaVerPila(tZona* pZona)
{
    return &(pZona->contenedores);
}

void contarEspacioZona(void* info,void* param)
{
    tZona* zona = (tZona*)info;
    int* cont = (int*)param;
    int cap;
    cap = zona->capacidad - zona->cantContenedores;
    *cont += cap;
}

int capacidadDeZonas(tLista* zonas)
{
    int cont=0;
    tListaRecorrer(zonas,contarEspacioZona, &cont);
    return cont;
}

int hayAtracosDeMuelles(tLista* muelles)
{
    int band = 0;
    tListaRecorrer(muelles,muelleEnUso,&band);
    return band;
}

void muelleEnUso(void* info,void* param)
{
    tMuelle* muelle = (tMuelle*)info;
    int* band = (int*)param;

    if(muelle->estado)
    {
        *band = 1;
    }
}

int buscarContenedorEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor)
{
    int pos = -1, i=0;
    tPila* pPila;
    tZona* pZona;
    pZona = tListaBuscarPorPos(zonas,i);

    while(pZona != NULL && pos == -1)
    {
        pPila = tZonaVerPila(pZona);
        if(buscarEnPila(pPila,contenedor,sizeof(tContenedor),cmpContenedor) < 0)
        {
            i++;
            pZona = tListaBuscarPorPos(zonas,i);
        }
        else
        {
            pos = i;
            memcpy(zona,pZona,sizeof(tZona));
        }
    }
    return pos;
}

int buscarEnPila(tPila *pp,void *info, unsigned tam,Cmp cmp)
{
    tPila auxPila;
    int pos = -1,i=0;
    void *elem = malloc(tam);
    if (!elem)
        return pos;

    crearPila(&auxPila);

    /* vacía la pila original contando antes y después del buscado */
    while (!pilaVacia(pp) && pos == -1)
    {
        desapilar(pp, elem, tam);
        apilar(&auxPila, elem, tam);

        if(cmp(elem, info) == 0)
        {
            pos = i;
            memcpy(info,elem,tam);
        }
        else
        {
            i++;
        }
    }

    /* restaura la pila original */
    while (!pilaVacia(&auxPila))
    {
        desapilar(&auxPila, elem, tam);
        apilar(pp, elem, tam);
    }

    free(elem);
    return pos;
}

int cmpContenedor(const void* a,const void* b)
{
    const tContenedor* contenedor1 = (const tContenedor*)a;
    const tContenedor* contenedor2 = (const tContenedor*)b;

    return strcmp(contenedor1->id,contenedor2->id);
}
