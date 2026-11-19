#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    struct sockaddr_in local;
    int local_len = sizeof(local);
    char buffer[BUFFER_SIZE];
    char ip[INET_ADDRSTRLEN];

    int n1, n2, n3;
    int risultato;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        printf("Errore nella creazione del socket.\n");
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    printf("Inserisci l'indirizzo IP del server: ");
    scanf("%s", ip);

    server.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Errore nella connessione al server.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    getsockname(sock, (struct sockaddr *)&local, &local_len);

    printf("Connesso dal client con IP: %s\n", inet_ntoa(local.sin_addr));

    recv(sock, buffer, BUFFER_SIZE - 1, 0);
    buffer[BUFFER_SIZE - 1] = '\0';
    printf("Server: %s", buffer);

    while (1) {
        printf("\nInserisci tre numeri interi: ");
        scanf("%d %d %d", &n1, &n2, &n3);

        sprintf(buffer, "%d %d %d\n", n1, n2, n3);
        send(sock, buffer, strlen(buffer), 0);

        int ricevuti = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (ricevuti <= 0) {
            printf("Connessione terminata.\n");
            break;
        }

        buffer[ricevuti] = '\0';

        if (strncmp(buffer, "QUIT", 4) == 0) {
            printf("Il risultato e' 100. Connessione terminata.\n");
            break;
        }

        printf("Risultato ricevuto dal server: %s", buffer);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
