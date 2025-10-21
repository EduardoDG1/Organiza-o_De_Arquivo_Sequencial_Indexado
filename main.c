#include "consultas.c"

int main()
{
    // criarDataFiles(); //Função para criar arquivos de dados;

    FILE *f1 = fopen("order.dat","rb");
    FILE *f2 = fopen("jewelry.dat","rb");

    float total = calculaTotalDosPedidos(f1,f2);
    printf("%f\n",total);

    // mostrarPedidos(f1);

    // fclose(f1);
    // fclose(f2);
    return 0;    
}