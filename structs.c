#include <stdbool.h>

#define SIZECATEGORYANDGEM 20
#define SIZECOLOR 15
#define SIZEMETAL 8

#define SIZEDATE 11
#define SIZETIME 9
#define MAXITENSORDER 26

typedef struct
{
    int numeroRegistros;
    int numeroInsercoes;
    int numeroExclusoes;
    unsigned long int deslocInicio;
}HEADER;

typedef struct
{
    unsigned long int id;
    double price;
    unsigned long int elo;
    char mainMetal[SIZEMETAL];
    char mainColor[SIZECOLOR];
    char productGender;
    char category[SIZECATEGORYANDGEM];
    char mainGem[SIZECATEGORYANDGEM];
    bool excluido;
}JOIA;
typedef struct
{
    unsigned long int items[MAXITENSORDER];
    unsigned long int id;
    unsigned long int elo;
    char date[SIZEDATE];
    char time[SIZETIME];
    int countItems;
    bool excluido;
}ORDER;

typedef struct
{
    unsigned long int ultimoDoBloco;
    unsigned long int deslocamento;
    int numeroRegistrosNoBloco;
}INDICE;