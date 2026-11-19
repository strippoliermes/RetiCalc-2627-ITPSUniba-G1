#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Errore nella creazione del socket.\n");
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Errore nel bind.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Errore nella listen.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);

        if (client_socket == INVALID_SOCKET) {
            printf("Errore nell'accettazione della connessione.\n");
            continue;
        }

        printf("Client connesso: %s\n", inet_ntoa(client.sin_addr));

        send(client_socket, "connessione avvenuta\n", 21, 0);

        while (1) {
            int ricevuti = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

            if (ricevuti <= 0) {
                printf("Client disconnesso.\n");
                break;
            }

            buffer[ricevuti] = '\0';

            int n1, n2, n3;

            if (sscanf(buffer, "%d %d %d", &n1, &n2, &n3) != 3) {
                printf("Dati non validi ricevuti.\n");
                break;
            }

            int risultato = n1 + n2 - n3;

            printf("Ricevuti: %d, %d, %d\n", n1, n2, n3);

            if (risultato == 100) {
                send(client_socket, "QUIT\n", 5, 0);
                printf("Risultato 100: chiusura connessione.\n");
                break;
            } else {
                sprintf(buffer, "%d\n", risultato);
                send(client_socket, buffer, strlen(buffer), 0);
            }
        }

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}
