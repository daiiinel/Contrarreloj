#include "jornada.h"
#include "constantes.h"
#include "TDA/colaDinamica.h"
#include "TDA/ListaDinamica.h"
#include "TDA/colaDinamica.h"
#include "comandos.h"
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

void printHistoria(const void* a);

//propuesta/pseudocodigo
int nuevaJornada(tEstadisticasOperadores* datosOperador, tJornada* jor, tParametros *param)
{
    //estructura que contiene datos de puerto.txt y config.txt

    tLista lMuelles;
    tCola cCamiones;
    tCola camionesEnTiempo;

    tMuelle auxMuelle;

    tZona auxZona;
    tLista lZonas;

    tCola cBuques;
    tCola buquesEnTiempo;

    unsigned i=1;

    if(validaConfigYGeneraPuerto(param)!=TODO_OK)
        return NO_SIMULABLE;

    ///Muelles
    tListaCrear(&lMuelles);

    while(i< param->cantMuelle + 1)
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
    while(i < param->cantZona)
    {
        sprintf(auxZona.nom, "Z%d", i);
        auxZona.capacidad=param->capPila;
        auxZona.cantContenedores=0;
        crearPila(&(auxZona.contenedores));
        if(tListaInsertarAlFinal(&lZonas,&auxZona,sizeof(tZona))!= TODO_OK)
             return NO_SIMULABLE;
        i++;
    }

    //Jornada
    strcpy(jor->operador, datosOperador->nomOperador);
    jor->tiempo=0;
    jor->tiempo_maximo= param->minJornada;
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

    return TODO_OK;
}

//esperar dentro de comandos
//arrvis de camiones y buques por separado
//hago el printeo de la jornada finalizada por k razon;

int desarrolloJornada(tJornada* jor, tParametros *param)
{
    int estado;
    tLista comandos;
    tLista historial;

    tListaCrear(&historial);
    cargarVectorComandos(&comandos, param);
    // arrivo los que llegan en el momento 0;

    procesarArrivos(jor);
    printf("---------Inicio de una nueva jornada---------\n");

//&& hayBloque()==TODO_OK (dentro del while)
    while((estado=estadoDeJornada(jor, comandos))==SIGUE)
    {
        printf("Tiempo: %d\n", jor->tiempo);
        verPrimeros(jor);
        ejecutarComandoOperacion(jor, &comandos,&historial);


        procesarArrivos(jor);
        //tListaMostrar(&historial,printHistoria);
        system("pause");
        system("cls");
    }

    printf(MSG_FIN_JORNADA, jor->tiempo, jor->tiempo_maximo, jor->puntaje);
    system("pause");
   // paso al hstorial del operador, lo hace dai por el arbol
   archivoFinJornada(jor);
   //    historialDelOperadores();
    return TODO_OK;
}
/// para ver el tope de la pila del buque en el muelle
void mostrarBuque(void* dato)
{
    tMuelle dat= *(tMuelle*)dato;
    tBuque aux= *(dat.buque);
    tContenedor auxP;
    verTope(&(aux.contenedores), &auxP, sizeof(tContenedor));
    printf("%s", aux.nomBuque);
    printf("%s", auxP.id);

    system("pause");
}
// quiero ver la carga del tope del buque

// podria necesita el tiempo del movimiento asi puedo ver si esta en el medio
// si esta en medio avanzo el tiempo yo hasta el arrivo y lo hago y que avance hasta el tiempo de finalizar?
// si no esta en el medio que simplemente avance y hace el arrivo

int estadoDeJornada(tJornada *jor, tLista coman)
{
    if(jor->tiempo >= jor->tiempo_maximo)
        return SIN_TIEMPO;

    // si todas las colas estan vacias es porque
    // no hay mas buques en ningun lado  y  no hay mas camiones
    if(colaVacia(&(jor->buquesEnTiempo)) && colaVacia(&(jor->camionesEnTiempo)) && colaVacia(&(jor->cBuques)) && colaVacia(&(jor->cCamiones)))
        return FIN_JORNADA;

    if(hayBloqueoOperativo(jor)==BLOQUE_OPERATIVO)
        return  BLOQUE_OPERATIVO;

    return SIGUE;
}

void avanceAutomatico(tJornada *jor)
{
    tBuque auxbuque;
    tCamiones auxCamion;
    if(colaVacia(&(jor->buquesEnTiempo)) && colaVacia(&(jor->camionesEnTiempo)))
    {
        if(verPrimeroEnCola(&(jor->cCamiones), &auxCamion, TAM_CAMION) != VACIO ||  verPrimeroEnCola(&(jor->cBuques), &auxCamion,TAM_BUQUE)!= VACIO)
        {
            if(auxbuque.tiempoLlegada < auxCamion.tiempoLlegada)
            {

                arrivoBuques(jor);
                jor->tiempo += (auxbuque.tiempoLlegada - jor->tiempo);
            }else
            {
                arrivoCamiones(jor);
                jor->tiempo += (auxCamion.tiempoLlegada - jor->tiempo);
            }
        }
    }
}

int hayBloqueoOperativo(tJornada* jor)
{
    int capZonas;
    /*tZona zona;
    tCamiones camion;
    tContenedor conte;*/
    capZonas = capacidadDeZonas(&jor->lZonas);

    /*verPrimeroEnCola(&jor->camionesEnTiempo,&camion,TAM_CAMION);
    strcpy(conte.id,camion.pedido);
    printf("capZonas: %d\n",capZonas);
    if(buscarContenedorEspEnZonas(&jor->lZonas,&zona,&conte) > NO_EXISTE)
    {
        mostrarUnaZona(&zona,NULL);
        return TODO_OK;
    }*/

    if(bloqueoPorDES(jor,capZonas) && bloqueoPorENT(jor,capZonas))
    {
        puts("Algo esta mal");
        return BLOQUE_OPERATIVO;
    }
    return TODO_OK;
}

int procesarArrivos(tJornada *jor)
{
    tCamiones auxCamion;
    tBuque auxBuque;

    verPrimeroEnCola(&(jor->cBuques),&auxBuque,TAM_BUQUE);
    verPrimeroEnCola(&(jor->cCamiones),&auxCamion,TAM_CAMION);

    if(jor->tiempo==0)
    {

        if(auxBuque.tiempoLlegada > auxCamion.tiempoLlegada)
        {
            arrivoBuques(jor);
            jor->tiempo+=auxBuque.tiempoLlegada;
            acercarBuques(jor);

        }else
        {
            arrivoCamiones(jor);
            jor->tiempo+=auxCamion.tiempoLlegada;
            arrivoBuques(jor);
            jor->tiempo+= (auxBuque.tiempoLlegada - jor->tiempo);
            acercarBuques(jor);
                        printf("entre");
                system("pause");
        }
    }
    else{
        arrivoCamiones(jor);
        arrivoBuques(jor);
        acercarBuques(jor);

    }
    return TODO_OK;
}

void liberarMuelle(tJornada* jor, tMuelle*mue)
{
    if(pilaVacia(&(mue->buque->contenedores)))
    {
        mue->estado=false;
        mue->buque=NULL;
        tListaEncontrarHacer(&(jor->lMuelles), mue,TAM_MUELLE, cmpMuelleNom, actualizarMuelleMartu);
        acercarBuques(jor);
    }
}
// saco de la jornada lo que tengo
int arrivoCamiones(tJornada *jor)
{
    tCamiones atender;
    verPrimeroEnCola(&(jor->cCamiones), &atender, TAM_CAMION);
    while(atender.tiempoLlegada <= (jor->tiempo+1) && !(colaVacia(&(jor->cCamiones))))
    {
        if(sacarDeCola(&(jor->cCamiones), &atender, TAM_CAMION)==TODO_OK)
            ponerEnCola(&(jor->camionesEnTiempo), &atender,TAM_CAMION);
        verPrimeroEnCola(&(jor->cCamiones), &atender, TAM_CAMION);
    }
    return TODO_OK;
}

int arrivoBuques(tJornada *jor)
{
    tBuque atender;
    verPrimeroEnCola(&(jor->cBuques), &atender, TAM_BUQUE);

    verPrimeroEnCola(&(jor->cBuques), &atender, TAM_BUQUE);
    while(atender.tiempoLlegada <= (jor->tiempo+1) && !(colaVacia(&(jor->cBuques))))
    {
        if(sacarDeCola(&(jor->cBuques), &atender, TAM_BUQUE)==TODO_OK)
            ponerEnCola(&(jor->buquesEnTiempo), &atender,TAM_BUQUE);
        verPrimeroEnCola(&(jor->cBuques), &atender, TAM_BUQUE);
    }
    acercarBuques(jor);
    return TODO_OK;
}

int acercarBuques(tJornada* jor)
{
    tMuelle auxMuelle;
    tBuque *auxBuque= (tBuque*)malloc(TAM_BUQUE);
    bool refe=0;

    while(!colaVacia(&(jor->buquesEnTiempo)))
    {
        if(tListaEncontrar(&(jor->lMuelles),&auxMuelle, &refe,TAM_MUELLE,cmpMuelleLibre)==1)
        {
            sacarDeCola(&(jor->buquesEnTiempo),auxBuque, TAM_BUQUE);
            // en el muelle voy a tener el muelle libre
            auxMuelle.buque = auxBuque;
            auxMuelle.estado=true;
            tListaEncontrarHacer(&(jor->lMuelles),&auxMuelle,TAM_MUELLE,cmpMuelleNom,actualizarMuelleMartu);
        }else// no hay muelles libres de momento
            break;
    }
    return TODO_OK;
}
//funcion de martu, de ultima puede llamarse actualizarMuelleBqque¿ o nose
void actualizarMuelleMartu(void* viejo,void* nuevo)
{
    tMuelle *vie=(tMuelle*)viejo, *nue=(tMuelle*)nuevo;
    vie->buque=nue->buque;
    vie->estado=nue->estado;
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
    bool valor=*(bool*)estado;
    return (mue->estado == valor)?1:0;
}

void verPrimeros(tJornada *jor)
{
    tCamiones camion;
    //veo los contenedores al tope de los
    mostrarTodosMulles(&(jor->lMuelles));
    // necesito ver lo que pide el primero de los camiones
    if(verPrimeroEnCola(&(jor->camionesEnTiempo), &camion, TAM_CAMION)==TODO_OK)
        printf("\nEl camion %s pide ->%s\n", camion.num, camion.pedido);

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
    if((colaVacia(camionesEspera))==1)
        printf("NO hay camoines por ahora");
    else
    {
        while((colaVacia(camionesEspera))==0)
        {
            sacarDeCola(camionesEspera, &camion,TAM_CAMION);
            if(cont< CAMIONES_A_MOSTRAR)
            {
                if(cont==0)
                    printf("\n%d) %s solicita -> %s ->Al frente", cont+1,camion.num, camion.pedido);
                else
                    printf("\n%d) %s solicita -> %s ", cont+1,camion.num, camion.pedido);

                cont++;
            }
            ponerEnCola(&colaAuxiliar, &camion, TAM_CAMION);
        }
        /// se vaa a uqedar aca para simepre
        while(!(colaVacia(&colaAuxiliar)))
        {
            sacarDeCola(&colaAuxiliar, &camion, TAM_CAMION);
            ponerEnCola(camionesEspera, &camion, TAM_CAMION);
        }
    }
}

void mostrarTodosMulles(tLista *lisMuelles)
{
    printf("-----El estado de los muelles-----");
    tListaRecorrer(lisMuelles, mostrarUnMuelle, NULL);
    puts("\n----------------------------------\n");
}

void mostrarUnMuelle(void* dato, void* extra)
{
    tMuelle *muelle= (tMuelle*)dato;
    tContenedor conte;

    if(muelle->buque !=NULL)
    {
        if(!pilaVacia(&(muelle->buque->contenedores)))
        {
            if(verTope(&(muelle->buque->contenedores), &conte, TAM_CONTENEDOR)==TODO_OK)
                printf("\nEl muelle %s tiene al buque %s con-> %s \n", muelle->nom, muelle->buque->nomBuque, conte.id);
        }
    }
}

void mostrarTodasLasPilasDeZonas(tLista* lisZonas)
{
    printf("\nEl contenido de las zonas:\n");

    tListaRecorrer(lisZonas, mostrarUnaZona,NULL);
    puts("\n");

}

void mostrarUnaZona(void *datoZona, void* extra)
{
    tPila auxPila;
    tZona *zona= (tZona*)datoZona;
    tContenedor nomCont;
    tPila* contenedores;
    crearPila(&auxPila);

    if(datoZona==NULL)
        printf("no hay nada");
    else
    {
        printf("%s: [", zona->nom);

        while(!(pilaVacia(&(zona->contenedores))))
        {
            desapilar(&(zona->contenedores), &nomCont, TAM_CONTENEDOR);
            printf("%s ", nomCont.id);
            apilar(&auxPila, &nomCont, TAM_CONTENEDOR);
        }
        //esta todo dado vuelta en la pila auxiliar

        printf("]");
        while(!(pilaVacia(&auxPila)))
        {
            desapilar(&auxPila, &nomCont,TAM_CONTENEDOR);
            apilar(&(zona->contenedores), &nomCont, TAM_CONTENEDOR);
        }
    }
}

void mostrarColaBuqesEspera(tCola *colaBuquesEspera)
{
    tCola auxCola;
    tBuque auxBuque;

    crearCola(&auxCola);
    printf("\nLos buques que que aun esperan un muelle son: ");

    if((colaVacia(colaBuquesEspera))==1)
        printf("\nNo hay buques atracados en la cola esperando muelle!!\n");
    else
    {
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
}

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

int buscarContenedorEspEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor)
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

int buscarEnPila(tPila *pp, void *info, unsigned tam, Cmp cmp)
{
    tPila auxPila;
    int pos = NO_EXISTE, i = 0;
    void *elem;

    if(!pp || !info || !cmp || tam == 0)
        return NO_EXISTE;

    elem = malloc(tam);
    if(!elem)
    {
        return SIN_MEM;
    }
    crearPila(&auxPila);

    while(!pilaVacia(pp) && pos == NO_EXISTE)
    {
        desapilar(pp, elem, tam);

        if(apilar(&auxPila, elem, tam))
        {
            apilar(pp, elem, tam);
            pos = SIN_MEM;
        }
        else if(cmp(elem, info) == 0)
        {
            pos = i;
            memcpy(info, elem, tam);
        }
        else
            i++;
    }
    ///restaurar la pila original
    while(!pilaVacia(&auxPila))
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

    return strncmp(contenedor1->id,contenedor2->id,sizeof(contenedor1->id));
}

int bloqueoPorENT(tJornada* jor,int capZonas)
{
    tContenedor conte;
    tCamiones camion;
    tZona zona;
    int encima;
    if(colaVacia(&jor->cCamiones) && colaVacia(&jor->camionesEnTiempo))
    {
        puts("---!No hay camiones esperando y no hay futuras llegadas---");
        return BLOQUE_OPERATIVO;
    }
    if(!colaVacia(&jor->camionesEnTiempo))
    {
        verPrimeroEnCola(&jor->camionesEnTiempo,&camion,TAM_CAMION);
        strcpy(conte.id,camion.pedido);
        if(buscarContenedorEspEnZonas(&jor->lZonas,&zona,&conte) < 0)
        {
            puts("---!No existe el contenedor en las zonas---");
            return BLOQUE_OPERATIVO;
        }
        else
        {
            encima = buscarEnPila(&zona.contenedores,&conte,TAM_CONTENEDOR,cmpContenedor);
            tListaInsertarOrdConDupli(&jor->lZonas,&zona,sizeof(tZona),cmpZona,actualizarZona);
            if(capZonas < encima)
            {
                puts("---!No hay espacio suficiente para a reubicar hasta al tope---");
                return BLOQUE_OPERATIVO;
            }
            return TODO_OK;
        }
    }
    return TODO_OK;

}
int bloqueoPorDES(tJornada* jor,int capZonas)
{
    if(capZonas == 0)
    {
        puts("---!No hay espacio suficiente para DESCARGAR---");
        return BLOQUE_OPERATIVO;
    }
    if(!hayAtracosDeMuelles(&jor->lMuelles) && colaVacia(&jor->cBuques))
    {
        puts("---!No hay buques en los ni por llegar---");
        return BLOQUE_OPERATIVO;
    }
    return TODO_OK;
}

void printHistoria(const void* a)
{
    const tHistoriaComando* hist = (const tHistoriaComando*)a;
    printf("Nombre:%4s\nOrigen:%5s\nDestino:%5s\nContenedor:%5s\n",hist->codComando,hist->origen,hist->destino,hist->contenedor);
    printf("Tiempo Inicio:%d\nTiempo Final:%d\n",hist->tiempoInicio,hist->tiempoFinal);
}
