#include <stdbool.h>

#define SIZECATEGORYANDGEM 20
#define SIZECOLOR 15
#define SIZEMETAL 8

#define SIZEDATE 11
#define SIZETIME 9
#define MAXITENSORDER 26

#define MEMORYREGISTERS 250
#define FILELIMIT 21

typedef struct
{
    unsigned long int id;
    char category[SIZECATEGORYANDGEM];
    double price;
    char productGender;
    char mainColor[SIZECOLOR];
    char mainMetal[SIZEMETAL];
    char mainGem[SIZECATEGORYANDGEM];
    bool excluido;
    unsigned long int elo;
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