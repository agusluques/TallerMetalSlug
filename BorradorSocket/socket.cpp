#include "socket.h"
#include <pthread.h>

mySocket :: mySocket(char* puerto, char* IP){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->IP = IP;
	cout << "IP: " << this->IP << endl;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;
	this->server = gethostbyname(IP);
	if (this->server == NULL) {
		cout << "Error: No existe el host indicado" << endl;
		exit(0);
	}
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	bcopy((char *)this->server->h_addr, (char *)&this->serv_addr.sin_addr.s_addr, this->server->h_length);
	this->serv_addr.sin_port = htons(this->puerto);
}

void mySocket::conectar(){
    if (connect(this->sockfd,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)) < 0){
    	cout << "Error intentado conectar" << endl;
    	exit(0);
    }
}

void mySocket::enviarMensaje(){
	int opc;
	do{
		cout << "A continuacion se presenta el listado de usuarios existenes" << endl;
		cout << "Ingrese el numero de usuario al que desea enviar el mensaje" << endl;
		//Faltaria generar la lista de usuarios
		cout << "1) Franco" << endl;
		cout << "2) Agustin" << endl;
		cout << "3) Pablo" << endl;
		cout << "4) Matias" << endl;
		cin >> opc;
		cin.get();
	} while ((opc < 1) || (opc > 4));

	/*string mensaje ="";
	cout << "Escriba el mensaje: " << endl;
	getline(cin, mensaje);

	cout << "El mensaje sera enviado al usuario " << opc << endl;
	cout << "Mensaje: " << mensaje << endl;

	int tamanio = mensaje.length();
	int acumulador = 0;
	int bytesEnviados = 0;
	bool errorSocket = false;*/

	char buffer[256];
	bzero(buffer,256);
	cout << "Escriba el mensaje: " << endl;
	cin.getline(buffer, 256);
	//fgets(buffer,255,stdin);
	int n;

	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0)
		cout << "Error en la escritura" << endl;
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0)
		cout << "Error en la lectura" << endl;
	printf("%s\n",buffer);

	//close(sockfd);

	/*
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesEnviados = write(this->sockfd, &mensaje, tamanio - acumulador);
		if(bytesEnviados == (-1)){
			cout << "Error en el envio de datos" << endl;
			errorSocket = true;
		} else {
			acumulador += bytesEnviados;
		}
	}*/
}

void mySocket::cerrar(){
	close(this->sockfd);
}

mySocket::~mySocket(){}
