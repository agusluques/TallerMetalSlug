#include "socket.hpp"
#include <pthread.h>

mySocket::mySocket(char* puerto, char* IP){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->sockfd = 0;
	//this->address = str(IP);
	server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(this->puerto);
    this->conectado = false;
}

bool mySocket::conexion(){
	return (this->conectado);
}

void mySocket::conectar(){
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;
	if (connect(this->sockfd,(struct sockaddr *) &this->server,sizeof(this->server)) < 0){
    	cout << "Error intentado conectar" << endl;
    	exit(0);
    }
	this->conectado = true;
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

	bool terminado = false;
	//int tamAcumulado = 0;
	//int cantEnters = 0;
	string mensaje;
	int linea;
	cout << "Escriba su mensaje: " << endl;
	while(!terminado){
		getline(cin, mensaje);
		//cin.get();
		if(mensaje.length() != 0){
			linea ++;
			char* cstr = new char [mensaje.length()+1];
			strcpy (cstr, mensaje.c_str());
			enviarMensaje(opc, cstr, mensaje.length()+1);
			//tamAcumulado += (mensaje.length()+1);
			delete[] cstr;
			//cantEnters = 0;
			cout << endl;
			cout << "Linea " << linea << " enviada. Presione enter para continuar" << endl;
			//cin.get();
		} else {
			terminado = true;
		}
	}
	//terminado = false;

	/*
	string mensaje;
	cout << "Escriba el mensaje: " << endl;
	getline(cin, mensaje);
	char* cstr = new char [mensaje.length()+1];
	//char* cstr = new char [mensaje.length()];
	strcpy (cstr, mensaje.c_str());
	enviarMensaje(opc, cstr, mensaje.length()+1);
	delete[] cstr;*/
}

void mySocket::enviarMensaje(int destino, char* mensaje, int tamanioMensaje){

	char codigo;
	codigo = '4';
	enviarMensaje(&codigo, sizeof(char));
	enviarMensaje(&destino, sizeof(int));
	enviarMensaje(&tamanioMensaje, sizeof(int));
	enviarMensaje(mensaje, tamanioMensaje*(sizeof(char)));
}

bool mySocket::enviarMensaje(void* mensaje, int tamanioMensaje){
	int bytesEnviados = 0;
	bool errorSocket = false;
	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = write(sockfd, mensaje, tamanioMensaje - bytesEnviados);
		if(n < 0){
			errorSocket = true;
		}
		bytesEnviados += n;
	}
	return errorSocket;
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

void mySocket::desconectar(){
	this->conectado = false;
}

void mySocket::cerrar(){
	char codigo;
	codigo = '2';
	enviarMensaje(&codigo, sizeof(char));
	close(this->sockfd);
}

mySocket::~mySocket(){}
