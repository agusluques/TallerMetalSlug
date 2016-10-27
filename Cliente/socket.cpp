#include "socket.hpp"
#include <pthread.h>
#include "SDL2/SDL.h"

bool avanzar;
char nombreCliente[50];

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

	recibirMensaje(&this->numeroCliente, sizeof(int));

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

	strcpy(nombreCliente,nombre.c_str());

	this->conectado = autenticar(nombre);
}

int mySocket::getFD(){
	return this->sockfd;
}

bool mySocket::enviarMensaje(void* mensaje, int tamanioMensaje){
	/*
	int bytesEnviados = 0;
	bool errorSocket = false;
	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		//cout << "entro antes del" << endl;
		int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
		//int n = write(sockfd, mensaje, tamanioMensaje);
		//cout << "n: " << n << endl;
		if(n < 0){
			errorSocket = true;
			//cout << "Entro a TRUE" << endl;
		}
		bytesEnviados += n;
	}
	return errorSocket;
	*/
	bool errorSocket = false;
	int n = write(sockfd, mensaje, tamanioMensaje);
	if(n < 0) errorSocket = true;

	char* tmp = (char*)mensaje;
	//int* tmp = (int*)mensaje;
	cout << "ENVIA: " << (*tmp) << endl;

	return errorSocket;
}

bool mySocket::recibirMensaje(void* buffer, int tamanio){
	/*int acumulador = 0, bytesRecibidos = 0;

	struct timeval tv;
	tv.tv_sec = 10;  // 10 Secs Timeout
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
	setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

	bool errorSocket = false;
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesRecibidos = read(this->sockfd, buffer, tamanio);
		if (bytesRecibidos < 0){
			cout << "Se cayo la conexion con el servidor" << endl;
			errorSocket = true;
		} else {
			acumulador += bytesRecibidos;
		}
	}
	return errorSocket;*/

	bool errorSocket = false;
	int n = read(this->sockfd, buffer, tamanio);
	if(n < 0) errorSocket = true;

	//char* tmp = (char*)mensaje;
	int* tmp = (int*)buffer;
	cout << "RECIBE: " << (*tmp) << endl;

	return errorSocket;
}

bool mySocket::recibirMensaje(){
	bool error = false;
	char codigo;
	codigo = '5';
	error = enviarMensaje(&codigo, sizeof(char));
	if(error){
		cout << "ENTRO A ERROR" << endl;
	}

	int corte = 1;
	error = recibirMensaje(&corte, sizeof(int));
	cout << "RECIBO!!!!!!!!!!!!!!!!!" << endl;

	if(error){
		cout << "ENTRO A ERROR 1" << endl;
		corte = 0;
		this->desconectar();
	}

	while(corte != 0){

		if(error){
			cout << "ENTRO A ERROR 1" << endl;
			corte = 0;
			this->desconectar();
			break;
		}

		int tipoMensaje;
		error = recibirMensaje(&tipoMensaje, sizeof(int));
		if(error){
			cout << "ENTRO A ERROR 2" << endl;
			corte = 0;
			break;
		}
		//cout << "Tipo de mensaje: " << tipoMensaje << endl;

		switch(tipoMensaje){
		case 1:{
			//recibo grafica jugadores
			int x,y, spx, spy, idObjeto;
			char flip;
			avanzar = false;

			error = recibirMensaje(&idObjeto, sizeof(int));
			//cout << "id objeto: " << idObjeto << endl;
			error = recibirMensaje(&x, sizeof(int));
			//cout << "X: " << x << endl;
			error = recibirMensaje(&y, sizeof(int));
			//cout << "Y: " << y << endl;
			error = recibirMensaje(&spx, sizeof(int));
			//cout << "SpriteX: " << spx << endl;
			error = recibirMensaje(&spy, sizeof(int));
			//cout << "SpriteY: " << spy << endl;
			error = recibirMensaje(&flip, sizeof(char));
			//cout << "Flip: " << flip << endl;
			error = recibirMensaje(&avanzar, sizeof(bool));
			//cout << "Puede avanzar: " << avanzar << endl;

			grafica.actualizar(idObjeto, x, y, spx, spy, avanzar, flip);

			//grafica.avanzarCamara(x); OTRA MANERA

			break;
		}
		case 2:
			//recibo fondo

			break;
		case 3:{
			//recibo mensaje
			int tam;

			error = recibirMensaje(&tam, sizeof(int));
			char mensaje[tam];
			error = recibirMensaje(&mensaje, sizeof(char)*tam);
			cout << "(*)Mensaje: " << mensaje << endl;

			break;
		}
		case 4:{
			cerrarGrafica();
			/*this->desconectar();
			this->cerrar();

			this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0)
				cout << "Error en la apertura del socket" << endl;
			if (connect(this->sockfd,(struct sockaddr *) &this->server,sizeof(this->server)) < 0){
				cout << "Error intentado conectar" << endl;
				exit(0);
			}

			this->conectado = autenticar(nombreCliente);

			iniciarGrafica();*/
			//cout << "CERRE OK" << endl;
			break;
		}
		case 5:
			error = recibirMensaje(&corte, sizeof(int));
			//pido msj antes del corte xq sino este me empieza a mandar msjs y se mezclan
			iniciarGrafica();
			//cout << "INICIE OK" << endl;
			break;

		}

		error = recibirMensaje(&corte, sizeof(int));
	}
	return error;
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

void mySocket::cerrarGrafica(){
	grafica.close();
}

void mySocket::cargarDibujables(){
	//pido cantidad de objetos a dibujar
	char codigo;
	codigo = '6';
	enviarMensaje(&codigo, sizeof(char));
	int cantObjetos;
	recibirMensaje(&cantObjetos, sizeof(int));

	//cout << "Cantidad de Objetos dibujables: " << cantObjetos << endl;

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
		//cout << "idobjeto: " << idObjeto << endl;

		//spriteId
		int tamSpriteId;
		recibirMensaje(&tamSpriteId, sizeof(int));
		char spriteId[tamSpriteId];
		recibirMensaje(&spriteId, sizeof(char)*tamSpriteId);
		//cout << "spriteId: "<< spriteId << endl;

		string result;
		stringstream sstm;
		sstm << idObjeto << spriteId;
		result = sstm.str();
		//cout << "Resultado1: " << result << endl;
		//ifstream ifs(result);
		ifstream ifs (result.c_str());
		//ifstream infile(&result[0]);
		if ((ifs.good()) == false) {
			result = "notfoundpj-ORIGINAL.png";
		}
		ifs.close();
		//infile.close();
		//cout << "Clark Imagen: " << result << endl;

		//posicion x
		int posX;
		recibirMensaje(&posX, sizeof(int));
		//cout << "posX: " << posX << endl;

		//posicion y
		int posY;
		recibirMensaje(&posY, sizeof(int));
		//cout << "posY: " << posY << endl;

		int spx;
		recibirMensaje(&spx, sizeof(int));
		//cout << "spx: " << spx << endl;

		int spy;
		recibirMensaje(&spy, sizeof(int));
		//cout << "spy: " << spy << endl;

		char flip;
		recibirMensaje(&flip, sizeof(char));
		//cout << "flip: " << flip << endl;

		grafica.nuevoDibujable(&result[0], idObjeto,posX,posY, spx, spy, flip);
	}

	//pido x camara
	codigo = 'c';
	enviarMensaje(&codigo, sizeof(char));

	int xCamara, camSet;
	recibirMensaje(&xCamara, sizeof(int));
	recibirMensaje(&camSet, sizeof(int));
	grafica.setXCamara(xCamara,camSet);

}

bool mySocket::iniciarGrafica(){
	bool returnIGrafica = true;
	//chequeo que esten todos los jugadores
	char codigo;
	codigo = '7';
	enviarMensaje(&codigo, sizeof(char));
	bool puedoComenzar = true;
	recibirMensaje(&puedoComenzar, sizeof(bool));
	if (!puedoComenzar){
		cout << "Faltan ingresar jugadores" << endl;
		returnIGrafica = false;
		return false;
	}

	cout << "Inicio grafica" << endl;

	//pido informacion de la ventana
	codigo = '3';
	enviarMensaje(&codigo, sizeof(char));
	int anchoVentana, altoVentana;
	recibirMensaje(&anchoVentana, sizeof(int));
	recibirMensaje(&altoVentana, sizeof(int));
	//recibirMensaje(&, sizeof(int));
	grafica.setIdCliente(numeroCliente);
	grafica.init(anchoVentana, altoVentana);

	//pido informacion de los fondos
	codigo = '4';
	enviarMensaje(&codigo, sizeof(char));
	int cantidad;
	recibirMensaje(&cantidad, sizeof(int));

	/*for (int i = 0; i<cantidad; i++){
		int anchoFondo, zIndex, tamId;
		recibirMensaje(&tamId, sizeof(int));
		char idFondo[tamId];
		recibirMensaje(&idFondo, sizeof(char)*tamId);
		recibirMensaje(&anchoFondo, sizeof(int));
		recibirMensaje(&zIndex, sizeof(int));
		//ifstream infile(&idFondo[0]);

		cout << "idFondo: " << idFondo << endl;
		cout << "tamId: " << tamId << endl;

		string resultado(idFondo, tamId);

		ifstream infile (resultado.c_str());
		//ifstream infile(&result[0]);
		if ((infile.good()) == false) {
			resultado = "notfoundFondo.png";
		}

		cout << "Resultado Fondo: " << resultado << endl;
		infile.close();
		grafica.inicializarFondo(&resultado[0]);
	}*/
	int anchoFondo1, tamId1, anchoFondo2,  tamId2, anchoFondo3, tamId3;
	recibirMensaje(&tamId1, sizeof(int));
	char idFondo1[tamId1];
	recibirMensaje(&idFondo1, sizeof(char)*tamId1);
	recibirMensaje(&anchoFondo1, sizeof(int));
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado1(idFondo1, tamId1);

	recibirMensaje(&tamId2, sizeof(int));
	char idFondo2[tamId2];
	recibirMensaje(&idFondo2, sizeof(char)*tamId2);
	recibirMensaje(&anchoFondo2, sizeof(int));
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado2(idFondo2, tamId2);

	recibirMensaje(&tamId3, sizeof(int));
	char idFondo3[tamId3];
	recibirMensaje(&idFondo3, sizeof(char)*tamId3);
	recibirMensaje(&anchoFondo3, sizeof(int));
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado3(idFondo3, tamId3);

	ifstream infile1(resultado1.c_str());
	if ((infile1.good()) == false) {
		resultado1 = "notFoundFondo1.png";
	}
	ifstream infile2(resultado2.c_str());
	if ((infile2.good()) == false) {
		resultado2 = "notFoundFondo2.png";
	}
	ifstream infile3(resultado3.c_str());
	if ((infile3.good()) == false) {
		resultado3 = "notFoundFondo3.png";
	}

	grafica.inicializarFondo(&resultado1[0], &resultado2[0],&resultado3[0] );

	bool quit = false;
	SDL_Event event;

	cargarDibujables();

	bool quieto = true;

	char codigoMover = 'M';

	while( !quit ) {
		//le envio un mover siempre
		codigoMover = 'M';
		enviarMensaje(&codigoMover, sizeof(char));

		//recibo si hay cambios
		bool huboError = recibirMensaje();
		if(huboError){
			cout << "Hubo error" << endl;
			grafica.close();
			quit = true;
			returnIGrafica = false;
		} else {
			//MOSTRAR VENTANA
			if (!grafica.empiezaDeNuevo()) grafica.mostrarDibujables();
			else{
				char codigo = '8';
				enviarMensaje(&codigo, sizeof(char));
			}
		}

		SDL_PumpEvents();
		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		if (SDL_QuitRequested()){
			strcpy(&codigo,"C");
			enviarMensaje(&codigo, sizeof(char));
			grafica.close();
			quit = true;
		}
		else if (keys[SDL_GetScancodeFromKey(SDLK_UP)]){
			strcpy(&codigo,"U");
			enviarMensaje(&codigo, sizeof(char));
			quieto = false;
		}
		else if (keys[SDL_GetScancodeFromKey(SDLK_LEFT)]){
			strcpy(&codigo,"L");
			enviarMensaje(&codigo, sizeof(char));

			int x = grafica.camera.x;
			enviarMensaje(&x, sizeof(int));
			quieto = false;
		}
		else if (keys[SDL_GetScancodeFromKey(SDLK_RIGHT)]){
			strcpy(&codigo,"R");
			enviarMensaje(&codigo, sizeof(char));

			int x = grafica.camera.x;
			enviarMensaje(&x, sizeof(int));
			quieto = false;
		}
		else if (keys[SDL_GetScancodeFromKey(SDLK_r)]){
			strcpy(&codigo,"S");
			enviarMensaje(&codigo, sizeof(char));
			strcpy(&codigo,"i");
			enviarMensaje(&codigo, sizeof(char));
			recibirMensaje();
			returnIGrafica = false;
			quit = true;
			quieto = true;
		}
		else if(!quieto){
			strcpy(&codigo,"S");
			enviarMensaje(&codigo, sizeof(char));
			quieto = true;
		}

	}

	return returnIGrafica;
	//grafica.close();

	return true;
}

mySocket::~mySocket(){}


/* PARTE VIEJA DE LEER EVENTOS
while( SDL_PollEvent(&event) != 0 ) {
			if( event.type == SDL_QUIT ) {
				strcpy(&codigo,"C");
				enviarMensaje(&codigo, sizeof(char));
				grafica.close();
				quit = true;
			}

			if( event.type == SDL_KEYDOWN ) {
				char codigo;

				switch( event.key.keysym.sym ) {
				case SDLK_UP:
					strcpy(&codigo,"U");
					enviarMensaje(&codigo, sizeof(char));
					quieto = false;
					break;

				case SDLK_DOWN:
					//hardcodeo
					strcpy(&codigo,"D");
					enviarMensaje(&codigo, sizeof(char));
					quieto = false;
					break;

				case SDLK_LEFT:{
					strcpy(&codigo,"L");
					enviarMensaje(&codigo, sizeof(char));

					int x = grafica.camera.x;
					enviarMensaje(&x, sizeof(int));

					quieto = false;
					break;
				}
				case SDLK_RIGHT:{
					strcpy(&codigo,"R");
					enviarMensaje(&codigo, sizeof(char));

					int x = grafica.camera.x;
					enviarMensaje(&x, sizeof(int));
					cout << "DERECHAAAAAAAAAAAAAAAA!" << endl;

					quieto = false;
					break;
				}
				case SDLK_r:
					strcpy(&codigo,"i");
					enviarMensaje(&codigo, sizeof(char));
					grafica.clear();
					iniciarGrafica();

				default:
					break;

				}
			}else if( event.type == SDL_KEYUP && event.key.repeat == 0 && !quieto ) {
				char codigo;
				//cuando suelto tecla dejo al personaje parado
				switch( event.key.keysym.sym ) {
				case SDLK_LEFT:
					strcpy(&codigo,"S");
					enviarMensaje(&codigo, sizeof(char));
					quieto = true;
					event.type = SDL_FIRSTEVENT;
					break;
				case SDLK_RIGHT:
					strcpy(&codigo,"S");
					enviarMensaje(&codigo, sizeof(char));
					quieto = true;
					event.type = SDL_FIRSTEVENT;
					break;
					//				case SDLK_UP:
					//					strcpy(&codigo,"S");
					//					enviarMensaje(&codigo, sizeof(char));
					//					quieto = true;
					//					event.type = SDL_FIRSTEVENT;
					//					break;
				}
			}

			//event.drop;
			//event.type = SDL_FIRSTEVENT;
			cout << "DEJO DE APRETAR" << endl;
		}
 */
