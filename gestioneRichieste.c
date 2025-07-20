#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     //usata per la funzione send  
#include "programma.h"


//funzione che controlla se l'importo è un numero valido
int is_number(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) { //controlla ogni carattere
        //se il carattere è prima di 0 in ascii oppure dopo il 9 in ascii allora non è un numero
        if (str[i] < '0' || str[i] > '9') {
            return 0;  //numero non valido
        }
    }
    return 1;  //numero valido
}

void gestisciADD(char *buffer, int client_fd){
    char *cmd = strtok(buffer, " "); //prende il comando 
    char *importo_str = strtok(NULL, " "); //prende l'importo
    char *causale = strtok(NULL, "\n");  // prendo tutto il resto come causale
    ADD(importo_str,causale,client_fd);
}

void gestisciDELETE(char* buffer, int client_fd){
    char *cmd = strtok(buffer, " "); //prende il comando 
    char *id=strtok(NULL, "\n");
    if (id == NULL) {
        send(client_fd, "Errore: ID mancante\n", 21, 0);
        return;
    }
    int numero=atoi(id);
    DELETE(numero,client_fd);
}

//UPDATE ID IMPORTO CAUSALE
void gestisciUPDATE(char* buffer,int client_fd){
    char *cmd = strtok(buffer, " "); //prende il comando 
    char *id=strtok(NULL, " ");     //prende l'id
    char *importo_str = strtok(NULL, " "); //prende l'importo
    char *causale = strtok(NULL, "\n");  // prendo tutto il resto come causale
    int numero=atoi(id);
    UPDATE(numero,importo_str,causale,client_fd);
}


//funzione che prende il buffer dove il client invia il comando
//prende anche il fd del client
void AnalisiStringa(char *buffer, int client_fd) {
    //suddiviamo la stringa in token separati da spazi
    char *cmd = strtok(buffer, " "); //prende il comando 
    char *importo_str = strtok(NULL, " "); //prende l'importo
    char *causale = strtok(NULL, "\n");  // prendo tutto il resto come causale
    if (!cmd || !importo_str || !causale) {
            //mando un messaggio di errore al client
            send(client_fd, "Errore: comando incompleto\n", 27, 0);
            return;
        }
    if (strcmp(cmd, "ADD") != 0 &&
            strcmp(cmd, "DELETE") != 0 &&
            strcmp(cmd, "UPDATE") != 0 &&
            strcmp(cmd, "LIST") != 0) {
            send(client_fd, "Errore: comando non riconosciuto\n", 34, 0);
            return;
        }

    if(strcmp(cmd, "ADD")==0){
        gestisciADD(buffer,client_fd);
    }else if(strcmp(cmd, "DELETE")==0){
        gestisciDELETE(buffer,client_fd); // DELETE id
    }else if(strcmp(cmd, "UPDATE")==0){
        gestisciUPDATE(buffer,client_fd);
    }else{
        LIST(client_fd);
    }
    return;
}
