#include "criacaoDataFile.c"

#define REGISTROSBLOCO 16

void mostrarPedidos(FILE *f){
    ORDER order;
    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    printf("%d\n",header.numeroRegistros);
    printf("%d\n",header.numeroInsercoes);
    printf("%d\n",header.numeroExclusoes);
    while(fread(&order,sizeof(ORDER),1,f))
    {
        if(!order.excluido)
            printf("%s %s UTC - %lu - %d\n",order.date, order.time, order.id, order.countItems);
    }
}

void mostrarJoias(FILE *f){
    JOIA joia;
    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    printf("%d\n",header.numeroRegistros);
    printf("%d\n",header.numeroInsercoes);
    printf("%d\n",header.numeroExclusoes);
    while(fread(&joia,sizeof(JOIA),1,f))
    {
        if(!joia.excluido)
            printf("%lu - %s - $%.2lf - %c - %s - %s - %s\n",joia.id, joia.category, joia.price, joia.productGender, joia.mainColor, joia.mainMetal, joia.mainGem);
    }
}

unsigned long int pesquisaBinariaArqIndice(char *arqInfo, char nomeArq[30], unsigned int cod)
{
    FILE *infoInd = fopen(arqInfo,"r");
    if(!infoInd)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }
    
    int qtdNiveis, qtd = 1, i;

    fscanf(infoInd,"Quantidade de niveis: %d\n",&qtdNiveis);

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
    return desloc;
}

ORDER pesquisaOrder(FILE *f, unsigned long int cod){
    ORDER order;
    unsigned long int desloc = pesquisaBinariaArqIndice("orderIndInfo.txt","orderLvl%d.ind",cod);
    fseek(f,desloc,SEEK_SET);

    int i;

    for (i = 0; i < REGISTROSBLOCO; i++)
    {
        bool leuCerto = fread(&order,sizeof(ORDER),1,f);
        if(!leuCerto) break;
        desloc += sizeof(ORDER);
        if(order.elo != -1)
        {
            while(order.elo != -1)
            {
                fseek(f,order.elo,SEEK_SET);
                fread(&order,sizeof(ORDER),1,f);

                ORDER aux;
                fseek(f,desloc,SEEK_SET);
                fread(&aux,sizeof(ORDER),1,f);
                if(aux.id == order.id) break;

                if(order.id == cod && !order.excluido) return order;
            }
            fseek(f,desloc,SEEK_SET);
        }
        if(order.id == cod && !order.excluido) return order;
    }
    
    printf("Pedido nao encontrado!\n");
}


JOIA pesquisaBinariaJewelry(FILE *f, unsigned long int cod){
    JOIA jewelry;
    
    unsigned long int desloc = pesquisaBinariaArqIndice("jewelryIndInfo.txt","jewelryLvl%d.ind",cod);
    fseek(f,desloc,SEEK_SET);

    int i;

    for (i = 0; i < REGISTROSBLOCO; i++)
    {
        bool leuCerto = fread(&jewelry,sizeof(JOIA),1,f);
        if(!leuCerto) break;
        if(jewelry.elo != -1)
        {
            while(jewelry.elo != -1)
            {
                fseek(f,jewelry.elo,SEEK_SET);
                fread(&jewelry,sizeof(JOIA),1,f);

                JOIA aux;
                fseek(f,desloc,SEEK_SET);
                fread(&aux,sizeof(ORDER),1,f);
                if(aux.id == jewelry.id) break;

                if(jewelry.id == cod && !jewelry.excluido) return jewelry;
            }
            fseek(f,desloc,SEEK_SET);
        }
        if(jewelry.id == cod && !jewelry.excluido) return jewelry;
    }

    printf("Joia nao encontrada!\n");
}

double calculaTotalPedido(FILE *fOrder, FILE *fJewelry, unsigned long int cod)
{
    ORDER order = pesquisaOrder(fOrder,cod);

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
    fseek(fOrder,sizeof(HEADER),SEEK_SET);
    fseek(fJewelry,sizeof(HEADER),SEEK_SET);
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