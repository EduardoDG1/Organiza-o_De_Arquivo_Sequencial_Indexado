#include "consultas.c"

int main()
{
    FILE *fOrder = fopen("order.dat","rb+");
    FILE *fJewelry = fopen("jewelry.dat","rb+");

    if(!fOrder || !fJewelry)
    {
        criarDataFiles();
        fOrder = fopen("order.dat","rb+");
        fJewelry = fopen("jewelry.dat","rb+");
    }
   
    while(true)
    {
        printf("LOJA DE JOIAS\n----------------------------");
        printf("1 - Mostra joias\n");
        printf("2 - Mostra joias\n");
        printf("3 - Mostra joias\n");
        printf("4 - Mostra joias\n");
        printf("5 - Mostra joias\n");
        printf("6 - Mostra joias\n");
    }

    return 0;    
}