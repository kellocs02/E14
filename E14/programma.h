#ifndef PROGRAMMA_H
#define PROGRAMMA_H
#define MAX_CAUSALE 100
#define MAX_MOVIMENTI 10

typedef struct {
    int id;                 // ID univoco dell'operazione
    int importo;          // Importo (positivo/negativo)
    char causale[MAX_CAUSALE];  // Descrizione
    int valido;             // Flag per sapere se lo slot è occupato : 0->occupato e 1->valido
} Movimento;

extern Movimento movimenti[MAX_MOVIMENTI]; 

void ADD(char* importo, char* causale, int client_fd);

void DELETE(int id,int client_fd);

void UPDATE(int id, char* importo, char* causale,int client_fd);

void LIST(int client_fd);

void AnalisiStringa(char *buffer, int client_fd);

#endif
