#include "socket.hpp"
#include <pthread.h>
#include <cstring>
#include <string>

char nombreDeUsuario[50];

struct paquete{
	int tipo; // tipo de estructura (autenticacion, mensajes)
	char usuario[50];
	char pass[50];
	char mensaje[256];
	char destinatario[50];
	int numUsuario;
};

char miUsuario[50];

mySocket :: mySocket(char* puerto, char* IP){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->IP = IP;
	cout << "IP: " << this->IP << endl;
	//this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//if (sockfd < 0)
	//	cout << "Error en la apertura del socket" << endl;
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

bool mySocket::autenticar(){
	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;
	int n;

	//MANDO USUARIO Y CONTRASEÑA
	paqueteAEnviar.tipo = 1; //ASUMO TIPO 1 A LA AUTENTICACION
	cout << "Escriba su usuario: " << endl;
	cin.getline(paqueteAEnviar.usuario, 50); /*50 para probar, cambiar*/
	strcpy(nombreDeUsuario,paqueteAEnviar.usuario);
	strcpy(miUsuario,nombreDeUsuario);
	cout << "Escriba su contraseña: " << endl;
	cin.getline(paqueteAEnviar.pass, 50);

	memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));

	n = write(sockfd,buffer, sizeof(struct paquete));
	if (n < 0) cout << "Error en la escritura" << endl;

	//LEO RESPUESTA
	bzero(buffer,sizeof(struct paquete));

	n = read(sockfd,buffer,sizeof(struct paquete));
	if (n < 0) cout << "Error en la lectura" << endl;

	memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

	cout << "Mensaje del servidor: " << paqueteRecibido.mensaje << endl;

	return paqueteRecibido.tipo; //lo uso como bool VER si conviene agregar el bool
}

bool mySocket::conectar(){
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;	
	if (connect(this->sockfd,(struct sockaddr *) &this->serv_addr,sizeof(this->serv_addr)) < 0){
		cout << "Error intentado conectar" << endl;
		exit(0);
		return false;
	}
	return autenticar();
}

void mySocket::desconectar(){
	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;

	//ENVIO MENSAJE
	strcpy(paqueteAEnviar.usuario,nombreDeUsuario); 
	paqueteAEnviar.tipo = 3;

	memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));

	int n = write(sockfd,buffer, sizeof(struct paquete));
	if (n < 0) cout << "Error en la escritura" << endl;

	//LEO RESPUESTA
	bzero(buffer,sizeof(struct paquete));

	n = read(sockfd,buffer,sizeof(struct paquete));
	if (n < 0) cout << "Error en la lectura" << endl;

	memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

	cout << "Mensaje del servidor: " << paqueteRecibido.mensaje << endl;
}

int mySocket::cantidadUsuariosDisponibles(){
	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;

	//SOLICITO CANTIDAD DE USUARIOS
	paqueteAEnviar.tipo = 5; //ASUMO TIPO 5 A PEDIDO DE CANTIDAD DE USUARIOS
	memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));

	int n = write(sockfd,buffer, sizeof(struct paquete));
	if (n < 0) cout << "Error en la escritura" << endl;

	//LEO RESPUESTA
	bzero(buffer,sizeof(struct paquete));

	n = read(sockfd,buffer,sizeof(struct paquete));
	if (n < 0) cout << "Error en la lectura" << endl;

	memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

	return paqueteRecibido.tipo;
}

void mySocket::cargarUsuariosDisponibles(vector<string> *usuariosDisponibles){
	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;
	int cantUsuariosDisponibles = cantidadUsuariosDisponibles();

	for(int i = 0; i < cantUsuariosDisponibles; i++){
		//SOLICITO USUARIOS
		paqueteAEnviar.tipo = 4; //ASUMO TIPO 4 A PEDIDO DE USUARIOS
		paqueteAEnviar.numUsuario = i; //*
		bzero(buffer,sizeof(struct paquete));
		memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));
		int n = write(sockfd,buffer, sizeof(struct paquete));
		if (n < 0) cout << "Error en la escritura" << endl;

		//LEO USUARIOS
		bzero(buffer,sizeof(struct paquete));
		n = read(sockfd,buffer,sizeof(struct paquete));
		if (n < 0) cout << "Error en la lectura" << endl;
		memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

		usuariosDisponibles->push_back(paqueteRecibido.usuario);
	}
}

void mySocket::mostrarUsuariosDisponibles(vector<string> usuariosDisponibles){
	//IMPRIMIR VECTOR
	int i = 1;
	for(vector<string>::iterator it = usuariosDisponibles.begin(); it != usuariosDisponibles.end(); ++it) {
		cout << i << ") " << *it << endl;
		i ++;
	}
}

void mySocket::enviarMensaje(){
	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;

	vector<string> usuariosDisponibles;

	int numUsuario;
		cout << "A continuacion se presenta el listado de usuarios existenes" << endl;
		cargarUsuariosDisponibles(&usuariosDisponibles);
	do{
		cout << "Ingrese el numero de usuario al que desea enviar el mensaje" << endl;
		mostrarUsuariosDisponibles(usuariosDisponibles);
		cin >> numUsuario;
		cin.get();
	} while ((numUsuario < 1) || (numUsuario > usuariosDisponibles.size()));

	//ENVIO MENSAJE
	cout << "Escriba el mensaje: " << endl;
	cin.getline(paqueteAEnviar.mensaje, 50);

	paqueteAEnviar.tipo = 2; //ASUMO TIPO 2 A LOS MSJS
	strcpy(paqueteAEnviar.destinatario,usuariosDisponibles[numUsuario-1].c_str());
	strcpy(paqueteAEnviar.usuario,miUsuario);
	memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));

	int n = write(sockfd,buffer, sizeof(struct paquete));
	if (n < 0) cout << "Error en la escritura" << endl;

	//LEO RESPUESTA
	bzero(buffer,sizeof(struct paquete));

	n = read(sockfd,buffer,sizeof(struct paquete));
	if (n < 0) cout << "Error en la lectura" << endl;

	memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

	cout << "Respuesta del servidor: " << paqueteRecibido.mensaje << endl;

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

void mySocket::enviarMensaje(int destinatario, char* mensaje, int tamanioMensaje){
	/*int bytesEnviados = 0;
	bool errorSocket = false;
	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = write(sockfd, mensaje, tamanioMensaje - bytesEnviados);
		if(n < 0){
			errorSocket = true;
		}
		bytesEnviados += n;
	}*/

	unsigned char buffer[sizeof(struct paquete)];
	struct paquete paqueteAEnviar;
	struct paquete paqueteRecibido;
	
	vector<string> usuariosDisponibles;

	cargarUsuariosDisponibles(&usuariosDisponibles);

	//ENVIO MENSAJE
	paqueteAEnviar.tipo = 2;
	strcpy(paqueteAEnviar.usuario,miUsuario);
	strcpy(paqueteAEnviar.destinatario,usuariosDisponibles[destinatario].c_str());
	strcpy(paqueteAEnviar.mensaje, mensaje);

	memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));

	int n = write(sockfd,buffer, sizeof(struct paquete));
	if (n < 0) cout << "Error en la escritura" << endl;

	//LEO RESPUESTA
	bzero(buffer,sizeof(struct paquete));

	n = read(sockfd,buffer,sizeof(struct paquete));
	if (n < 0) cout << "Error en la lectura" << endl;

	memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

	cout << "Respuesta del servidor: " << paqueteRecibido.mensaje << endl;

}

void mySocket::cerrar(){
	close(this->sockfd);
}

mySocket::~mySocket(){}
