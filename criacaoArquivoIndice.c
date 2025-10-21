#include <stdio.h>
#include <math.h>

#include "structs.c"

#define BLOCKSIZE 4096

typedef struct
{
    unsigned long int ultimoDoBloco;
    unsigned long int deslocamento;
    int numeroRegistrosNoBloco;
}INDICE;

void criarArquivoIndicePedidos(int n, FILE *f)
{
    int bfrDataFile = (int)floor(BLOCKSIZE/sizeof(ORDER));
    int nblocos = (int)(ceil((float)n/bfrDataFile));

    int nivel = 1;

    int i,j;
    char nomeArq[30];
    FILE *arqIndice = fopen("orderLvl1.ind","wb");
    FILE *infoArqIndice = fopen("orderIndInfo.txt","w");

    INDICE ind;

    unsigned int desloc = sizeof(int);
    ind.deslocamento = desloc;

    for (i = 0; i < nblocos; i++)
    {
        if(i == nblocos-1)
        {
            ORDER aux;
            ind.numeroRegistrosNoBloco = 0;
            while(fread(&aux,sizeof(ORDER),1,f))
            {
                ind.numeroRegistrosNoBloco++;
            }
            fseek(f,-sizeof(ORDER),SEEK_END);
            fread(&aux,sizeof(ORDER),1,f);
            ind.ultimoDoBloco = aux.id;
            fwrite(&ind,sizeof(INDICE),1,arqIndice);
            fclose(arqIndice);
            fclose(f);
            nblocos = (int)ceil((float)nblocos/bfrDataFile);
            break;
        }
        for (j = 0; j < bfrDataFile; j++)
        {
            if(j == bfrDataFile-1)
            {   
                ORDER auxOrder;
                fseek(f,desloc,SEEK_SET);
                fread(&auxOrder,sizeof(ORDER),1,f);
                ind.ultimoDoBloco = auxOrder.id;
                ind.numeroRegistrosNoBloco = bfrDataFile;
                fwrite(&ind,sizeof(INDICE),1,arqIndice);
            } 
            desloc += sizeof(ORDER);
        }
        ind.deslocamento = desloc;
    }

    desloc = 0;
    ind.deslocamento = desloc;
    bool ultimo = false;

    while(nblocos >= 1 && !ultimo)
    {
        if(nblocos == 1) ultimo = true; 
        sprintf(nomeArq,"orderLvl%d.ind",nivel++);
        f = fopen(nomeArq,"rb");
        sprintf(nomeArq,"orderLvl%d.ind",nivel);
        arqIndice = fopen(nomeArq,"wb");
        if(ultimo)
        {
            fwrite(&nblocos,sizeof(int),1,arqIndice);
            desloc += sizeof(int);
        }
        for (i = 0; i < nblocos; i++)
        {
            if(i == nblocos-1)
            {
                INDICE aux;
                ind.numeroRegistrosNoBloco = 0;
                while(fread(&aux,sizeof(INDICE),1,f))
                {
                    ind.numeroRegistrosNoBloco++;
                }
                fseek(f,-sizeof(INDICE),SEEK_END);
                fread(&aux,sizeof(INDICE),1,f);
                ind.ultimoDoBloco = aux.ultimoDoBloco;
                fwrite(&ind,sizeof(INDICE),1,arqIndice);
                fclose(arqIndice);
                fclose(f);
                nblocos = (int)ceil((float)nblocos/bfrDataFile);
                desloc = 0;
                ind.deslocamento = desloc;
                break;
            }
            for (j = 0; j < bfrDataFile; j++)
            {
                if(j == bfrDataFile-1)
                {   
                    INDICE auxInd;
                    fseek(f,desloc,SEEK_SET);
                    fread(&auxInd,sizeof(INDICE),1,f);
                    ind.ultimoDoBloco = auxInd.ultimoDoBloco;
                    ind.numeroRegistrosNoBloco = bfrDataFile;
                    fwrite(&ind,sizeof(INDICE),1,arqIndice);
                } 
                desloc += sizeof(INDICE);
            }
            ind.deslocamento = desloc;
        }
    }

    fprintf(infoArqIndice,"Quantidade de niveis: %d\n",nivel);
    fclose(infoArqIndice);
}   


void criarArquivoIndiceJoias(int n, FILE *f)
{
    int bfrDataFile = (int)floor(BLOCKSIZE/sizeof(JOIA));
    int nblocos = (int)(ceil((float)n/bfrDataFile));

    int nivel = 1;

    int i,j;
    char nomeArq[30];
    FILE *arqIndice = fopen("jewelryLvl1.ind","wb");

    FILE *infoArqIndice = fopen("jewelryIndInfo.txt","w");

    INDICE ind;

    unsigned int desloc = sizeof(int);
    ind.deslocamento = desloc;

    for (i = 0; i < nblocos; i++)
    {
        if(i == nblocos-1)
        {
            JOIA aux;
            ind.numeroRegistrosNoBloco = 0;
            while(fread(&aux,sizeof(JOIA),1,f))
            {
                ind.numeroRegistrosNoBloco++;
            }
            fseek(f,-sizeof(JOIA),SEEK_END);
            fread(&aux,sizeof(JOIA),1,f);
            ind.ultimoDoBloco = aux.id;
            fwrite(&ind,sizeof(INDICE),1,arqIndice);
            fclose(arqIndice);
            fclose(f);
            nblocos = (int)ceil((float)nblocos/bfrDataFile);
            break;
        }
        for (j = 0; j < bfrDataFile; j++)
        {
            if(j == bfrDataFile-1)
            {   
                JOIA auxJoia;
                fseek(f,desloc,SEEK_SET);
                fread(&auxJoia,sizeof(JOIA),1,f);
                ind.ultimoDoBloco = auxJoia.id;
                ind.numeroRegistrosNoBloco = bfrDataFile;
                fwrite(&ind,sizeof(INDICE),1,arqIndice);
            } 
            desloc += sizeof(JOIA);
        }
        ind.deslocamento = desloc;
    }

    desloc = 0;
    ind.deslocamento = desloc;
    bool ultimo = false;

    while(nblocos >= 1 && !ultimo)
    {
        if(nblocos == 1) ultimo = true; 
        sprintf(nomeArq,"jewelryLvl%d.ind",nivel++);
        f = fopen(nomeArq,"rb");
        sprintf(nomeArq,"jewelryLvl%d.ind",nivel);
        arqIndice = fopen(nomeArq,"wb");
        if(ultimo)
        {
          fwrite(&nblocos,sizeof(int),1,arqIndice);
          desloc += sizeof(int);  
        }
        for (i = 0; i < nblocos; i++)
        {
            if(i == nblocos-1)
            {
                INDICE aux;
                ind.numeroRegistrosNoBloco = 0;
                while(fread(&aux,sizeof(INDICE),1,f))
                {
                    ind.numeroRegistrosNoBloco++;
                }
                fseek(f,-sizeof(INDICE),SEEK_END);
                fread(&aux,sizeof(INDICE),1,f);
                ind.ultimoDoBloco = aux.ultimoDoBloco;
                fwrite(&ind,sizeof(INDICE),1,arqIndice);
                fclose(arqIndice);
                fclose(f);
                nblocos = (int)ceil((float)nblocos/bfrDataFile);
                desloc = 0;
                ind.deslocamento = desloc;
                break;
            }
            for (j = 0; j < bfrDataFile; j++)
            {
                if(j == bfrDataFile-1)
                {   
                    INDICE auxInd;
                    fseek(f,desloc,SEEK_SET);
                    fread(&auxInd,sizeof(INDICE),1,f);
                    ind.ultimoDoBloco = auxInd.ultimoDoBloco;
                    ind.numeroRegistrosNoBloco = bfrDataFile;
                    fwrite(&ind,sizeof(INDICE),1,arqIndice);
                } 
                desloc += sizeof(INDICE);
            }
            ind.deslocamento = desloc;
        }
    }

    fprintf(infoArqIndice,"Quantidade de niveis: %d\n",nivel);
    fclose(infoArqIndice);
}  