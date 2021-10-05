#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// VINDOS
#include <WinSock2.h>

// UNIX
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <sys/types.h>

#define PORT 5000
#define MAX_DATA 100

int main()
{
    int socketCliente;                      // descritor de comunicação do cliente
    struct sockaddr_in enderecoServidor;    // Informações do servidor
    struct hostent *he;                     // Struct auxiliar de informações do host (Servidor)
    
    int estado;
    int numBytes;
    char buffer[MAX_DATA];

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

    if ( socketCliente == -1 ){
        perror("Erro de inicializacao do Cliente\n");
        exit(EXIT_FAILURE);
    }

    he = gethostbyname("192.168.0.30");

    if ( he < 0) {
        perror("Erro ao iniciar endpoint\n");
        exit(EXIT_FAILURE);
    }

    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = PORT;
    enderecoServidor.sin_addr = *((struct in_addr *) he->h_addr);

    // Ligar o cliente ao servidor
    estado = connect(socketCliente, 
    (struct sockaddr *) &enderecoServidor, 
    sizeof(struct sockaddr));

    if ( estado == -1){
        perror("Erro ao conectar no server\n");
        exit(EXIT_FAILURE);
    }

    numBytes = recv(socketCliente,buffer,MAX_DATA,0);

    if ( numBytes < 0){ 
        perror("Erro ao receber informacoes\n");
        exit(EXIT_FAILURE);
    }

    buffer[numBytes] = '\0';
    printf("Mensagem Recebida = %s", buffer);
    closesocket(socketCliente);

    return 0;
}	