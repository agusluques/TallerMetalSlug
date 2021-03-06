#include "socket.hpp"
#include <pthread.h>
#include "SDL2/SDL.h"

bool avanzar;
char nombreCliente[50];
bool enviado = false;
bool cambiaNivel = false;

mySocket::mySocket(char* puerto, char* IP){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->sockfd = 0;
	//this->address = str(IP);
	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(this->puerto);
	this->conectado = false;

	estaEnPantallaScore = false;
	quitGrafica = false;
	returnIGrafica = true;
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

	//char* tmp = (char*)mensaje;
	//	int* tmp = (int*)mensaje;
	//	cout << "ENVIA: " << (*tmp) << endl;

	return errorSocket;
}

bool mySocket::recibirMensaje(void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;

	struct timeval tv;
	tv.tv_sec = 10;  // 10 Secs Timeout
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
	setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

	bool errorSocket = false;
	//while ((acumulador < tamanio) && (errorSocket == false)){
	//	bytesRecibidos = read(this->sockfd, buffer, tamanio);
	//	if (bytesRecibidos < 0){
	//		cout << "Se cayo la conexion con el servidor" << endl;
	//		errorSocket = true;
	//	} else {
	//		acumulador += bytesRecibidos;
	//	}
	//}

	int n = read(this->sockfd, buffer, tamanio);
	if (n < 0) errorSocket = true;

	//char* tmp = (char*)buffer;
		//int* tmp = (int*)buffer;
		//cout << "RECIBE: " << (*tmp) << endl;

	return errorSocket;
}

bool mySocket::recibirMensaje(bool &pasarNivel){
	bool error = false;
	char codigo;
	codigo = '5';
	error = enviarMensaje(&codigo, sizeof(char));
	if(error){
		cout << "ENTRO A ERROR" << endl;
	}

	int corte = 1;
	error = recibirMensaje(&corte, sizeof(int));

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
			//cout << "RECIBO GRAFICA JUGADORES" << endl;
			//recibo grafica jugadores
			int x,y, spx, spy, idObjeto;
			int tipo;
			char flip;
			avanzar = false;

			error = recibirMensaje(&idObjeto, sizeof(int));
			error = recibirMensaje(&x, sizeof(int));
			error = recibirMensaje(&y, sizeof(int));
			error = recibirMensaje(&spx, sizeof(int));
			error = recibirMensaje(&spy, sizeof(int));
			error = recibirMensaje(&flip, sizeof(char));
			error = recibirMensaje(&avanzar, sizeof(bool));
			error = recibirMensaje(&tipo, sizeof(int));

			int xCamara, setCamara;
			error = recibirMensaje(&xCamara, sizeof(int));

			grafica.setXCamara(xCamara);
			grafica.actualizar(idObjeto, x, y, spx, spy, avanzar, flip, tipo);

			break;
		}
		case 2:
			//cout << "MATO ENEMIGO" << endl;
			//MATAR ENEMIGO!
			int idObjeto;
			error = recibirMensaje(&idObjeto, sizeof(int));

			grafica.quitarEnemigo(idObjeto);

			break;
		case 3:{
			//cout << "RECIBO MENSAJE" << endl;
			//recibo mensaje
			int tam;

			error = recibirMensaje(&tam, sizeof(int));
			char mensaje[tam];
			error = recibirMensaje(&mensaje, sizeof(char)*tam);
			cout << "(*)Mensaje: " << mensaje << endl;

			break;
		}
		case 4:{
			//cout << "PIDE CAMBIAR NIVEL" << endl;
			//cerrarGrafica();
			error = recibirMensaje(&corte, sizeof(int));
			cambiarDeNivel();
			estaEnPantallaScore = false;
			cambiaNivel = true;

			break;
		}
		case 5:{
			//cout << "INICIAR GRAFICA" << endl;
			error = recibirMensaje(&corte, sizeof(int));
			grafica.setearFondoScore(false);
			iniciarGrafica(pasarNivel);
			cerrarGrafica();
			cerrar();
			desconectar();

			break;
		}

		case 6: {
			//cout << "RECIBO BALAS" << endl;
			//recibo balas
			int id, x, y, tipoDisp, dirBala, spY;

			error = recibirMensaje(&id, sizeof(int));
			error = recibirMensaje(&x, sizeof(int));
			error = recibirMensaje(&y, sizeof(int));
			error = recibirMensaje(&dirBala, sizeof(int));
			error = recibirMensaje(&tipoDisp, sizeof(int));
			error = recibirMensaje(&spY, sizeof(int));

			grafica.actualizarBalas(id, x, y, dirBala, tipoDisp, spY);

			break;
		}

		case 9: {
			//cout << "RECIBO BONUS" << endl;
			//recibo Bonus
			//cout << "ENTRO A RECIBIR BONUS" << endl;
			int id, x, y, tipoBonus;

			error = recibirMensaje(&id, sizeof(int));
			error = recibirMensaje(&x, sizeof(int));
			error = recibirMensaje(&y, sizeof(int));
			error = recibirMensaje(&tipoBonus, sizeof(int));
			//cout << "ID: " << id << endl;
			//cout << "X Bonus: " << x << endl;
			//cout << "Y Bonus: " << y << endl;
			//cout << "Tipo Bonus: " << tipoBonus << endl;

			//grafica.agregarBonus(x, y, id, tipoBonus);
			grafica.actualizarBonus(id, x, y, tipoBonus);

			break;

		}

		case 8:{
			//cout << "RECIBO BAJAS BALAS" << endl;
			//recibo bajas de balas
			int id;
			error = recibirMensaje(&id, sizeof(int));

			grafica.quitarBalas(id);

			break;
		}

		case 10:{
			//cout << "RECIBO BAJAS BONUS" << endl;
			int id;
			error = recibirMensaje(&id, sizeof(int));
			grafica.quitarBonus(id);

			break;
		}

		case 11:{
			//cout << "RECIBO SCORE 11" << endl;
			int score;
			error = recibirMensaje(&score, sizeof(int));

			cout << score << endl;
			//grafica.mostrarScores(score);

			break;
		}

		case 12:{
			//cout << "RECIBO SCORE 12" << endl;
			int id,score;

			error = recibirMensaje(&id, sizeof(int));
			error = recibirMensaje(&score, sizeof(int));

			grafica.actualizarScore(id, score);

			break;
		}

		case 13:{
			//cout << "RECIBO VIDA" << endl;
			int id,vida;

			error = recibirMensaje(&id, sizeof(int));
			error = recibirMensaje(&vida, sizeof(int));

			if(vida < 0){
				vida = 0;
			}

			if(vida > 10){
				vida = 10;
			}

			//cout << "VIDA: " << vida << endl;

			grafica.actualizarVida(id, vida);

			break;
		}

		case 7:{
			//cout << "CASE 7" << endl;
			return true;
			break;
		}

		case 20:{
			cout << "RECIBO PASE DE NIVEL" << endl;

			//pasarNivel = true;
			//cerrarGrafica();

			grafica.setearFondoScore(true);
			estaEnPantallaScore = true;

			break;
		}

		case 21:{
			cout << "RECIBO CIERRE GRAFICA" << endl;

			grafica.setearFondoScore(false);
			estaEnPantallaScore = false;

			grafica.liberar();

			quitGrafica = true;
			returnIGrafica = false;

			cout << "00000000000   0000   00000       000 " << endl;
			cout << "00000000000   0000   000000      000 " << endl;
			cout << "0000          0000   000 000     000 " << endl;
			cout << "0000          0000   000  000    000 " << endl;
			cout << "00000000000   0000   000   000   000 " << endl;
			cout << "00000000000   0000   000    000  000 " << endl;
			cout << "0000          0000   000     000 000 " << endl;
			cout << "0000          0000   000      000000 " << endl;
			cout << "0000          0000   000       00000 " << endl;
			cout << "0000          0000   000        0000 " << endl;

			break;
		}

		default:
			break;

		}

		if(!cambiaNivel)
			error = recibirMensaje(&corte, sizeof(int));
		cambiaNivel = false;
		//cout << "SALGO CASES" << endl;

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
		result = "img/pjs/" + result;
		//cout << "Resultado1: " << result << endl;
		//ifstream ifs(result);
		ifstream ifs (result.c_str());
		//ifstream infile(&result[0]);
		if ((ifs.good()) == false) {
			result = "img/pjs/notfoundpj-ORIGINAL.png";
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

	int xCamara;
	recibirMensaje(&xCamara, sizeof(int));
	grafica.setXCamara(xCamara);

}

void mySocket::cambiarDeNivel(){
	cout << "RE Inicio grafica" << endl;
	grafica.liberar();
	//pido informacion de los fondos
	char codigo = '4';
	enviarMensaje(&codigo, sizeof(char));

	int cantidad;
	recibirMensaje(&cantidad, sizeof(int));
	cout << "CANTIDAD: " << cantidad << endl;

	int anchoFondo1, tamId1, anchoFondo2,  tamId2, anchoFondo3, tamId3;
	recibirMensaje(&tamId1, sizeof(int));
	//cout << "TAMID1: " << tamId1 << endl;
	char idFondo1[tamId1];
	recibirMensaje(&idFondo1, sizeof(char)*tamId1);
	//cout << "ID Fondo 1: " << idFondo1 << endl;
	recibirMensaje(&anchoFondo1, sizeof(int));
	//cout << "Ancho 1: " << anchoFondo1 << endl;
	string resultado1(idFondo1, tamId1);
	resultado1 = "img/fondos/" + resultado1;
	//cout << "FONDO 1: " << resultado1 << endl;

	recibirMensaje(&tamId2, sizeof(int));
	char idFondo2[tamId2];
	recibirMensaje(&idFondo2, sizeof(char)*tamId2);
	recibirMensaje(&anchoFondo2, sizeof(int));
	string resultado2(idFondo2, tamId2);
	resultado2 = "img/fondos/" + resultado2;
	cout << "FONDO 2: " << resultado2 << endl;

	recibirMensaje(&tamId3, sizeof(int));
	char idFondo3[tamId3];
	recibirMensaje(&idFondo3, sizeof(char)*tamId3);
	recibirMensaje(&anchoFondo3, sizeof(int));
	string resultado3(idFondo3, tamId3);
	resultado3 = "img/fondos/" + resultado3;
	cout << "FONDO 3: " << resultado3 << endl;

	ifstream infile1(resultado1.c_str());
	if ((infile1.good()) == false) {
		resultado1 = "img/fondos/notFoundFondo1.png";
	}
	ifstream infile2(resultado2.c_str());
	if ((infile2.good()) == false) {
		resultado2 = "img/fondos/notFoundFondo2.png";
	}
	ifstream infile3(resultado3.c_str());
	if ((infile3.good()) == false) {
		resultado3 = "img/fondos/notFoundFondo3.png";
	}

	grafica.inicializarFondo(&resultado1[0], &resultado2[0],&resultado3[0]);

	grafica.setearFondoScore(false);

	//cargarDibujables();

	estaEnPantallaScore = false;
	bool quieto = true;
	bool hayAnterior = false;

	cout << "inicie fondos" << endl;
}

bool mySocket::iniciarGrafica(bool &pasarNivel){
	//chequeo que esten todos los jugadores
	char codigo;
	codigo = '7';
	enviarMensaje(&codigo, sizeof(char));
	int puedoComenzar;
	recibirMensaje(&puedoComenzar, sizeof(int));
	if (puedoComenzar == 0){
		cout << "Faltan ingresar jugadores" << endl;
		returnIGrafica = false;
		return false;
	} else if (puedoComenzar == -1){
		cout << "Ya han ingresado la cantidad maxima de jugadores" <<endl;
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

	int anchoFondo1, tamId1, anchoFondo2,  tamId2, anchoFondo3, tamId3;
	recibirMensaje(&tamId1, sizeof(int));
	//cout << "TAMID1: " << tamId1 << endl;
	char idFondo1[tamId1];
	recibirMensaje(&idFondo1, sizeof(char)*tamId1);
	//cout << "ID Fondo 1: " << idFondo1 << endl;
	recibirMensaje(&anchoFondo1, sizeof(int));
	//cout << "Ancho 1: " << anchoFondo1 << endl;
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado1(idFondo1, tamId1);
	resultado1 = "img/fondos/" + resultado1;
	//cout << "FONDO 1: " << resultado1 << endl;

	recibirMensaje(&tamId2, sizeof(int));
	char idFondo2[tamId2];
	recibirMensaje(&idFondo2, sizeof(char)*tamId2);
	recibirMensaje(&anchoFondo2, sizeof(int));
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado2(idFondo2, tamId2);
	resultado2 = "img/fondos/" + resultado2;
	//cout << "FONDO 2: " << resultado2 << endl;

	recibirMensaje(&tamId3, sizeof(int));
	char idFondo3[tamId3];
	recibirMensaje(&idFondo3, sizeof(char)*tamId3);
	recibirMensaje(&anchoFondo3, sizeof(int));
	//recibirMensaje(&zIndex, sizeof(int));
	string resultado3(idFondo3, tamId3);
	resultado3 = "img/fondos/" + resultado3;
	//cout << "FONDO 3: " << resultado3 << endl;

	ifstream infile1(resultado1.c_str());
	if ((infile1.good()) == false) {
		resultado1 = "img/fondos/notFoundFondo1.png";
	}
	ifstream infile2(resultado2.c_str());
	if ((infile2.good()) == false) {
		resultado2 = "img/fondos/notFoundFondo2.png";
	}
	ifstream infile3(resultado3.c_str());
	if ((infile3.good()) == false) {
		resultado3 = "img/fondos/notFoundFondo3.png";
	}

	grafica.inicializarFondo(&resultado1[0], &resultado2[0],&resultado3[0] );

	//pido informacion de las barras de energia y scores
	codigo = 'a';
	enviarMensaje(&codigo,sizeof(char));
	int cantida;
	recibirMensaje(&cantida,sizeof(int));
	for (int i = 0; i < cantida; i++){
		int id,spY;
		recibirMensaje(&id, sizeof(int));
		recibirMensaje(&spY, sizeof(int));

		string numero ="";
		string imagen;

		numero = static_cast<ostringstream*>(&(ostringstream() << id))->str();


		imagen = "img/" + numero + "barra.png";

		grafica.agregarEnergia(id,spY,imagen);
		grafica.agregarScores(id);

	}

	pasarNivel = false;
	SDL_Event event;

	cargarDibujables();

	bool quieto = true;

	char codigoMover = 'M';

	const Uint8 *keyAnterior;

	estaEnPantallaScore = false;
	bool hayAnterior = false;

	while( !quitGrafica ) {
		//le envio un mover siempre
		codigoMover = 'M';
		if (!estaEnPantallaScore) enviarMensaje(&codigoMover, sizeof(char));

		//recibo si hay cambios
		bool huboError = recibirMensaje(pasarNivel);
		if(pasarNivel){
			quitGrafica = true;
			returnIGrafica = false;
		}
		if(huboError){
			cout << "Hubo error" << endl;
			grafica.close();
			quitGrafica = true;
			returnIGrafica = false;
		} else {
			//MOSTRAR VENTANA
			grafica.mostrarDibujables();
		}

		if(!estaEnPantallaScore){
			//if (!grafica.estaMuerto()){
			SDL_PumpEvents();
			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			if(hayAnterior){
				if((keyAnterior[SDL_GetScancodeFromKey(SDLK_h)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)])){
					strcpy(&codigo,"S");
					enviarMensaje(&codigo, sizeof(char));
					quieto = true;
				}
			}

			if( (keys[SDL_GetScancodeFromKey(SDLK_w)]) && (keys[SDL_GetScancodeFromKey(SDLK_a)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 3;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}

			if (SDL_QuitRequested()){
				strcpy(&codigo,"C");
				enviarMensaje(&codigo, sizeof(char));
				//grafica.close();
				quitGrafica = true;
				returnIGrafica = false;
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_w)]) && (keys[SDL_GetScancodeFromKey(SDLK_d)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 1;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_s)]) && (keys[SDL_GetScancodeFromKey(SDLK_a)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 5;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_s)]) && (keys[SDL_GetScancodeFromKey(SDLK_d)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 7;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_s)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 6;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_w)]) && (keys[SDL_GetScancodeFromKey(SDLK_h)]) ){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = 2;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_h)]){
				strcpy(&codigo,"d");
				enviarMensaje(&codigo, sizeof(char));
				int direccionDisparo = -1;
				enviarMensaje(&direccionDisparo, sizeof(int));
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_j)]){
				strcpy(&codigo,"J");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_a)]){
				strcpy(&codigo,"L");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_d)]){
				strcpy(&codigo,"R");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_w)]) && (keys[SDL_GetScancodeFromKey(SDLK_d)]) ){
				strcpy(&codigo,"Q");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if( (keys[SDL_GetScancodeFromKey(SDLK_w)]) && (keys[SDL_GetScancodeFromKey(SDLK_a)]) ){
				strcpy(&codigo,"E");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_w)]){
				strcpy(&codigo,"U");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			else if (keys[SDL_GetScancodeFromKey(SDLK_s)]){
				strcpy(&codigo,"D");
				enviarMensaje(&codigo, sizeof(char));
				quieto = false;
			}
			/*else if (keys[SDL_GetScancodeFromKey(SDLK_r)]){
			strcpy(&codigo,"S");
			enviarMensaje(&codigo, sizeof(char));
			strcpy(&codigo,"i");
			enviarMensaje(&codigo, sizeof(char));
			recibirMensaje(pasarNivel);
			returnIGrafica = false;
			quit = true;
			quieto = true;
			}*/
			else if(!quieto){
				strcpy(&codigo,"S");
				enviarMensaje(&codigo, sizeof(char));
				quieto = true;
			}
			//}

			keyAnterior = keys;
			hayAnterior = true;

		}
	}

	return returnIGrafica;
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
