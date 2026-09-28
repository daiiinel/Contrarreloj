#include "jornada.h"


/*
PREGUNTAS PROFE:

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



int main()
{
    int opc;
    char nombreOperador[MAX_NOMBRE_OP];
    

    srand(time(NULL));

    do
    {
        puts(MENU_INICIO);
        scanf("%d", &opc);

        switch(opc)
        {
            case 1:
                printf("Ingrese su nombre: ");
                fflush(stdin);
                gets(nombreOperador);
                nuevaJornada(nombreOperador);
                break;
            case 2:
                //verRanking(); //--prox en operadores.h (persistencia de datos)
                break;
            case 3:
                break;
            default: puts("Opcion invalida, reingrese..");
                    system("pause");
                    break;
        }

        system("cls");
    }while(opc!=3);

    return 0;
}
