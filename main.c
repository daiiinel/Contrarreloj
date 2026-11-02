#include "constantes.h"
//#include "operadores.h"
#include "jornada.h"

/*
PREGUNTAS PROFE:
-- a la hora de generar el puerto, sigue siendo simulable el caso en el que
se indique que la cantidad de contenedores sea mayor a la cantidad de camiones?
de ser asi, a cada camion se le asigna un solo contenedor y los contenedores sobrantes quedan almacenados
de no ser asi, ..


de la cola de camiones hay que mostrar los 2 primeros? pedro
si si,en que momento se muestran los dos primeros?

tiene que haber una cola de antedidos? los primeros 3?
una cola de
esta cola queda intacta 1 2 3 4 5

en la funcion
verNPrimeros(tCola *pc, unsigned n) una copia de la cola
    creo una colaCopia que sea lo mismo que viene de pc de todos los nodos

    while(ver los n primeros)
        desacolar
        mostrar

    4 5 si me quedan cosas, las tiro vaciarCola

*/

//aux'spara ver que si se guardan los .bin y .idx¿
void verBinIdx(const char*nArch);
void verBinOp(const char*nArch);

int main()
{
    int opc,c;
    char nombreOperador[MAX_NOMBRE_OP];
    tJornada jornada;
    tEstadisticasOperadores datosOperador;

    tArbol arbolIdx; //para la persistencia de datos

    crearArbol(&arbolIdx);

    if(inicializarIndiceOperadores(&arbolIdx)!=TODO_OK)
        return ERROR_ARCH;

    srand(time(NULL));

    do
    {
        puts(MENU_INICIO);
        scanf("%d", &opc);

        while((c=getchar()) != '\n' && c != EOF);
        switch(opc)
        {
            case 1:
                    printf("Ingrese su nombre: ");
                    fflush(stdin);
                    gets(nombreOperador);

                    if(gestionarOperador(nombreOperador, &arbolIdx, &datosOperador)!=TODO_OK)
                    {
                        puts(MSG_ERROR_OPERADOR);
                        system("pause");
                        break;
                    }

                    if(nuevaJornada(&datosOperador, &jornada)==NO_SIMULABLE)
                    {
                        puts(MSG_NO_SIMULABLE);
                        break;
                    }


                    //tengo que bajar lo del minuto 0 ? yo o dai
                    //pregunto por el movimiento se lo pido al pibe

                break;
            case 2:
                //verRanking(); //--prox en operadores.h (persistencia de datos)
                break;
            case 3:
                break;
            default:
                puts("Opcion invalida, reingrese..");
                system("pause");
                break;
        }
//        verBinIdx(IDX_OPERADORES_ARCH);
//        verBinOp(OPERADORES_ARCH);

        system("cls");

    }while(opc!=3);


    vaciarArbol(&arbolIdx);
    return 0;
}


void verBinIdx(const char*nArch)
{
    FILE*pf=fopen(nArch, "rb");
    if(!pf)
        return;

    tOperadorIdx reg;

    while(fread(&reg, sizeof(tOperadorIdx),1,pf ))
        printf("%u - %s - %u\n", reg.id, reg.nombre, reg.pos);

    fclose(pf);
}

void verBinOp(const char*nArch)
{
    FILE*pf=fopen(nArch, "rb");
    if(!pf)
        return;

    tEstadisticasOperadores reg;

    while(fread(&reg, sizeof(tEstadisticasOperadores),1,pf ))
        printf("%u - %s - %u - %u \n", reg.id, reg.nomOperador, reg.cantMov, reg.puntuacion);

    fclose(pf);
}
