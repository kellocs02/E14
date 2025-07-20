#include "programma.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; //creiamo il mutex per sincronizzare l'accesso alle operazioni sulla struttura dati condivisa

void ADD(char* importo, char* causale, int client_fd){
        printf("siamo entrati nell'ADD\n");
        int importo_valore=atoi(importo); //convertiamo il numero da stringa a intero
        int aggiunto = 0;
        //mettiamo il lock sul mutex
        pthread_mutex_lock(&mutex);
        //ora possiamo modificare la struttura dati condivisa
        for(int i=0;i<MAX_MOVIMENTI;i++){
            
            if(movimenti[i].valido==0){
               
                movimenti[i].valido=1;
                movimenti[i].id=i;
                movimenti[i].importo=importo_valore;
                strncpy(movimenti[i].causale, causale, MAX_CAUSALE - 1);
                movimenti[i].causale[MAX_CAUSALE - 1] = '\0'; //ATTENZIONE FORSE DA MODIFICARE
                aggiunto = 1;
                break;
            }

        }
        pthread_mutex_unlock(&mutex);

        if (aggiunto) {
            send(client_fd, "Movimento aggiunto con successo\n", 33, 0);
        } else {
            send(client_fd, "Errore: elenco movimenti pieno\n", 31, 0);
        }
}

//modifica un movimento già registrato
void UPDATE(int id, char* importo, char* causale,int client_fd){
        //controlliamo che id sia nel range
        if (id < 0 || id >= MAX_MOVIMENTI) {
        send(client_fd, "Errore: ID fuori dal range\n", 28, 0);
        return;
        }

        pthread_mutex_lock(&mutex);
        int importo_valore=atoi(importo); //convertiamo il numero da stringa a intero
        int aggiunto=0;
        if(movimenti[id].valido==0){
            printf("Questo movimento non esiste\n");
        }else{
            printf("modifica in corso\n");
            movimenti[id].importo=importo_valore;
            strncpy(movimenti[id].causale, causale, MAX_CAUSALE - 1);
            movimenti[id].causale[MAX_CAUSALE - 1] = '\0';
            aggiunto=1;
        }
        pthread_mutex_unlock(&mutex);
        if (aggiunto) {
            send(client_fd, "Movimento aggiornato con successo\n", 34, 0);
        }else {
            send(client_fd, "Errore: movimento inesistente\n", 30, 0);
        }
}

void DELETE(int id,int client_fd){
    if (id < 0 || id >= MAX_MOVIMENTI) {
        send(client_fd, "Errore: ID fuori dal range\n", 28, 0);
        return;
    }
    pthread_mutex_lock(&mutex);
    //sezione critica
    movimenti[id].valido=0;
    //invalidiamo il movimento
    pthread_mutex_unlock(&mutex);
}

void LIST(int client_fd) {
    int BUFFER_SIZE=100;
    pthread_mutex_lock(&mutex);  // 🔒 Protezione sezione critica

    char risposta[BUFFER_SIZE];  // buffer per la risposta finale
    risposta[0] = '\0'; // lo inizializziamo a stringa vuota

    int trovati = 0;

    for (int i = 0; i < MAX_MOVIMENTI; i++) {
        if (movimenti[i].valido) {
            char riga[128];  // buffer temporaneo per ogni riga
            snprintf(riga, sizeof(riga), "ID: %d | Importo: %d | Causale: %s\n", 
                     movimenti[i].id, movimenti[i].importo, movimenti[i].causale);
            strncat(risposta, riga, BUFFER_SIZE - strlen(risposta) - 1);
            trovati++;
        }
    }

    pthread_mutex_unlock(&mutex);  // 🔓 Fine sezione critica

    if (trovati == 0) {
        send(client_fd, "Nessun movimento registrato\n", 29, 0);
    } else {
        send(client_fd, risposta, strlen(risposta), 0);
    }
}
