#include "socket.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    if (argc < 3) {
    	cout << "Falta proveer datos al programa" << endl;
    	cout << "Modo de uso: ./Cliente hostname/IP puerto" << endl;
       exit(0);
    }

	mySocket mySocket(argv[2], argv[1]);

	int opc = 0;
	do{
		cout << "##### Menu #####" << endl;
		cout << "1) Conectar" << endl;
		cout << "2) Desconectar" << endl;
		cout << "3) Salir" << endl;
		cout << "4) Enviar" << endl;
		cout << "5) Recibir" << endl;
		cout << "6) Lorem Ipsum" << endl;
		cin >> opc;
		switch(opc){
				case 1:
					cout << "Conectando al servidor..." << endl;
				    mySocket.conectar();
				    cout << "Se ha conectado correctamente con el servidor" << endl;
				    break;

				case 2:
					cout << "Desconectando del serrvidor..." << endl;
					//Implementar el desconectar
					break;

				case 3:
					cout << "Cerrando el programa..." << endl;
					return 0;
					break;

				case 4:
					cout << "Enviando mensajes..." << endl;
				    mySocket.enviarMensaje(); //corregir
				    break;

				case 5:
					cout << "Recibiendo Datos..." << endl;
					//Falta implementar
					break;

				case 6:
					cout<< "Lorem Ipsum..." << endl;
					//Falta implementar
					break;

				default:
					cout << "Opcion incorrecta. Intente de nuevo" << endl;
					break;
			}
	} while (opc != 3);



	/*
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    */



    /*
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    */



    /*
    if (connect(mySocket.sockfd,(struct sockaddr *) &mySocket.serv_addr,sizeof(mySocket.serv_addr)) < 0)
        error("ERROR connecting");
	*/



    /*printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);*/
    //return 0;
}
