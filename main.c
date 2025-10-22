#include "consultas.c"

int main()
{
    // criarDataFiles();
    FILE *fOrder = fopen("order.dat","rb+");
    FILE *fJewelry = fopen("jewelry.dat","rb+");

    // if(!fOrder || !fJewelry)
    // {
    //     fOrder = fopen("order.dat","rb+");
    //     fJewelry = fopen("jewelry.dat","rb+");
    // }
   
    JOIA joia = pesquisaBinariaJewelry(fJewelry,1842214461889315556);
    printf("%lu\n",joia.id);


    return 0;    
}