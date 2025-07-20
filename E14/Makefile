# Nomi degli eseguibili
TARGET_SERVER = server
TARGET_CLIENT = client

# Compilatore
CC = gcc

# Opzioni di compilazione
CFLAGS = -Wall -pthread

# File sorgenti
SERVER_SRC = server.c gestioneRichieste.c movimenti.c
CLIENT_SRC = client.c

# Header comune
HEADERS = programma.h

# Compilazione generale
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# Regola per il server
$(TARGET_SERVER): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC)

# Regola per il client
$(TARGET_CLIENT): $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRC)

# Pulizia file oggetto ed eseguibili
clean:
	rm -f *.o $(TARGET_SERVER) $(TARGET_CLIENT)
