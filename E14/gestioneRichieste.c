#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     //usata per la funzione send  
#include <pthread>
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

//funzione che prende il buffer dove il client invia il comando
//prende anche il fd del client
void AnalisiStringa(char *buffer, int client_fd) {
    //suddiviamo la stringa in token separati da spazi
    char *cmd = strtok(buffer, " "); //prende il comando 
    char *importo_str = strtok(NULL, " "); //prende l'importo
    char *causale = strtok(NULL, "\n");  // prendo tutto il resto come causale

    //controlliamo che i token esistono quindi che non sono null
    if (!cmd || !importo_str || !causale) {
        //mando un messaggio di errore al client
        send(client_fd, "Errore: comando incompleto\n", 27, 0);
        return;
    }

    //controlliamo che il comando sia uno dei 4 validi
    //strcmp ritorna 0 se le stringhe sono uguali
    //quindi entriamo nell'if solo se il comando non corrisponde ai nostri quattro
    if (strcmp(cmd, "ADD") != 0 &&
        strcmp(cmd, "DELETE") != 0 &&
        strcmp(cmd, "UPDATE") != 0 &&
        strcmp(cmd, "LIST") != 0) {
        send(client_fd, "Errore: comando non riconosciuto\n", 34, 0);
        return;
    }

    //controlla che l'importo sia valido
    if (!is_number(importo_str)) {
        send(client_fd, "Errore: importo non valido\n", 27, 0);
        return;
    }

    //controlliamo la casuale e se è valida impostiamo valid a 1
    //*valid_casuali[] è un array di puntatori a costanti char(è una lista di strignhe)
    /* 
    valid_causali[0] → "Bonifico"
    valid_causali[1] → "Prelievo"
    valid_causali[2] → "Deposito"
    */
    const char *valid_causali[] = {"Bonifico", "Prelievo", "Deposito"};
    int valid = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp(causale, valid_causali[i]) == 0) {
            valid = 1;
            break;
        }
    }

    if (!valid) {
        send(client_fd, "Errore: causale non valida\n", 27, 0);
        return;
    }

    // Se arrivi qui, il comando è valido: puoi chiamare la funzione specifica
    // Esempio: gestisci_add(importo_str, causale, client_fd);

    char msg[] = "Comando valido, procedo con l'operazione\n";
    send(client_fd, msg, strlen(msg), 0);

    //ora dobbiamo qui mettere la funzione corrispondente da eseguire in base al comando

    if(strcmp(cmd,"ADD")==0){
        ADD();
    }else if(strcmp(cmd,"DELETE")==0){
        DELETE();
    }else if(strcmp(cmd,"UPDATE")==0){
        UPDATE();
    }else{
        LIST();
    }
}



