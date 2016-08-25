/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    //VARIABLES
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr,"ERROR, escriba el puerto\n");
        exit(1);
    }

    //SOCKET
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR, no se pudo abrir el servidor");
    else printf("Servidor Abierto\n");   

    //Inicializo serv_addr en 0
    bzero((char *) &serv_addr, sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = INADDR_ANY;

    portno = atoi(argv[1]);
    serv_addr.sin_port = htons(portno);

    //BIND
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");

    //LISTEN
    listen(sockfd,5);
    printf("Escuchando cliente...\n");

    //ACCEPT
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    else {
        printf("Conectado!\n");
        printf("Leyendo mensaje...\n");
    }

    //READ
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR, no se pudo leer el mensaje");
    
    printf("Mensaje: %s\n",buffer);
    
    //WRITE
    char msg[256]="Mensaje recibido por el servidor\n";
    n = write(newsockfd,msg,strlen(msg));
    if (n < 0) error("ERROR writing to socket");
    
    close(newsockfd);
    close(sockfd);
    return 0; 
}