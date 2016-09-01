/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <pthread.h>
#include <iostream>
#define TAM_MAX 8192
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
  char buffer[TAM_MAX];
    int n;
    int newsockfd = (long)arg;

    while (1){
        //READ
        bzero(buffer,TAM_MAX);
        n = read(newsockfd,buffer,TAM_MAX-1);
        if (n < 0) error("ERROR, no se pudo leer el mensaje");
        cout << "Mensaje:" << buffer << endl<<'\n';

        //WRITE
        char msg[256]="Mensaje recibido por el servidor\n";
        n = write(newsockfd,msg,strlen(msg));
        if (n < 0) error("ERROR writing to socket");
     }   

    close(newsockfd);
    return NULL;
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, pid;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
      fprintf(stderr,"ERROR, no port provided\n");
      exit(1);
  }

  //SOCKET
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  else printf("Servidor Abierto\n");

  //Inicializo serv_addr en 0
  bzero((char *) &serv_addr, sizeof(serv_addr));

  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
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
  while (1) {
    pthread_t thread;//CREO UN THREAD

    newsockfd = accept(sockfd,
          (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    else {
            pthread_create(&thread, NULL, atender_cliente, (void*)(long)newsockfd);
            printf("Conectado!\n");
            printf("Leyendo mensaje...\n");
          }

  } /* end of while */
  close(sockfd);
  return 0; /* we never get here */
}