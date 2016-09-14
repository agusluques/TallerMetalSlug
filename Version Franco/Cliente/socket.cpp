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

bool mySocket::autenticar(){
	char codigo = '1';
	char usrnum;
	int usuario;
	string linea;

	enviarMensaje(&codigo, sizeof(char));
	int tamLista;
	recibirMensaje(&tamLista, sizeof(int));
	do {
		cout << "Ingrese numero de usuario" << endl;
		cin >> usrnum;
		cin.get();
	}while ((usrnum< 48) || (usrnum>(48+tamLista)));
	usuario = atoi(&usrnum);
	enviarMensaje(&usuario, sizeof(int));

	cout << "Escriba su contraseña: " << endl;
	getline(cin, linea);
	char* cstr = new char [linea.length() + 1];
	strcpy (cstr, linea.c_str());

	int tamanioContrasenia = linea.length()+1;

	enviarMensaje(&tamanioContrasenia, sizeof(int));
	enviarMensaje(cstr, tamanioContrasenia*(sizeof(char)));

	delete[] cstr;
	linea.clear();

	//LEO RESPUESTA
	int tam, respuesta;

	recibirMensaje(&respuesta, sizeof(int));

	recibirMensaje(&tam, sizeof(int));

	char mensaje[tam];
	recibirMensaje(&mensaje, sizeof(char)*tam);
	cout << "Mensaje del servidor: " << mensaje << endl;

	return respuesta;
}

void mySocket::conectar(){
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;
	if (connect(this->sockfd,(struct sockaddr *) &this->server,sizeof(this->server)) < 0){
    	cout << "Error intentado conectar" << endl;
    	exit(0);
    }

	pedirUsuarios();

	this->conectado = autenticar();
}

void mySocket::enviarMensaje(){
	int opc;
	do{
		cout << "A continuacion se presenta el listado de usuarios existenes" << endl;
		cout << "Ingrese el numero de usuario al que desea enviar el mensaje" << endl;

		pedirUsuarios();
		cout << cantidadUsuariosDisponibles()+1 << ") Todos" << endl;

		cin >> opc;
		cin.get();
	} while ((opc < 1) || (opc > cantidadUsuariosDisponibles()+1));

	/*bool terminado = false;
	//int tamAcumulado = 0;
	//int cantEnters = 0;
	string mensaje;
	int linea = 0;
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
	}*/
	int tamAcumulado = 0;
	int enters = 0;
	bool terminado = false;
	string mensaje;
	string mensajeTotal = "";
	cout << "Escriba su mensaje: (Ingrese /enviar para enviar) " << endl;
	while(!terminado){
		getline(cin, mensaje);
		if(mensaje.compare("/enviar") == 0){
			terminado = true;
		} else {
			mensajeTotal = mensajeTotal + mensaje + '\n';
			tamAcumulado += (mensaje.length()+1);
		}
	}
	cout << "Mensaje total: " << mensajeTotal << endl;
	char* cstr = new char [mensajeTotal.length()+1];
	strcpy (cstr, mensajeTotal.c_str());
	enviarMensaje(opc, cstr, mensajeTotal.length()+1);
	delete[] cstr;
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
		//cout << "entro antes del" << endl;
		int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
		//cout << "n: " << n << endl;		
		if(n <= 0){
			errorSocket = true;
			//cout << "Entro a TRUE" << endl;
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

int mySocket::cantidadUsuariosDisponibles(){
	char codigo;
	codigo = '8';
	enviarMensaje(&codigo, sizeof(char));

	int cantidad;
	recibirMensaje(&cantidad, sizeof(int));

	return cantidad;
}

void mySocket::pedirUsuarios(){
	vector<string> usuariosDisponibles;

	int cantUsuariosDisponibles = cantidadUsuariosDisponibles();

	//cout << cantUsuariosDisponibles << endl;

	for(int i = 1; i <= cantUsuariosDisponibles; i++){
		//SOLICITO USUARIOS
		char codigo;
		codigo = '7';
		enviarMensaje(&codigo, sizeof(char));

		int numeroUsuario = i;
		enviarMensaje(&numeroUsuario, sizeof(int));

		//LEO USUARIOS
		int tamMensaje;
		recibirMensaje(&tamMensaje, sizeof(int));

		char mensaje[tamMensaje];
		recibirMensaje(&mensaje, sizeof(char)*tamMensaje);

		usuariosDisponibles.push_back(mensaje);
	}

	//IMPRIMIR VECTOR
	int i = 1;
	for(vector<string>::iterator it = usuariosDisponibles.begin(); it != usuariosDisponibles.end(); ++it) {
		cout << i << ") " << *it << endl;
		i++;
	}
}

void mySocket::cerrar(){
	char codigo;
	codigo = '2';
	enviarMensaje(&codigo, sizeof(char));
	close(this->sockfd);
}

mySocket::~mySocket(){}
