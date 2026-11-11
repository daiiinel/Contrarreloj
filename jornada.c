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
    while(i <= param->cantZona)
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

    return TODO_OK;
}

//esperar dentro de comandos
//arrvis de camiones y buques por separado
//hago el printeo de la jornada finalizada por k razon;

int desarrolloJornada(tJornada* jor, tParametros *param, tEstadisticasOperadores* datosOperador)
{
    int estado, pena=0;
    tLista comandos;
    tCamiones cam;

    cargarVectorComandos(&comandos, param);
    // arrivo los que llegan en el momento 0;

    system("cls");

    printf("---------Inicio de una nueva jornada---------\n");

    while((estado=estadoDeJornada(jor, comandos))==SIGUE)
    {
        procesarArrivos(jor);
        printf("Tiempo: %d\n", jor->tiempo);
        verPrimeros(jor);
        ejecutarComandoOperacion(jor, &comandos);


        system("pause");
        system("cls");
    }

    if(verPrimeroEnCola(&(jor->camionesEnTiempo), &cam, TAM_CAMION)!=VACIO)
    {
        pena=contarPuntajeNegativo(&(jor->camionesEnTiempo));
        jor->puntaje-=(pena*PUNTAJE_PENALIZADOR);
    }
    printf(MSG_FIN_JORNADA, jor->tiempo, jor->tiempo_maximo, jor->puntaje, pena);
    system("pause");
   // paso al hstorial del operador, lo hace dai por el arbol
   archivoFinJornada(jor, datosOperador->id);

    datosOperador->puntuacion += jor->puntaje;


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
int estadoDeJornada(tJornada *jor, tLista coman)
{
    if(jor->tiempo >= jor->tiempo_maximo)
        return SIN_TIEMPO;

    // si todas las colas estan vacias es porque
    // no hay mas buques en ningun lado  y  no hay mas camiones
    if(colaVacia(&(jor->buquesEnTiempo)) && colaVacia(&(jor->camionesEnTiempo)) && colaVacia(&(jor->cBuques)) && colaVacia(&(jor->cCamiones)))
        return FIN_JORNADA;

    // llamo a la funcion de pedro del bloqueo

    return SIGUE;
}
void procesarEnMedio(tJornada* jor, int tiempoUso)
{
    tBuque auxBuque;
    tCamiones auxCamion;

    verPrimeroEnCola(&(jor->cBuques), &auxBuque, TAM_BUQUE);
    verPrimeroEnCola(&(jor->cCamiones), &auxCamion, TAM_CAMION);
    if(auxBuque.tiempoLlegada < (jor->tiempo + tiempoUso))
        arrivoBuques(jor);
    if(auxCamion.tiempoLlegada < (jor->tiempo + tiempoUso))
        arrivoCamiones(jor);
}


int contarPuntajeNegativo(tCola *colaCamiones)
{
    tCamiones camion;
    int cantidad=0;
    while(!(colaVacia(colaCamiones)))
    {
        sacarDeCola(colaCamiones, &camion, TAM_CAMION);
        cantidad++;
    }
    return cantidad;
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
    }else if(colaVacia(&(jor->buquesEnTiempo)) && colaVacia(&(jor->camionesEnTiempo)) && colaVacia(&(jor->cBuques)) && colaVacia(&(jor->cCamiones)) && jor->tiempo<jor->tiempo_maximo)
        jor->tiempo+= (jor->tiempo_maximo - jor->tiempo);
}

int procesarArrivos(tJornada *jor)
{
    tCamiones auxCamion;
    tBuque auxBuque;

    verPrimeroEnCola(&(jor->cBuques),&auxBuque,TAM_BUQUE);
    verPrimeroEnCola(&(jor->cCamiones),&auxCamion,TAM_CAMION);
    arrivoBuques(jor);
    acercarBuques(jor);
    arrivoCamiones(jor);
/*
    if(jor->tiempo==0)
    {

        if(auxBuque.tiempoLlegada < auxCamion.tiempoLlegada)
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

        }
    }
    else
    {
        arrivoCamiones(jor);
        arrivoBuques(jor);
        acercarBuques(jor);

    }
*/
    return TODO_OK;
}

void liberarMuelle(tJornada* jor, tMuelle*mue)
{
    if(pilaVacia(&(mue->buque->contenedores)))
    {
        mue->estado=false;
        mue->buque=NULL;
        tListaEncontrarHacer(&(jor->lMuelles), mue,TAM_MUELLE, cmpMuelleNom, actualizarMuelleMartu);
        jor->puntaje+=PUNTAJE_DESEMBARCO;
        acercarBuques(jor);
    }
}
// saco de la jornada lo que tengo
int arrivoCamiones(tJornada *jor)
{
    tCamiones atender;
    verPrimeroEnCola(&(jor->cCamiones), &atender, TAM_CAMION);
    while(!(colaVacia(&(jor->cCamiones)))&& atender.tiempoLlegada <= (jor->tiempo) )
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
    while(atender.tiempoLlegada <= (jor->tiempo) && !(colaVacia(&(jor->cBuques))))
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
    tBuque *auxBuque;
    bool refe=0;

    while(!colaVacia(&(jor->buquesEnTiempo)))
    {
        auxBuque=(tBuque*)malloc(TAM_BUQUE);
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

int archivoFinJornada(tJornada *jor, unsigned idOperador)
{
    FILE *pf=fopen(JORNADA_ARCH,"r+t");
    if(!pf)
    {
        pf=fopen(JORNADA_ARCH, "w+t");
        if(!pf)
            return ERROR_ARCH;
        //si entro aca es porque tuvo que crear el archivo entonces no habia nada antes
        fprintf(pf,"\t---------Historial de Jornadas---------\n");
        fprintf(pf, "%-5s %-15s %-10s %s\n", "ID","OPERADOR", "PUNTAJE","TIEMPO FINAL/DISPONIBLE");
    }
    else
        fseek(pf,0,SEEK_END);

    fprintf(pf,"%d %-15s %-10u %d/%d\n", idOperador, jor->operador, jor->puntaje,jor->tiempo, jor->tiempo_maximo);
    fclose(pf);

    return TODO_OK;
}

/// PARA EL HISTORIAL DE LOS OPERADORES TXT
/*
int archivoOperadores(tJornada *jor)
{
    FILE *pf=fopen(OPERADORES_ARCH,"r+t");
    if(!pf)
    {
        pf=fopen(OPERADORES_ARCH, "w+t");
        if(!pf)
            return ERROR_ARCH;
        //si entro aca es porque tuvo que crear el archivo entonces no habia nada antes
        fprintf(pf,"\t---------Historial de Operadores---------\n");
        fprintf(pf, "%-15s %-10s %-20s %s\n","OPERADOR", "PUNTAJE","CANT. MOVIMIENTOS");
    }
    else
        fseek(pf,0,SEEK_END);

    fprintf(pf,"%-15s %-10u %-20d %d/%d\n",jor->operador, jor->puntaje,jor->puntaje,jor->tiempo, jor->tiempo_maximo);
    fclose(pf);

    return TODO_OK;
}
*/


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
