#include "socket.hpp"
#include <pthread.h>

mySocket :: mySocket(char* puerto, char* IP){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->sockfd = 0;
	//this->address = str(IP);
	server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(this->puerto);
}

void mySocket::conectar(){
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;
	if (connect(this->sockfd,(struct sockaddr *) &this->server,sizeof(this->server)) < 0){
    	cout << "Error intentado conectar" << endl;
    	exit(0);
    }
	char codigo;
	codigo = '1';
	enviarMensaje(&codigo, sizeof(char));
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

	/*char buffer[256];
	bzero(buffer,256);
	cout << "Escriba el mensaje: " << endl;
	cin.getline(buffer, 256);*/

	string mensaje;
	cout << "Escriba el mensaje: " << endl;
	getline(cin, mensaje);
	//char* cstr = new char [mensaje.length()+1];
	char* cstr = new char [mensaje.length()];
	strcpy (cstr, mensaje.c_str());
	enviarMensaje(opc, cstr, mensaje.length());
	delete[] cstr;
}

void mySocket::enviarMensaje(string mensaje){

	char* cstr = new char [mensaje.length()+1];
	strcpy (cstr, mensaje.c_str());


	int n = write(sockfd,cstr,mensaje.length());
	if (n < 0)
		cout << "Error en la escritura" << endl;
	delete[] cstr;
}

void mySocket::enviarMensaje(int usuario, char* mensaje, int tamanioMensaje){

	char codigo;
	codigo = '4';
	enviarMensaje(&codigo, sizeof(char));
	enviarMensaje(&usuario, sizeof(int));
	enviarMensaje(&tamanioMensaje, sizeof(int));
	enviarMensaje(mensaje, tamanioMensaje*(sizeof(char)));
}

void mySocket::enviarMensaje(void* mensaje, int tamanioMensaje){
	int bytesEnviados = 0;
	bool errorSocket = false;
	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = write(sockfd, mensaje, tamanioMensaje - bytesEnviados);
		if(n < 0){
			errorSocket = true;
		}
		bytesEnviados += n;
	}
}

void mySocket::recibirMensaje(){
	char codigo;
	codigo = '5';
	enviarMensaje(&codigo, sizeof(char));
}

void mySocket::recibirMensaje(void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;
	bool errorSocket = false;
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesRecibidos = read(this->sockfd, buffer, tamanio);
		if (bytesRecibidos == (-1)){
			cout << "Error al recibir datos SOCKET" << endl;
			errorSocket = true;
		} else {
			acumulador += bytesRecibidos;
		}
	}
}

void mySocket::cerrar(){
	char codigo;
	codigo = '2';
	enviarMensaje(&codigo, sizeof(char));
	close(this->sockfd);
}

mySocket::~mySocket(){}
