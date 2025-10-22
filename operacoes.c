#include <stdio.h>

#include "consultas.c"

void insercaoOrder(FILE *f, ORDER pedidoInserido)
{
    unsigned long int desloc = pesquisaBinariaArqIndice("orderIndInfo.txt","orderLvl%d.ind",pedidoInserido.id);

    fseek(f,0,SEEK_SET);
    HEADER headerArq;
    fread(&headerArq,sizeof(HEADER),1,f);

    unsigned long int deslocAreaExtensao = headerArq.numeroInsercoes*sizeof(ORDER)+sizeof(HEADER);
    
    ORDER aux, primeiro;

    fseek(f,headerArq.deslocInicio,SEEK_SET);
    fread(&primeiro,sizeof(ORDER),1,f);

    fseek(f,desloc,SEEK_SET);
    bool leuCerto = fread(&aux,sizeof(ORDER),1,f), entrar = true;
    while(aux.id < pedidoInserido.id && leuCerto)
    {  
        while(aux.elo != -1)
        {
            entrar = false;
            fseek(f,aux.elo,SEEK_SET);
            fread(&aux,sizeof(ORDER),1,f);
            if(aux.id < pedidoInserido.id)
            {
                pedidoInserido.elo = aux.elo;
                fread(&aux,sizeof(ORDER),1,f);
                aux.elo = deslocAreaExtensao;
                break;
            }
        }
        leuCerto = fread(&aux,sizeof(ORDER),1,f); 
        desloc += sizeof(ORDER);
    }
    
    if(aux.id < pedidoInserido.id)
    {
        pedidoInserido.elo = desloc;
        if(aux.id == primeiro.id)
        {
            headerArq.deslocInicio = deslocAreaExtensao;
        }
    }
    else if(entrar)
    {
        fseek(f,-sizeof(ORDER),SEEK_CUR);
        fread(&aux,sizeof(ORDER),1,f);
        if(leuCerto)
        {
            pedidoInserido.elo = desloc;
        }else
        {
            pedidoInserido.elo = -1;
        }
        aux.elo = deslocAreaExtensao;
    }

    fseek(f,-sizeof(ORDER),SEEK_CUR);
    fwrite(&aux,sizeof(ORDER),1,f);
    fseek(f,deslocAreaExtensao,SEEK_SET);
    fwrite(&pedidoInserido,sizeof(ORDER),1,f);

    printf("Insercao realizada com sucesso!\n");
}

void remocaoOrder(FILE *f, unsigned long int cod)
{  
    unsigned long int desloc = pesquisaBinariaArqIndice("orderIndInfo.txt","orderLvl%d.ind",cod);

    fseek(f,desloc,SEEK_SET);

    ORDER order;
    int i;

    for (i = 0; i < REGISTROSBLOCO; i++)
    {
        bool leuCerto = fread(&order,sizeof(ORDER),1,f);
        if(!leuCerto) break;
        if(cod == order.id)
        {
            order.excluido = 1;
            fseek(f,-sizeof(ORDER),SEEK_CUR);
            fwrite(&order,sizeof(ORDER),1,f);
            printf("Pedido removido com sucesso!\n");
            return;
        }
    }
    printf("Pedido nao encontrado!\n");
}

void insercaoJewelry(FILE *f, JOIA joiaInserida)
{
    unsigned long int desloc = pesquisaBinariaArqIndice("jewelryIndInfo.txt","jewelryLvl%d.ind",joiaInserida.id);

    fseek(f,0,SEEK_SET);
    HEADER headerArq;
    fread(&headerArq,sizeof(HEADER),1,f);

    unsigned long int deslocAreaExtensao = headerArq.numeroInsercoes*sizeof(JOIA)+sizeof(HEADER);
    
    JOIA aux, primeiro;

    fseek(f,headerArq.deslocInicio,SEEK_SET);
    fread(&primeiro,sizeof(JOIA),1,f);

    fseek(f,desloc,SEEK_SET);
    bool leuCerto = fread(&aux,sizeof(JOIA),1,f), entrar = true;
    while(aux.id < joiaInserida.id && leuCerto)
    {  
        while(aux.elo != -1)
        {
            entrar = false;
            fseek(f,aux.elo,SEEK_SET);
            fread(&aux,sizeof(JOIA),1,f);
            if(aux.id < joiaInserida.id)
            {
                joiaInserida.elo = aux.elo;
                fread(&aux,sizeof(JOIA),1,f);
                aux.elo = deslocAreaExtensao;
                break;
            }
        }
        leuCerto = fread(&aux,sizeof(JOIA),1,f); 
        desloc += sizeof(JOIA);
    }
    
    if(aux.id < joiaInserida.id)
    {
        joiaInserida.elo = desloc;
        if(aux.id == primeiro.id)
        {
            headerArq.deslocInicio = deslocAreaExtensao;
        }
    }
    else if(entrar)
    {
        fseek(f,-sizeof(JOIA),SEEK_CUR);
        fread(&aux,sizeof(JOIA),1,f);
        if(leuCerto)
        {
            joiaInserida.elo = desloc;
        }else
        {
            joiaInserida.elo = -1;
        }
        aux.elo = deslocAreaExtensao;
    }

    fseek(f,-sizeof(JOIA),SEEK_CUR);
    fwrite(&aux,sizeof(JOIA),1,f);
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
    unsigned long int desloc = pesquisaBinariaArqIndice("jewelryIndInfo.txt","jewelryLvl%d.ind",cod);

    fseek(f,desloc,SEEK_SET);

    JOIA joia;
    int i;

    for (i = 0; i < REGISTROSBLOCO; i++)
    {
        bool leuCerto = fread(&joia,sizeof(JOIA),1,f);
        if(!leuCerto) break;
        if(cod == joia.id)
        {
            joia.excluido = 1;
            fseek(f,-sizeof(JOIA),SEEK_CUR);
            fwrite(&joia,sizeof(JOIA),1,f);
            printf("Joia removida com sucesso!\n");

            HEADER headerArq;
            fseek(f,0,SEEK_SET);
            fread(&headerArq,sizeof(HEADER),1,f);
            headerArq.numeroExclusoes++;
            headerArq.numeroRegistros--;
            fseek(f,0,SEEK_SET);
            fwrite(&headerArq,sizeof(HEADER),1,f);

            return;
        }
    }
    printf("Joia nao encontrada!\n");
    return;
}

void reorganizacaoArquivoOrder(FILE *f)
{
    FILE *arqAuxiliar = fopen("arqAuxiliar.dat","wb");
    if(!arqAuxiliar)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    ORDER order;
    fseek(f,header.deslocInicio,SEEK_SET);

    while(fread(&order,sizeof(ORDER),1,f))
    {
        while(order.elo != -1)
        {
            if(!order.excluido)
            {
                fwrite(&order,sizeof(ORDER),1,arqAuxiliar);
            }
            fseek(f,order.elo,SEEK_SET);
            fread(&order,sizeof(ORDER),1,f);
        }
        if(!order.excluido)
        {
            fwrite(&order,sizeof(ORDER),1,arqAuxiliar);
        }
    }

    fclose(f);
    remove("order.dat");

    FILE *infoInd = fopen("orderIndIndo.txt","r");
    int n, i;
    fscanf(infoInd,"%d",&n);

    fclose(infoInd);

    char nomeArq[30];
    for (i = n; i > 0; i--)
    {
        sscanf(nomeArq,"orderLvl%d.ind",i);
        remove(nomeArq);
    }
    
    criarArquivoIndicePedidos(header.numeroRegistros,arqAuxiliar);
    rename("arqAuxiliar.dat","order.dat");
    f = arqAuxiliar;
}

void reorganizacaoArquivoJewelry(FILE *f)
{
    FILE *arqAuxiliar = fopen("arqAuxiliar.dat","wb");
    if(!arqAuxiliar)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    HEADER header;
    fread(&header,sizeof(HEADER),1,f);
    fwrite(&header,sizeof(HEADER),1,arqAuxiliar);

    JOIA joia;
    fseek(f,header.deslocInicio,SEEK_SET);

    while(fread(&joia,sizeof(JOIA),1,f))
    {
        while(joia.elo != -1)
        {
            if(!joia.excluido)
            {
                fwrite(&joia,sizeof(JOIA),1,arqAuxiliar);
            }
            fseek(f,joia.elo,SEEK_SET);
            fread(&joia,sizeof(JOIA),1,f);
        }
        if(!joia.excluido)
        {
            fwrite(&joia,sizeof(JOIA),1,arqAuxiliar);
        }
    }

    fclose(f);
    remove("jewelry.dat");

    FILE *infoInd = fopen("jewelryIndIndo.txt","r");
    int n, i;
    fscanf(infoInd,"%d",&n);
    fclose(infoInd);

    char nomeArq[30];
    for (i = n; i > 0; i--)
    {
        sscanf(nomeArq,"jewelryLvl%d.ind",i);
        remove(nomeArq);
    }
    
    criarArquivoIndiceJoias(header.numeroRegistros,arqAuxiliar);
    rename("arqAuxiliar.dat","jewelry.dat");
    f = arqAuxiliar;
}