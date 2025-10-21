#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "criacaoArquivoIndice.c"

int gerarParticoesJewelryFile(FILE *f)
{
    int i, nArquivo = 1;
    JOIA joiasMem[MEMORYREGISTERS];
    bool congelados[MEMORYREGISTERS];
    bool gravar[MEMORYREGISTERS];

    for (i = 0; i < MEMORYREGISTERS; i++)
    {
        fread(&joiasMem[i],sizeof(JOIA),1,f);
        congelados[i] = false;
        gravar[i] = true;        
    }

    char nomePt[5] = "pt0";
    FILE *particao = fopen(nomePt,"wb");

    while(true)
    {
        int pos = -1;
        unsigned long int menor = ULONG_MAX;
        for (i = 0; i < MEMORYREGISTERS; i++)
        {
            if(joiasMem[i].id < menor && !congelados[i])
            {
                pos = i;
                menor = joiasMem[i].id;
            }
        }

        fwrite(&joiasMem[pos],sizeof(JOIA),1,particao);
        bool leuCerto = fread(&joiasMem[pos],sizeof(JOIA),1,f);
        
        if(leuCerto)
        {
            if(joiasMem[pos].id < menor)
            {
                congelados[pos] = true;
            }   
        }
        else{
            congelados[pos] = true;
            gravar[pos] = false;
        }

        bool fecharPart = true;

        for (i = 0; i < MEMORYREGISTERS; i++)
        {
            if(!congelados[i])
            {
                fecharPart = false;
                break;
            }
        }
        
        if(fecharPart)
        {
            fclose(particao);
            if(!leuCerto)
            {
                sprintf(nomePt,"pt%d",nArquivo++);
                particao = fopen(nomePt,"wb");

                while (true)
                {
                    int pos = -1;
                    unsigned long int menor = ULONG_MAX;
                    for (i = 0; i < MEMORYREGISTERS; i++)
                    {
                        if(joiasMem[i].id < menor && gravar[i])
                        {
                            pos = i;
                            menor = joiasMem[i].id;
                        }
                    }
                    fwrite(&joiasMem[pos],sizeof(JOIA),1,particao);
                    gravar[pos] = false;
                    bool continuar = false;
                    for (i = 0; i < MEMORYREGISTERS; i++)
                    {
                        if(gravar[i])
                        {
                            continuar = true;
                            break;
                        }
                    }
                    if(!continuar){
                        fclose(particao);
                        return nArquivo;
                    }
                }
            }
            for (i = 0; i < MEMORYREGISTERS; i++)
            {
                congelados[i] = false;   
            }
            sprintf(nomePt,"pt%d",nArquivo++);
            particao = fopen(nomePt,"wb");
        }
    }
}

void intercalarParticoesJewelryFile(int nParticoes, int contJoias)
{
    int i = 0;
    char nomePt[5];
    FILE **vet = (FILE **)malloc(sizeof(FILE*)*nParticoes);
    JOIA *vetJoias = (JOIA *)malloc(sizeof(JOIA)*nParticoes);
    bool *endFiles = (bool *)malloc(sizeof(bool)*nParticoes);

    for (i = 0; i < FILELIMIT-1; i++)
    {
        sprintf(nomePt,"pt%d",i);
        vet[i] = fopen(nomePt,"rb");
        fread(&vetJoias[i],sizeof(JOIA),1,vet[i]);
        endFiles[i] = false;
    }
    
    FILE *finalFile = fopen("jewelry.dat","wb");
    fwrite(&contJoias,sizeof(int),1,finalFile);

    while(true)
    {
        int pos = -1;
        unsigned long int menor = ULONG_MAX;

        for (i = 0; i < FILELIMIT-1; i++)
        {
            if(vetJoias[i].id < menor && !endFiles[i])
            {
                pos = i;
                menor = vetJoias[i].id;
            }
        }
        if(pos != -1)
        {
            fwrite(&vetJoias[pos],sizeof(JOIA),1,finalFile);
            bool leuCerto = fread(&vetJoias[pos],sizeof(JOIA),1,vet[pos]);
            if(!leuCerto){
                endFiles[pos] = true;
                fclose(vet[pos]);
            }
        }
        else
        {
            fclose(finalFile);
            break;
        }
    }
    for (i = 0; i < FILELIMIT-1; i++)
    {
        sprintf(nomePt,"pt%d",i);
        remove(nomePt);
    }
    free(vet);
    free(vetJoias);
    free(endFiles);
}

void criarDataFiles()
{
    FILE *csvFile = fopen("jewelry.csv","r");

    if(!csvFile)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    FILE *orderDataFile = fopen("order.dat","wb+");

    if(!orderDataFile)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    FILE *jewelryDataFile = fopen("jewelry.dat", "wb+");

    if(!jewelryDataFile)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    ORDER order;
    order.countItems = 0;
    int contOrder = 0;
    JOIA joia;
    int contJoias = contOrder;

    bool gravaPedido = true;

    fwrite(&contOrder,sizeof(int),1,orderDataFile);
    while(fscanf(csvFile,"%s %s UTC,%lu,%lu,%s ,%f,%c,%s ,%s ,%s", order.date, order.time, &order.id, &joia.id, joia.category, &joia.price, &joia.productGender, joia.mainColor, joia.mainMetal, joia.mainGem) == 10)
    {
        order.countItems = 0;
        joia.excluido = false;
        joia.elo = 0;
        bool gravaJoia = true;
        JOIA auxJoia;
        fseek(jewelryDataFile,0,SEEK_SET);
        while(fread(&auxJoia,sizeof(JOIA),1,jewelryDataFile))
        {
            if(auxJoia.id == joia.id)
            {
                gravaJoia = false;
                break;
            }
        }
        if(gravaJoia)
        {
            fseek(jewelryDataFile,0,SEEK_END);
            fwrite(&joia,sizeof(JOIA),1,jewelryDataFile);
            contJoias++;
        }

        if(gravaPedido)
        {
            order.excluido = false;
            order.elo = 0;
            order.items[order.countItems++] = joia.id;
            fwrite(&order,sizeof(ORDER),1,orderDataFile);
            contOrder++;
            gravaPedido = false;
        }
        else
        {
            ORDER auxOrder;
            fseek(orderDataFile,-sizeof(ORDER),SEEK_CUR);
            fread(&auxOrder,sizeof(ORDER),1,orderDataFile);
            if(order.id == auxOrder.id)
            {
                auxOrder.items[auxOrder.countItems++] = joia.id;
                fseek(orderDataFile,-sizeof(ORDER),SEEK_CUR);
                fwrite(&auxOrder,sizeof(ORDER),1,orderDataFile);
            }
            else
            {
                order.excluido = false;
                order.elo = 0;
                order.items[order.countItems++] = joia.id;
                fwrite(&order,sizeof(ORDER),1,orderDataFile);
                contOrder++;
            }
        }
    }

    fseek(orderDataFile,0,SEEK_SET);
    fseek(jewelryDataFile,0,SEEK_SET);

    fwrite(&contOrder,sizeof(int),1,orderDataFile);
    fclose(csvFile);

    criarArquivoIndicePedidos(contOrder,orderDataFile);
    
    int nParticoes = gerarParticoesJewelryFile(jewelryDataFile);
    fclose(jewelryDataFile);
    remove("jewelry.dat");
    intercalarParticoesJewelryFile(nParticoes,contJoias);

    jewelryDataFile = fopen("jewelry.dat","rb");
    criarArquivoIndiceJoias(contJoias,jewelryDataFile);
}