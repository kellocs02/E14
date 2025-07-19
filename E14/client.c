// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // close()
#include <arpa/inet.h>     // sockaddr_in, inet_pton
#include <pthread>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Crea il socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    // 2. Configura l'indirizzo del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // 3. Converti l'indirizzo IP
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Indirizzo non valido");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 4. Connetti al server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nella connessione");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 5. Manda un messaggio
    const char* messaggio = "Ciao server, sono il client!";
    send(sockfd, messaggio, strlen(messaggio), 0);

    // 6. Ricevi la risposta
    ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("📨 Risposta dal server: %s\n", buffer);
    }

    // 7. Chiudi
    close(sockfd);
    return 0;
}
