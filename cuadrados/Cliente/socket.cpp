#include "socket.hpp"
#include <pthread.h>
#include "SDL2/SDL.h"

bool avanzar;

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

	this->conectado = autenticar(nombre);
}

int mySocket::getFD(){
	return this->sockfd;
}

bool mySocket::enviarMensaje(void* mensaje, int tamanioMensaje){
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

	while(corte != 0){
		error = recibirMensaje(&corte, sizeof(int));
		if (corte != 0){
			if(error){
				cout << "ENTRO A ERROR 1" << endl;
				corte = 0;
				this->desconectar();
				break;
			}	
			cout << "Corte: " << corte << endl;

			int tipoMensaje;
			error = recibirMensaje(&tipoMensaje, sizeof(int));
			if(error){
				cout << "ENTRO A ERROR 2" << endl;
				corte = 0;
				break;
			}	
			cout << "Tipo de mensaje: " << tipoMensaje << endl;

			switch(tipoMensaje){
				case 1:{
					//recibo grafica jugadores
					int x,y, spx, spy, idObjeto;
					avanzar = false;

					error = recibirMensaje(&idObjeto, sizeof(int));
					cout << "id objeto: " << idObjeto << endl;
					error = recibirMensaje(&x, sizeof(int));
					cout << "X: " << x << endl;
					error = recibirMensaje(&y, sizeof(int));
					cout << "Y: " << y << endl;
					error = recibirMensaje(&spx, sizeof(int));
					cout << "SpriteX: " << spx << endl;
					error = recibirMensaje(&spy, sizeof(int));
					cout << "SpriteY: " << spy << endl;
					error = recibirMensaje(&avanzar, sizeof(bool));
					cout << "Puede avanzar: " << avanzar << endl;

					grafica.actualizar(idObjeto, x, y, spx, spy);
					grafica.avanzarCamara (x);

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
					grafica.close();
					iniciarGrafica();

					break;
				}
			}
		}
	}
	return error;
}

bool mySocket::recibirMensaje(void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;

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
	return errorSocket;
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

void mySocket::cargarDibujables(){
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

		cout << "tamSpriteId (ESTE): " << tamSpriteId << endl;

		char spriteId[tamSpriteId];
		recibirMensaje(&spriteId, sizeof(char)*tamSpriteId);
		cout << "spriteID (ESTE): " << spriteId << endl;
		string result;
		stringstream sstm;
		sstm << idObjeto <<spriteId;
		result = sstm.str();
		cout << "Resultado1: " << result << endl;
		//ifstream ifs(result);
		ifstream ifs (result.c_str());
		//ifstream infile(&result[0]);
		if ((ifs.good()) == false) {
			result = "notfoundpj.png";
		}
		ifs.close();
		//infile.close();
		cout << "Clark Imagen: " << result << endl;

		//posicion x
		int posX;
		recibirMensaje(&posX, sizeof(int));
		cout << posX << endl;

		//posicion y
		int posY;
		recibirMensaje(&posY, sizeof(int));
		cout << posY << endl;

		int spx;
		recibirMensaje(&spx, sizeof(int));
		cout << spx << endl;

		int spy;
		recibirMensaje(&spy, sizeof(int));
		cout << spy << endl;

		grafica.nuevoDibujable(&result[0], idObjeto,posX,posY, spx, spy);
	}
}

bool mySocket::iniciarGrafica(){
	//chequeo que esten todos los jugadores
	grafica.close();

	char codigo;
	codigo = '7';
	enviarMensaje(&codigo, sizeof(char));
	bool puedoComenzar;
	recibirMensaje(&puedoComenzar, sizeof(bool));
	if (!puedoComenzar){
		cout << "Faltan ingresar jugadores" << endl;
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
	for (int i = 0; i<cantidad; i++){
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
	}

	bool quit = false;
	SDL_Event event;

	cargarDibujables();
	
	SDL_Rect camera = { 0, 0, anchoVentana, altoVentana };

	float constCamera = ((3*anchoVentana)/4);

	bool quieto = true;

	while( !quit ) {

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

				case SDLK_LEFT:
					strcpy(&codigo,"L");
					enviarMensaje(&codigo, sizeof(char));
					quieto = false;
					break;

				case SDLK_RIGHT:
					strcpy(&codigo,"R");
					enviarMensaje(&codigo, sizeof(char));
					quieto = false;
					break;

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
				case SDLK_UP:
					strcpy(&codigo,"S");
					enviarMensaje(&codigo, sizeof(char));
					quieto = true;
					event.type = SDL_FIRSTEVENT;
					break;
				}
			}
		}

		//recibo si hay cambios
		bool huboError = false;
		huboError = recibirMensaje();
		if(huboError){
			cout << "Hubo error" << endl;
			grafica.close();
			quit = true;
		} else {
			//MOSTRAR VENTANA
			if(avanzar == true){
				camera.x += 10;
				avanzar = false;
			}

			if( camera.x < 0 )
			{ 
				camera.x = 0;
			}
			if( camera.x > 1950 /*LEVEL_WIDTH*/ - camera.w )
			{
				camera.x = 1950 /*LEVEL_WIDTH*/ - camera.w;
			}
			grafica.mostrarDibujables(camera, constCamera);
		}
	}

	grafica.close();

	return true;
}

mySocket::~mySocket(){}
