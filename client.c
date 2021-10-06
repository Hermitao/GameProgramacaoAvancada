#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <windows.h>
#include <conio.h>

// VINDOS
#include <WinSock2.h>

// UNIX
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <sys/types.h>

#define PORT 5000
#define MAX_DATA 100000

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

    he = gethostbyname("127.0.0.1");

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

	//int socketServidor;
	//int tamanho = 0;
	//tamanho = sizeof(struct sockaddr);
	//socketServidor = accept(socketCliente, (struct sockaddr *) &enderecoServidor, &tamanho);
    while (1)
    {
        char comando = _getch();
        if (comando == 'q')
        {
            exit(0);
        }
        char comandoString[2];
        comandoString[0] = comando;
        comandoString[1] = '\0';
        send(socketCliente, comandoString, 64, 0);
    }

    closesocket(socketCliente);

    return 0;
}	