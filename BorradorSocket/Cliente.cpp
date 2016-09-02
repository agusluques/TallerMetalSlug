#include "socket.hpp"

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
	bool conectado = false;
	int opc = 0;
	do{
		cout << "##### Menu #####" << endl;
		if (!conectado){
			cout << "1) Conectar" << endl;
		}
		else{
			cout << "1) Desconectar" << endl;
			cout << "2) Salir" << endl;
			cout << "3) Enviar" << endl;
			cout << "4) Recibir" << endl;
			cout << "5) Lorem Ipsum" << endl;
		}
		cin >> opc;
		cin.get();
		switch(opc){
				case 1:
					switch(conectado){
						case false:
							cout << "Conectando al servidor..." << endl;
						    mySocket.conectar();
						    cout << "Se ha conectado correctamente con el servidor" << endl;
						    conectado = true;
						    break;

						case true:
							cout << "Desconectando del serrvidor..." << endl;
							//Implementar el desconectar
							break;	
					}	
					break;					

				case 2:
					cout << "Cerrando el programa..." << endl;
					return 0;
					break;

				case 3:
					cout << "Enviando mensajes..." << endl;
				    mySocket.enviarMensaje(); //corregir
				    break;

				case 4:
					cout << "Recibiendo Datos..." << endl;
					//Falta implementar
					break;

				case 5:
					cout<< "Lorem Ipsum..." << endl;
					//Falta implementar
					break;

				default:
					cout << "Opcion incorrecta. Intente de nuevo" << endl;
					break;
			}
	} while (opc != 2);



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
