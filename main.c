#include <string.h>

#include "operacoes.c"

int main()
{
    criarDataFiles();
    FILE *fOrder = fopen("order.dat","rb+");
    FILE *fJewelry = fopen("jewelry.dat","rb+");

    // if(!fOrder || !fJewelry)
    // {
    //     criarDataFiles();    
    //     fOrder = fopen("order.dat","rb+");
    //     fJewelry = fopen("jewelry.dat","rb+");
    // }
   
    // 1 - Mostrar pedidos e joias;
    // printf("Pedidos:\n");
    // mostrarPedidos(fOrder);
    // printf("\n\nJoias:\n");
    // mostrarJoias(fJewelry);

    //2 - Consultar um pedido e uma joia;
    // ORDER order = pesquisaBinariaOrder(fOrder,2718537529266536812);
    // printf("%lu\n",order.id);
    // JOIA joia = pesquisaBinariaJewelry(fJewelry,1956663836694020322);
    // printf("%lu\n",joia.id);

    // 3 - Consultas extras;
    // double totalPedido = calculaTotalPedido(fOrder,fJewelry,2718537529266536812);
    // printf("R$%.2lf\n",totalPedido);
    // double totalPedidos = calculaTotalDosPedidos(fOrder,fJewelry);
    // printf("R$%.2lf\n",totalPedidos);

    // 4 - Insercao
    ORDER pedido;
    pedido.countItems = 0;
    strcpy(pedido.date,"teste");
    strcpy(pedido.time, "teste");
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);
    insercaoOrder(fOrder,pedido);

    JOIA joia;
    strcpy(joia.category ,"teste");
    strcpy(joia.mainColor, "teste");
    strcpy(joia.mainMetal ,"teste");
    strcpy(joia.mainGem, "teste");
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);
    insercaoJewelry(fJewelry,joia);

    //5 - Remocao
    remocaoOrder(fOrder,1924719191579951782);
    remocaoOrder(fOrder,1924899396621697920);
    remocaoOrder(fOrder,1925511016616034733);
    remocaoOrder(fOrder,1925626951238681511);
    remocaoOrder(fOrder,1925740842841014667);

    remocaoJewelry(fJewelry,2501331816804253948);
    remocaoJewelry(fJewelry,2537782512956997930);
    remocaoJewelry(fJewelry,2539957035806490883);
    remocaoJewelry(fJewelry,2540059984184476519);
    remocaoJewelry(fJewelry,2541962442884252323);

    // 6 - Reorganização, definir limite de insercoes e exclusoes para implementar
    reorganizacaoArquivoOrder(fOrder);
    reorganizacaoArquivoJewelry(fJewelry);
    fOrder = fopen("order.dat","rb+");
    fJewelry = fopen("jewelry.dat","rb+");

    // mostrarPedidosComExcluidos(fOrder);
    // mostrarJoiasComExcluidos(fJewelry);


    return 0;    
}