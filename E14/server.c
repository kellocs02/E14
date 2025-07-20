// server_multithread.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          // close()
#include <arpa/inet.h>       // sockaddr_in, inet_ntoa
#include <pthread.h>
#include "programma.h"         

#define PORT 12345
#define BUFFER_SIZE 1024

//inizializziamo tutti i movimenti mettendo 0 nel campo valido
//questo ci servirà quando faremo le operazioni sulla struttura dati condivisa
Movimento movimenti[MAX_MOVIMENTI];

void inizializza_movimenti() {
    for (int i = 0; i < MAX_MOVIMENTI; i++) {
        movimenti[i].valido = 0;  // 0 = slot libero
    }
}

// Funzione che gestisce ogni client, eseguita in un thread separato
void* gestisci_client(void* arg) {
    int client_fd = *((int*)arg);  // Prendo il socket passato come argomento
    free(arg);                     // Libero la memoria allocata nel main
    char buffer[BUFFER_SIZE];

    while (1) {
        //i dati ricevuti vengono messi nel buffer
        ssize_t bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);  // la variabile byte contiene il numero di byte ricevuto
        if (bytes <= 0) {  // Se errore o connessione chiusa dal client, esco dal loop
            break;
        }
        //la funzione recv non mette mai il carattere terminatore quindi lo dobbiamo mettere
        buffer[bytes] = '\0';  

        printf(" Messaggio ricevuto: %s\n", buffer);
        AnalisiStringa(buffer,client_fd);
        // Risposta semplice
        const char* risposta = "Messaggio ricevuto dal server\n";
        send(client_fd, risposta, strlen(risposta), 0);
    }

    close(client_fd);  // Chiudo la connessione con il client
    printf("🔴 Connessione con client chiusa.\n");
    return NULL;
}

int main() {
    inizializza_movimenti();
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 1. Creo il socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Errore creazione socket");
        exit(EXIT_FAILURE);
    }

    // 2. Imposto l'indirizzo e la porta del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da tutte le interfacce
    server_addr.sin_port = htons(PORT);       // converto la porta in network byte order

    // 3. Bind: associo il socket all'indirizzo e porta
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Metto il socket in ascolto per connessioni in arrivo
    if (listen(server_fd, 10) == -1) {
        perror("Errore listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("🟢 Server in ascolto sulla porta %d...\n", PORT);

    // 5. Loop infinito per accettare più client
    while (1) {
        int* client_fd = malloc(sizeof(int));  // Alloco spazio per il socket client (da passare al thread)
        if (!client_fd) {
            perror("malloc fallita");
            continue;  // salto questa iterazione
        }

        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (*client_fd == -1) {
            perror("Errore accept");
            free(client_fd);
            continue;  // continuo a accettare altri client
        }

        printf(" Connessione accettata da %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 6. Creo un thread per gestire il client connesso
        pthread_t tid;
        if (pthread_create(&tid, NULL, gestisci_client, client_fd) != 0) {
            perror("Errore creazione thread");
            close(*client_fd);
            free(client_fd);
            continue;
        }

        // 7. Stacco il thread per liberare automaticamente risorse al termine
        pthread_detach(tid);
    }

    // 8. Chiudo il socket server (in teoria non ci arrivo mai)
    close(server_fd);
    return 0;
}
