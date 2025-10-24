#include <stdio.h>

#include "consultas.c"

void insercaoOrder(FILE *f, ORDER pedidoInserido)
{
    fseek(f,0,SEEK_SET);
    HEADER headerArq;
    fread(&headerArq,sizeof(HEADER),1,f);

    unsigned long int deslocAreaExtensao = headerArq.numeroRegistros*sizeof(ORDER)+sizeof(HEADER);
    
    ORDER ultimo;

    fseek(f,deslocAreaExtensao-sizeof(ORDER),SEEK_SET);
    fread(&ultimo,sizeof(ORDER),1,f);
    pedidoInserido.id = ultimo.id+rand()%10000000000001;
                                               
    fseek(f,deslocAreaExtensao,SEEK_SET);
    fwrite(&pedidoInserido,sizeof(ORDER),1,f);

    headerArq.numeroInsercoes++;
    headerArq.numeroRegistros++;

    fseek(f,0,SEEK_SET);
    fwrite(&headerArq,sizeof(HEADER),1,f);

    printf("Insercao realizada com sucesso!\n");
}

void remocaoOrder(FILE *f, unsigned long int cod)
{  
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

    fseek(f,0,SEEK_SET);
    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    
    fseek(f,desloc,SEEK_SET);
    if(desloc == header.deslocUltimoBloco)
    {
        qtd += header.numeroInsercoes;
    }
    
    inicio = 0;
    fim = qtd-1;
    while(inicio <= fim)
    {
        int meio = (inicio+fim)/2;
        fseek(f,desloc+meio*sizeof(ORDER),SEEK_SET);
        fread(&order,sizeof(ORDER),1,f);
        if(order.id > cod)
        {
            fim = meio - 1;
        }
        else if(order.id < cod)
        {
            inicio = meio + 1;
        }
        else
        {
            order.excluido = 1;
            fseek(f,-sizeof(ORDER),SEEK_CUR);
            fwrite(&order,sizeof(ORDER),1,f);

            header.numeroRegistros--;
            header.numeroExclusoes++;
            fseek(f,0,SEEK_SET);
            fwrite(&header,sizeof(HEADER),1,f);

            printf("Pedido removido com sucesso!\n");
            return;
        }
    }

    printf("Pedido nao encontrado!\n");
}

void insercaoJewelry(FILE *f, JOIA joiaInserida)
{
    fseek(f,0,SEEK_SET);
    HEADER headerArq;
    fread(&headerArq,sizeof(HEADER),1,f);

    unsigned long int deslocAreaExtensao = headerArq.numeroRegistros*sizeof(JOIA)+sizeof(HEADER);
    
    JOIA ultimo;

    fseek(f,deslocAreaExtensao-sizeof(JOIA),SEEK_SET);
    fread(&ultimo,sizeof(JOIA),1,f);
    joiaInserida.id = ultimo.id+rand()%10000000000001;                                 

    fseek(f,deslocAreaExtensao,SEEK_SET);
    fwrite(&joiaInserida,sizeof(JOIA),1,f);

    headerArq.numeroInsercoes++;
    headerArq.numeroRegistros++;

    fseek(f,0,SEEK_SET);
    fwrite(&headerArq,sizeof(HEADER),1,f);

    printf("Insercao realizada com sucesso!\n");
}

void remocaoJewelry(FILE *f, unsigned long int cod)
{  
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

    JOIA joia;

    fseek(f,0,SEEK_SET);
    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    
    fseek(f,desloc,SEEK_SET);
    if(desloc == header.deslocUltimoBloco)
    {
        qtd += header.numeroInsercoes;
    }
    
    inicio = 0;
    fim = qtd-1;
    while(inicio <= fim)
    {
        int meio = (inicio+fim)/2;
        fseek(f,desloc+meio*sizeof(JOIA),SEEK_SET);
        fread(&joia,sizeof(JOIA),1,f);
        if(joia.id > cod)
        {
            fim = meio - 1;
        }
        else if(joia.id < cod)
        {
            inicio = meio + 1;
        }
        else
        {
            joia.excluido = 1;
            fseek(f,-sizeof(JOIA),SEEK_CUR);
            fwrite(&joia,sizeof(JOIA),1,f);

            header.numeroRegistros--;
            header.numeroExclusoes++;
            fseek(f,0,SEEK_SET);
            fwrite(&header,sizeof(HEADER),1,f);

            printf("Joia removida com sucesso!\n");
            return;
        }
    }
    printf("Joia nao encontrada!\n");
}

void reorganizacaoArquivoOrder(FILE *f)
{
    FILE *arqAuxiliar = fopen("arqAuxiliar.dat","wb+");
    if(!arqAuxiliar)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    HEADER header;
    fseek(f,0,SEEK_SET);
    fread(&header,sizeof(HEADER),1,f);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    header.numeroRegistros = 0;
    header.deslocUltimoBloco = 0;
    header.numeroInsercoes = 0;
    header.numeroExclusoes = 0;

    ORDER order;

    while(fread(&order,sizeof(ORDER),1,f))
    {
        if(!order.excluido)
        {
            fwrite(&order,sizeof(ORDER),1,arqAuxiliar);
            header.numeroRegistros++;
        }
    }

    fclose(f);
    remove("order.dat");

    FILE *infoInd = fopen("orderIndInfo.txt","r");
    int n, i;
    fscanf(infoInd,"Quantidade de niveis: %d\n",&n);

    fclose(infoInd);

    char nomeArq[30];
    for (i = n; i > 0; i--)
    {
        sprintf(nomeArq,"jewelryLvl%d.ind",i);
        remove(nomeArq);
    }
    
    fseek(arqAuxiliar,0,SEEK_SET);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    criarArquivoIndicePedidos(header.numeroRegistros,arqAuxiliar);
    rename("arqAuxiliar.dat","order.dat");
}

void reorganizacaoArquivoJewelry(FILE *f)
{
    FILE *arqAuxiliar = fopen("arqAuxiliar.dat","wb+");
    if(!arqAuxiliar)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    HEADER header;
    fseek(f,0,SEEK_SET);
    fread(&header,sizeof(HEADER),1,f);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    header.numeroRegistros = 0;
    header.deslocUltimoBloco = 0;
    header.numeroInsercoes = 0;
    header.numeroExclusoes = 0;

    JOIA joia;

    while(fread(&joia,sizeof(JOIA),1,f))
    {
        if(!joia.excluido)
        {
            fwrite(&joia,sizeof(JOIA),1,arqAuxiliar);
            header.numeroRegistros++;
        }
    }

    fclose(f);
    remove("jewelry.dat");

    FILE *infoInd = fopen("jewelryIndInfo.txt","r");
    int n, i;
    fscanf(infoInd,"Quantidade de niveis: %d\n",&n);
    fclose(infoInd);

    char nomeArq[30];
    for (i = n; i > 0; i--)
    {
        sprintf(nomeArq,"jewelryLvl%d.ind",i);
        remove(nomeArq);
    }
    
    fseek(arqAuxiliar,0,SEEK_SET);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    criarArquivoIndiceJoias(header.numeroRegistros,arqAuxiliar);
    rename("arqAuxiliar.dat","jewelry.dat");
}