#include "criacaoDataFile.c"

void mostrarArqIndice(FILE *f)
{
    INDICE ind;
    int reg;
    fread(&reg,sizeof(int),1,f);
    printf("%d\n",reg);
    while(fread(&ind,sizeof(INDICE),1,f))
    {
        printf("%lu - %lu - %d\n",ind.ultimoDoBloco, ind.deslocamento,ind.numeroRegistrosNoBloco);
    }
}

void mostrarPedidos(FILE *f){
    ORDER order;
    int cont;
    fread(&cont,sizeof(int),1,f);
    printf("%d\n",cont);
    while(fread(&order,sizeof(ORDER),1,f))
    {
        printf("%s %s UTC - %lu - %d\n",order.date, order.time, order.id,order.countItems);
    }
}

void mostrarJoias(FILE *f){
    JOIA joia;
    int qtd;
    fread(&qtd,sizeof(int),1,f);
    printf("%d\n",qtd);
    while(fread(&joia,sizeof(JOIA),1,f))
    {
        printf("%lu - %s - $%.2f\n",joia.id, joia.category, joia.price);
    }
}

ORDER pesquisaBinariaOrder(FILE *f, unsigned long int cod){
    
    FILE *infoInd = fopen("orderIndInfo.txt","r");
    if(!infoInd)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }
    
    int qtdNiveis, qtd, i;

    fscanf(infoInd,"Quantidade de niveis: %d\n",&qtdNiveis);

    char nomeArq[30];
    
    int inicio,fim;
    unsigned long int desloc = 0;

    for (i = qtdNiveis; i > 0; i--)
    {   
        sprintf(nomeArq,"orderLvl%d.ind",i);
        FILE *arqInd = fopen(nomeArq,"rb");
        if(i == qtdNiveis)
        {
            fread(&qtd,sizeof(int),1,arqInd);
            desloc = sizeof(int);
        }
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
    inicio = 0, fim = qtd-1;
    while(inicio <= fim)
    {
        int meio = (inicio+fim)/2;
        fseek(f,desloc+meio*sizeof(ORDER),SEEK_SET);
        fread(&order,sizeof(ORDER),1,f);
        if(cod < order.id)
        {
            fim = meio - 1;
        }
        else if(cod > order.id)
        {
            inicio = meio + 1;
        }
        else{
            return order;
        }
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
    
    int qtdNiveis, qtd, i;

    fscanf(infoInd,"Quantidade de niveis: %d\n",&qtdNiveis);

    char nomeArq[30];
    
    int inicio,fim;
    unsigned long int desloc = 0;

    for (i = qtdNiveis; i > 0; i--)
    {   
        sprintf(nomeArq,"jewelryLvl%d.ind",i);
        FILE *arqInd = fopen(nomeArq,"rb");
        if(i == qtdNiveis)
        {
            fread(&qtd,sizeof(int),1,arqInd);
            desloc = sizeof(int);
        }
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
    inicio = 0, fim = qtd-1;
    while(inicio <= fim)
    {
        int meio = (inicio+fim)/2;
        fseek(f,desloc+meio*sizeof(JOIA),SEEK_SET);
        fread(&jewelry,sizeof(JOIA),1,f);
        if(cod < jewelry.id)
        {
            fim = meio - 1;
        }
        else if(cod > jewelry.id)
        {
            inicio = meio + 1;
        }
        else{
            return jewelry;
        }
    }

    printf("Joia nao encontrada!\n");
    exit(0);
}

float calculaTotalPedido(FILE *fOrder, FILE *fJewelry, unsigned long int cod)
{
    ORDER order = pesquisaBinariaOrder(fOrder,cod);

    int i;
    float total = 0;

    for (i = 0; i < order.countItems; i++)
    {
        unsigned long int codJoia = order.items[i];
        JOIA joia = pesquisaBinariaJewelry(fJewelry,codJoia);
        total += joia.price;
    }

    return total;
}

float calculaTotalDosPedidos(FILE *fOrder, FILE *fJewelry)
{
    ORDER order;
    float total = 0;
    fseek(fOrder,sizeof(int),SEEK_SET);
    fseek(fJewelry,sizeof(int),SEEK_SET);
    while(fread(&order,sizeof(ORDER),1,fOrder))
    {
        total += calculaTotalPedido(fOrder,fJewelry,order.id);
    }

    return total;
}