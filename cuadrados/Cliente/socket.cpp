#include "socket.hpp"
#include <pthread.h>
#include "SDL2/SDL.h"

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

bool mySocket::autenticar(string nombre){
	char codigo = '1';
	enviarMensaje(&codigo, sizeof(char));

	int tamanioNombre = nombre.length()+1;
	char* cstr = new char [tamanioNombre];
	strcpy (cstr, nombre.c_str());

	enviarMensaje(&tamanioNombre, sizeof(int));
	enviarMensaje(cstr, tamanioNombre*(sizeof(char)));

	delete[] cstr;
	nombre.clear();

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

	//ingreso nuevo usuario
	string nombre;
	cout << "Ingrese su nombre: " << endl;
	getline(cin, nombre);

	this->conectado = autenticar(nombre);
}

int mySocket::getFD(){
	return this->sockfd;
}

bool mySocket::enviarMensaje(void* mensaje, int tamanioMensaje){
	//int bytesEnviados = 0;
	bool errorSocket = false;
	//while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
	//cout << "entro antes del" << endl;
	//int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
	int n = write(sockfd, mensaje, tamanioMensaje);
	//cout << "n: " << n << endl;
	if(n < 0){
		errorSocket = true;
		//cout << "Entro a TRUE" << endl;
	}
	//bytesEnviados += n;
	//}
	return errorSocket;
}

void mySocket::recibirMensaje(){
	char codigo;
	codigo = '5';
	enviarMensaje(&codigo, sizeof(char));

	int tam = 1;

	while(tam != 0){
		recibirMensaje(&tam, sizeof(int));
		if (tam != 0){
			char mensaje[tam];
			int idObjeto;

			cout << "Tamanio del mensaje: " << tam << endl;
			recibirMensaje(&mensaje, sizeof(char)*tam);
			cout << "Mensaje: " << mensaje << endl;
			recibirMensaje(&idObjeto, sizeof(int));
			cout << "id objeto: " << idObjeto << endl;

			if (strcmp(mensaje,"U") == 0){
				grafica.mensajeUp(idObjeto);
			}else
				if (strcmp(mensaje,"D") == 0) {
					grafica.mensajeDown(idObjeto);
				}else
					if (strcmp(mensaje,"L") == 0){
						grafica.mensajeLeft(idObjeto);
					}else
						if (strcmp(mensaje,"R") == 0){
							grafica.mensajeRight(idObjeto);
						}
		}
	}
}

void mySocket::recibirMensaje(void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;
	//bool errorSocket = false;
	//while ((acumulador < tamanio) && (errorSocket == false)){
	bytesRecibidos = read(this->sockfd, buffer, tamanio);
	if (bytesRecibidos == (-1)){
		cout << "Error al recibir datos SOCKET" << endl;
		//		errorSocket = true;
		//	} else {
		//		acumulador += bytesRecibidos;
		//	}
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

void mySocket::iniciarGrafica(){
	grafica.init();

	bool quit = false;
	SDL_Event event;
	const Uint8* teclas;

	//pido cantidad de objetos a dibujar
	char codigo;
	codigo = '6';
	enviarMensaje(&codigo, sizeof(char));
	int cantObjetos;
	recibirMensaje(&cantObjetos, sizeof(int));

	cout << "Cantidad de Objetos dibujables: " << cantObjetos << endl;

	//SOLICITO OBJETOS
	for(int i = 1; i <= cantObjetos; i++){
		char codigo;
		codigo = '9';
		enviarMensaje(&codigo, sizeof(char));

		int numeroUsuario = i;
		enviarMensaje(&numeroUsuario, sizeof(int));

		//idObjeto
		int idObjeto;
		recibirMensaje(&idObjeto, sizeof(int));
		cout << idObjeto << endl;

		//spriteId
		int tamSpriteId;
		recibirMensaje(&tamSpriteId, sizeof(int));
		char spriteId[tamSpriteId];
		recibirMensaje(&spriteId, sizeof(char)*tamSpriteId);
		cout << spriteId << endl;

		//posicion x
		int posX;
		recibirMensaje(&posX, sizeof(int));
		cout << posX << endl;

		//posicion y
		int posY;
		recibirMensaje(&posY, sizeof(int));
		cout << posY << endl;

		Dibujable nuevo;
		nuevo.setId(idObjeto);
		nuevo.setX(posX);
		nuevo.setY(posY);

		if (strcmp(spriteId,"fondo") == 0){
			nuevo.setSprite("img/fondo.bmp");
		}else
			nuevo.setSprite("img/cuadrado.bmp");

		grafica.nuevoDibujable(nuevo);
	}

	while( !quit ) {

		//recibo si hay cambios
		recibirMensaje();

		//MOSTRAR VENTANA
		grafica.mostrarDibujables();

		//SALIR CON ESC O CERRAR VENTANA
		while( SDL_PollEvent(&event) != 0 ) {
			if( event.type == SDL_QUIT ) {
				grafica.close();
				quit = true;
			}

			/*//variar velocidad si es un toqe o mantengo apretado
			//If a key was pressed
			if( event.type == SDL_KEYDOWN && event.key.repeat == 0 ) {
				//Adjust the velocity
				switch( event.key.keysym.sym ) {
				case SDLK_UP:{
					//hardcodeo
					strcpy(&codigo,"U");
					enviarMensaje(&codigo, sizeof(char));
					break;
				}
				case SDLK_DOWN:{
					//hardcodeo
					strcpy(&codigo,"D");
					enviarMensaje(&codigo, sizeof(char));
					break;
				}
				case SDLK_LEFT:{
					//hardcodeo
					strcpy(&codigo,"L");
					enviarMensaje(&codigo, sizeof(char));
					break;
				}
				case SDLK_RIGHT:{
					//hardcodeo
					strcpy(&codigo,"R");
					enviarMensaje(&codigo, sizeof(char));
					break;
				}
				}
			}
			//If a key was released
			else if( event.type == SDL_KEYUP && event.key.repeat == 0 ) {
				//Adjust the velocity
				switch( event.key.keysym.sym ) {
				case SDLK_UP:{
					//hardcodeo
					strcpy(&codigo,"U");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeUp();
					break;
				}
				case SDLK_DOWN:{
					//hardcodeo
					strcpy(&codigo,"D");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeUp();
					break;
				}
				case SDLK_LEFT:{
					//hardcodeo
					strcpy(&codigo,"L");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeUp();
					break;
				}
				case SDLK_RIGHT:{
					//hardcodeo
					strcpy(&codigo,"R");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeUp();
					break;
				}
				}
			}*/

			if( event.type == SDL_KEYDOWN ) {
				char codigo;

				switch( event.key.keysym.sym ) {
				case SDLK_UP: {
					strcpy(&codigo,"U");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeUp();
					break;
				}
				case SDLK_DOWN: {
					//hardcodeo
					strcpy(&codigo,"D");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeDown();
					break;
				}
				case SDLK_LEFT:
					strcpy(&codigo,"L");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeLeft();
					break;

				case SDLK_RIGHT:
					strcpy(&codigo,"R");
					enviarMensaje(&codigo, sizeof(char));
					//grafica.mensajeRight();
					break;

				default:
					break;
				}
			}
		}


	}

	grafica.close();
}

mySocket::~mySocket(){}
