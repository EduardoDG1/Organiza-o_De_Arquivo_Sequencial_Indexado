#include "criacaoDataFile.c"

void mostrarPedidos(FILE *f){
    ORDER order;
    int cont, i;
    fread(&cont,sizeof(int),1,f);
    printf("%d\n",cont);
    while(fread(&order,sizeof(ORDER),1,f))
    {
        printf("%s %s UTC - %lu - %d\n",order.date, order.time, order.id, order.countItems);
    }
}

void mostrarJoias(FILE *f){
    JOIA joia;
    int qtd;
    fread(&qtd,sizeof(int),1,f);
    printf("%d\n",qtd);
    while(fread(&joia,sizeof(JOIA),1,f))
    {
        printf("%lu - %s - $%.2lf - %c - %s - %s - %s\n",joia.id, joia.category, joia.price, joia.productGender, joia.mainColor, joia.mainMetal, joia.mainGem);
    }
}

ORDER pesquisaBinariaOrder(FILE *f, unsigned long int cod){
    
    FILE *infoInd = fopen("orderIndInfo.txt","r");
    if(!infoInd)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }
    
    int qtdNiveis, qtd = 1, i;

    fscanf(infoInd,"Quantidade de niveis: %d\n",&qtdNiveis);

    char nomeArq[30];
    
    int inicio,fim;
    unsigned long int desloc = 0;

    for (i = qtdNiveis; i > 0; i--)
    {   
        sprintf(nomeArq,"orderLvl%d.ind",i);
        FILE *arqInd = fopen(nomeArq,"rb");
        INDICE ind; 
        inicio = 0;
        fim = qtd-1;

        while(inicio <= fim)
        {
            int meio = (inicio+fim)/2;
            fseek(arqInd,desloc+meio*sizeof(INDICE),SEEK_SET);
            fread(&ind,sizeof(INDICE),1,arqInd);
            if(cod <= ind.ultimoDoBloco)
            {
                fim = meio - 1;
            }
            else if(cod > ind.ultimoDoBloco)
            {
                inicio = meio + 1;
            }
        }
        fseek(arqInd,desloc+inicio*sizeof(INDICE),SEEK_SET);
        fread(&ind,sizeof(INDICE),1,arqInd);
        desloc = ind.deslocamento;
        qtd = ind.numeroRegistrosNoBloco;
        fclose(arqInd);
    }
    
    ORDER order;
    fseek(f,desloc,SEEK_SET);
    for (i = 0; i < qtd; i++)
    {
        fread(&order,sizeof(ORDER),1,f);
        if(order.id == cod) return order;
    }
    
    printf("Pedido nao encontrado!\n");
    exit(0);
}


JOIA pesquisaBinariaJewelry(FILE *f, unsigned long int cod){
    
    FILE *infoInd = fopen("jewelryIndInfo.txt","r");
    if(!infoInd)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }
    
    int qtdNiveis, qtd = 1, i;

    fscanf(infoInd,"Quantidade de niveis: %d\n",&qtdNiveis);

    char nomeArq[30];
    
    int inicio,fim;
    unsigned long int desloc = 0;

    for (i = qtdNiveis; i > 0; i--)
    {   
        sprintf(nomeArq,"jewelryLvl%d.ind",i);
        FILE *arqInd = fopen(nomeArq,"rb");
        INDICE ind; 
        inicio = 0;
        fim = qtd-1;

        while(inicio <= fim)
        {
            int meio = (inicio+fim)/2;
            fseek(arqInd,desloc+meio*sizeof(INDICE),SEEK_SET);
            fread(&ind,sizeof(INDICE),1,arqInd);
            if(cod <= ind.ultimoDoBloco)
            {
                fim = meio - 1;
            }
            else if(cod > ind.ultimoDoBloco)
            {
                inicio = meio + 1;
            }
        }
        fseek(arqInd,desloc+inicio*sizeof(INDICE),SEEK_SET);
        fread(&ind,sizeof(INDICE),1,arqInd);
        desloc = ind.deslocamento;
        qtd = ind.numeroRegistrosNoBloco;
        fclose(arqInd);
    }
    
    JOIA jewelry;
    fseek(f,desloc,SEEK_SET);
    for (i = 0; i < qtd; i++)
    {
        fread(&jewelry,sizeof(JOIA),1,f);
        if(jewelry.id == cod) return jewelry;
    }

    printf("Joia nao encontrada!\n");
    exit(0);
}

double calculaTotalPedido(FILE *fOrder, FILE *fJewelry, unsigned long int cod)
{
    ORDER order = pesquisaBinariaOrder(fOrder,cod);

    int i;
    double total = 0;

    for (i = 0; i < order.countItems; i++)
    {
        unsigned long int codJoia = order.items[i];
        JOIA joia = pesquisaBinariaJewelry(fJewelry,codJoia);
        total += joia.price;
    }
    return total;
}

double calculaTotalDosPedidos(FILE *fOrder, FILE *fJewelry)
{
    ORDER order;
    double total = 0;
    int i;
    fseek(fOrder,sizeof(int),SEEK_SET);
    fseek(fJewelry,sizeof(int),SEEK_SET);
    while(fread(&order,sizeof(ORDER),1,fOrder))
    {
        for (i = 0; i < order.countItems; i++)
        {
            JOIA joia = pesquisaBinariaJewelry(fJewelry,order.items[i]);
            total += joia.price;
        }
    }
    return total;
}