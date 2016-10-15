#include "socket.hpp"
#include <pthread.h>
#include <list>
#include <fstream>
#include "mensajeClass.hpp"
#include "usuarioClass.hpp"
#include "DibujableServer.h"
#include "FondoServer.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

#define DEBUG 2

using namespace rapidxml;
using namespace std;

list<DibujableServer> listaDibujables;
list<FondoServer> listaFondos;
int ANCHO_VENTANA;
int ALTO_VENTANA;
char* archivoXml;

list<mensajeClass> listaDeMensajes;
list<usuarioClass> listaDeUsuarios;
int xMin;

//esto se lee del xml, en caso que no haya nada es 2 por defecto.
int cantidadJugadores = 2;

//list<mensajeClass> listaDeMensajesAlCliente;
pthread_mutex_t mutexLista = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaRecibir = PTHREAD_MUTEX_INITIALIZER;


void loggear(string usr, string destinatario, string mensaje){
	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);
	char buffTime[20];
	struct tm *sTm;

	time_t now = time (0);
	sTm = gmtime (&now);
	strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S", sTm);
	if (DEBUG == 2)
	{
		archLog << buffTime << " Nuevo mensaje de: " << usr << " Para: " << destinatario << " Contenido: " << mensaje << endl;
	}else {
		archLog << buffTime << " Nuevo mensaje de: " << usr << " Para: " << destinatario << endl;
	}

	archLog.close();
}

void loggearServer(string mensaje){
	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);
	char buffTime[20];
	struct tm *sTm;

	time_t now = time (0);
	sTm = gmtime (&now);
	strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S", sTm);

	archLog << buffTime << " MENSAJE SERVIDOR : " << mensaje << endl;

	archLog.close();
}

void recibirMensaje(int sockfd, void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;
	//bool errorSocket = false;
	//while ((acumulador < tamanio) && (errorSocket == false)){
	bytesRecibidos = read(sockfd, buffer, tamanio);
	if (bytesRecibidos == (-1)){
		cout << "Error al recibir datos SOCKET" << endl;
		loggearServer( " ERROR AL RECIBIR DATOS SOCKET");
		//		errorSocket = true;
		//	} else {
		//		acumulador += bytesRecibidos;
	}
	//}
}

bool enviarMensaje(int sockfd, void* mensaje, int tamanioMensaje){
	//int bytesEnviados = 0;
	bool errorSocket = false;

	//while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
	//int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
	int n = write(sockfd, mensaje, tamanioMensaje);
	if(n < 0){
		errorSocket = true;
	}
	//bytesEnviados += n;
	//}
	return errorSocket;
}

void responderLogin(int newsockfd, int respuesta, string mensaje, int numeroCliente){
	char* cstr = new char [mensaje.length()+1];
	strcpy (cstr, mensaje.c_str());
	int tamanioMensaje = mensaje.length()+1;

	enviarMensaje(newsockfd, &respuesta, sizeof(int));
	enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
	enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));
	enviarMensaje(newsockfd, &numeroCliente, sizeof(int));

	delete[] cstr;
}

void cantidadDeJugadores(char* xml){
	file<> xmlFile(xml);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags

	xml_node<> *cantidad = doc.first_node("cantidadDeJugadores");
	if (cantidad != NULL) cantidadJugadores = atoi(cantidad->value());

}

void cargarFondos(char* xml){
	file<> xmlFile(xml);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags

	xml_node<> *ventana = doc.first_node("ventana");
	xml_node<> *ancho = ventana->first_node("ancho");
	xml_node<> *alto = ventana->first_node("alto");

	ANCHO_VENTANA = atoi(ancho->value());
	ALTO_VENTANA = atoi(alto->value());

	xml_node<> *capas = doc.first_node("capas");
	xml_node<> *capa1 = capas->first_node("capa");
	xml_node<> *fondoXML = capa1->first_node("imagen_fondo");
	xml_node<> *anchoFondo = fondoXML->next_sibling("ancho");
	xml_node<> *zFondo = anchoFondo->next_sibling("z-index");

	FondoServer nuevo;
	nuevo.setAncho(atoi(anchoFondo->value()));
	nuevo.setSpriteId(fondoXML->value());
	nuevo.setZindex(atoi(zFondo->value()));
	listaFondos.push_back(nuevo);


}

void buscarNombreUsuario(char *nombreRetorno, int numeroUsuario){
	list<usuarioClass>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(*it).nombreUsuario().c_str());
}

void agregaraLista(int numeroCliente, int usrDest, int x, int y, int spx, int spy, bool avanzar){
	char nombreAutor[50];
	char nombreDestino[50];
	buscarNombreUsuario(nombreAutor, numeroCliente);
	buscarNombreUsuario(nombreDestino, usrDest);

	mensajeClass mensajeObj(nombreAutor, nombreDestino, numeroCliente, x, y, spx, spy, avanzar);

	int a = pthread_mutex_trylock(&mutexLista);
	/*if (a != 0) {
		cout<<"Otro hilo usando la lista"<<endl;
	}*/

	listaDeMensajes.push_back(mensajeObj);
	//loggear(mensajeObj.nombreAutor(),mensajeObj.nombreDestinatario(),mensajeObj.getMensaje());
	pthread_mutex_unlock (&mutexLista);
}

void enviarMensajeAConectados(string mensaje){
	//envio a todos los q esten online el mensaje
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			//mensajes = tipo 3
			char nombreDestino[50];
			buscarNombreUsuario(nombreDestino, (*it).numCliente());

			mensajeClass mensajeObj(3,nombreDestino,mensaje);
			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
}

void enviarAConectados(int numeroCliente, int nuevaCordX, int nuevaCordY, int nuevoSpX, int nuevoSpY, bool avanzar){
	//envio a todos los q esten online el mensaje de q se modifico un objeto
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			agregaraLista(numeroCliente, (*it).numCliente(), nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, avanzar);
		}
	}
}

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	//char buffer[TAM_MAX];
	int newsockfd = (long)arg;
	bool abierto = true;
	int numeroCliente;
	usuarioClass* userPoint= NULL;

	while (abierto){

		//int tamanio = (sizeof(char));
		//char codigo;
		//recibirMensaje(newsockfd, &codigo, tamanio);
		//cout << "Codigo: " << codigo << endl;

		/*struct timeval tv;
		tv.tv_sec = 10;  // 10 Secs Timeout
		tv.tv_usec = 0;  // Not init'ing this can cause strange errors
		setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
		 */
		char codigo;
		int data = read(newsockfd, &codigo, sizeof(char));
		/*if(data < 0){
			cout << "Se cayo la conexion con el cliente " << userPoint->nombreUsuario() << endl;

			userPoint->desconectar();
			list<usuarioClass>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			memcpy(&(*it),userPoint,sizeof(usuarioClass));

			userPoint->loggear(" perdio la conexion con el servidor (se corto internet)");

			abierto = false;
		}*/
		//cliente->recibirMensaje(&cod, sizeof(char));

		switch(codigo){

		case '0': {
			/*char cod = '0';
			if(enviarMensaje(newsockfd, &cod, sizeof(char)) == true){
				cout << "Se perdio la conexion con el cliente " << userPoint->nombreUsuario() << endl;

				userPoint->desconectar();
				list<usuarioClass>::iterator it = listaDeUsuarios.begin();
				advance(it, numeroCliente-1);
				memcpy(&(*it),userPoint,sizeof(usuarioClass));

				userPoint->loggear(" perdio la conexion con el servidor (mata proceso)");

				abierto = false;
			}*/
			break;
		}

		case '1': {
			//cout << "Entro a /1 que es conectar " << endl;
			int tamNombre;

			recibirMensaje(newsockfd, &tamNombre, sizeof(int));
			char nombre[tamNombre];
			recibirMensaje(newsockfd, &nombre, sizeof(char)*tamNombre);

			//VALIDO SI EL NOMBRE YA EXISTE
			bool respuesta = true;
			bool estabaDesconectado = false;
			char mensaje[256];

			//comparo con todos los usuarios q esten si existe devuelvo error
			for (list<usuarioClass>::iterator i = listaDeUsuarios.begin(); i != listaDeUsuarios.end(); ++i) {
				if ((*i).nombreUsuario().compare(nombre) == 0){
					if(!(*i).estaConectado()){
						estabaDesconectado = true;
						(*i).conectar();
						numeroCliente = (*i).numCliente();

						//mandar msj a todos q volvio asi no esta mas gris
						int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
						for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
							if ((*it).id == numeroCliente){
								//hacer metodo mover up y q sume solo en la clase dibujableServer...
								nuevaCordY = it->y;
								nuevaCordX = it->x;
								//parado
								nuevoSpX = it->spX = 0;
								nuevoSpY = it->spY = 1;
							}
						}

						//envio a todos los q esten online el mensaje de q se modifico un objeto
						enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, false);
					}
					respuesta = false;
					strcpy(mensaje,"El nombre ya esta en uso");
				}
			}

			if (respuesta){
				strcpy(mensaje,"Bienvenido");
				usuarioClass nuevoUsuario(nombre, listaDeUsuarios.size()+1);
				listaDeUsuarios.push_back(nuevoUsuario);

				numeroCliente = listaDeUsuarios.size();

				//creo el dibujable del nuevo cliente
				DibujableServer nuevo;
				nuevo.setId(listaDeUsuarios.size());
				nuevo.setSpriteId("player");
				nuevo.setX(1 + rand() % (150));
				//nuevo.setY(ALTO_VENTANA - 50);
				nuevo.setY(ALTO_VENTANA - 100);
				nuevo.setSpX (0);
				nuevo.setSpY(1);
				listaDibujables.push_back(nuevo);
			}else if(estabaDesconectado){
				strcpy(mensaje,"Bienvenido nuevamente");
				respuesta = true;
			}
			responderLogin(newsockfd,respuesta,mensaje, numeroCliente);

			break;
		}
		case '2':{
			//cout << "Entro a /2 que es desconectar" << endl;
			abierto = false;

			list<usuarioClass>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			it->desconectar();

			break;
		}
		case '3':{
			//cout << "Entro a /3 que es ventana" << endl;
			cargarFondos(archivoXml);
			enviarMensaje(newsockfd, &ANCHO_VENTANA, sizeof(int));
			enviarMensaje(newsockfd, &ALTO_VENTANA, sizeof(int));

			break;
		}
		case '4':{
			//cout << "Entro a /4 que es fondos" << endl;
			cargarFondos(archivoXml);
			int tamano = listaFondos.size();
			enviarMensaje(newsockfd, &tamano, sizeof(int));
			for(list<FondoServer>::iterator i =listaFondos.begin(); i != listaFondos.end();++i){
				int ancho = (*i).ancho;
				int z = (*i).zindex;
				int tamFondoId = sizeof((*i).spriteId);
				char spriteId[tamFondoId];
				strcpy(spriteId, (*i).spriteId.c_str());

				enviarMensaje(newsockfd, &tamFondoId, sizeof(int));
				enviarMensaje(newsockfd, &spriteId, sizeof(char)*tamFondoId);
				enviarMensaje(newsockfd, &ancho, sizeof(int));
				enviarMensaje(newsockfd, &z, sizeof(int));
			}
			break;
		}
		case '5': {
			//cout << "Entro a /5 que es recibir" << endl;

			for (list<mensajeClass>::iterator i = listaDeMensajes.begin(); i != listaDeMensajes.end(); ++i) {
				char nombre[50];
				//****me puedo ahorrar esta busqueda, guardando el id y no el nombre
				buscarNombreUsuario(nombre, numeroCliente);
				if ((*i).nombreDestinatario().compare(nombre) == 0 ){
					int corte = 1;
					enviarMensaje(newsockfd, &corte, sizeof(int));

					//envio tipo de mensaje
					int tipoMensaje = (*i).getTipoMensaje();
					enviarMensaje(newsockfd, &tipoMensaje, sizeof(int));

					cout << "TIPO MSJ: " << tipoMensaje << endl;

					switch(tipoMensaje){
					case 1:{
						//envio de grafica
						int  xCord = (*i).getX();
						int  yCord = (*i).getY();
						int spX = (*i).getSpX();
						int spY = (*i).getSpY();
						int idObjeto = (*i).getidObjeto();
						bool puedeAvanzar = (*i).getBoolAvanzar();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &spX, sizeof(int));
						enviarMensaje(newsockfd, &spY, sizeof(int));
						enviarMensaje(newsockfd, &puedeAvanzar, sizeof(bool));

						break;
					}
					case 2:
						//envio de fondo

						break;

					case 3:{
						//envio de mensajes
						string mensaje = (*i).getMensaje();
						char* cstr = new char [mensaje.length()+1];
						strcpy (cstr, mensaje.c_str());
						int tamanioMensaje = mensaje.length()+1;

						enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
						enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));

						break;
					}
					}

					i = listaDeMensajes.erase(i);
					i--;
				}
			}
			int tamanioMensaje = 0;
			enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));

			break;
		}

		case '6': {
			//peticion de cantidad de objetos a dibujar
			int cantidad = listaDibujables.size();
			enviarMensaje(newsockfd,&cantidad,sizeof(int));

			break;
		}

		case '7': {
			//pido si puede empezar el juego
			bool respuesta = false;
			if (listaDeUsuarios.size() == cantidadJugadores){
				respuesta = true;
			}
			enviarMensaje(newsockfd,&respuesta,sizeof(bool));

			break;
		}

		case '9':{
			//caso envio objetos dibujables
			int numeroUsuario;
			recibirMensaje(newsockfd, &numeroUsuario, sizeof(int));

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroUsuario-1);

			//idObjeto
			enviarMensaje(newsockfd,&(*it).id,sizeof(int));

			//spriteId
			int tamSpriteId = sizeof((*it).spriteId);
			char spriteId[tamSpriteId];
			enviarMensaje(newsockfd,&tamSpriteId,sizeof(int));
			strcpy(spriteId, (*it).spriteId.c_str());
			enviarMensaje(newsockfd,spriteId,tamSpriteId);

			//posicion x
			enviarMensaje(newsockfd,&(*it).x,sizeof(int));

			//posicion y
			enviarMensaje(newsockfd,&(*it).y,sizeof(int));

			enviarMensaje(newsockfd,&(*it).spX,sizeof(int));

			enviarMensaje(newsockfd,&(*it).spY,sizeof(int));

			break;
		}

		case 'U':{
			// ESTO ES PARA MODIFICAR LA LISTA DE OBJ DEL SERVIDOR..
			// Q CUANDO UN CLIENTE NUEVO SE CONECTA DURANTE EL JUEGO, COPIA ESOS DATOS
			//**CON UN MAPA ACCEDO MAS RAPIDO LLEGADO AL CASO DE QUE SEAN BANDA DE OBJETOS
			//modifico el dibujable de la lista del servidor
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;

			/*
			 * USAR AVANCE EN LA LISTA
			 * */
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				//ACA AVANZAR
				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					nuevaCordY = it->y = it->y - 2;
					nuevaCordX = it->x;
					nuevoSpX = it->spX = it->spX+1;				//COMENTARIO: DEBO CAMBIAR DE LINEA
					if (nuevoSpX > 5) nuevoSpX = it->spX = 0;	//DE SPRITE PARA QUE SUBA
					nuevoSpY = it->spY = 0;
				}
			}

			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, false);

			break;
		}
		case 'D':{
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					nuevaCordY = it->y = it->y + 2;
					nuevaCordX = it->x;
					nuevoSpX = it->spX = it->spX+1;				//COMENTARIO: DEBO CAMBIAR DE LINEA
					if (nuevoSpX > 5) nuevoSpX = it->spX = 0;	//DE SPRITE PARA QUE BAJE?
					nuevoSpY = it->spY = 0;
				}
			}

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, false);

			break;
		}
		case 'L':{
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			xMin = 10000;
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if((*it).x < xMin){
					xMin = it->x;
				}

				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					nuevaCordY = it->y;
					nuevaCordX = it->x = it->x -2;
					nuevoSpX = it->spX = it->spX+1;
					if (nuevoSpX > 5) nuevoSpX = it->spX = 0;
					nuevoSpY = it->spY = 0;
				}
			}

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			bool avanzar;
			if(xMin > 300){
				avanzar = true;
			}
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, avanzar);
			cout <<"XMIN: " << xMin << endl;
			break;
		}
		case 'R':{
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			xMin = 10000;
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if((*it).x < xMin){
					xMin = it->x;
				}

				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					nuevaCordY = it->y;
					nuevaCordX = it->x = it->x +2;
					nuevoSpX = it->spX = it->spX+1;
					if (nuevoSpX > 5) nuevoSpX = it->spX = 0;
					nuevoSpY = it->spY = 0;
				}
			}

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			bool avanzar;
			if(xMin > 300){
				avanzar = true;
			}
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, avanzar);
			cout <<"XMIN: " << xMin << endl;
			break;
		}
		case 'C':{
			//caso cerrar ventana grafica
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					nuevaCordX = it->x;
					nuevaCordY = it->y;
					//momia
					nuevoSpX = it->spX = 1;
					nuevoSpY = it->spY = 1;
				}
			}

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, false);

			//envio un msj a todos los q esten online q se desconecto el usuario
			list<usuarioClass>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);

			string mensaje = (*it).nombreUsuario() + " Se ah desconectado";
			enviarMensajeAConectados(mensaje);

			break;
		}
		case 'S':{
			//caso se queda parado

			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if ((*it).id == numeroCliente){
					//sprite parado
					nuevaCordX = it->x;
					nuevaCordY = it->y;
					nuevoSpX = it->spX = 0;
					nuevoSpY = it->spY = 1;
				}
			}

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, false);

			break;
		}
		
		case 'i':{
			int x, y;
			
			/*for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
				if((*it).estaConectado()){
					//mensajes = tipo 3
					char nombreDestino[50];
					buscarNombreUsuario(nombreDestino, (*it).numCliente());

					mensajeClass mensajeObj(4,nombreDestino,NULL);
					int a = pthread_mutex_trylock(&mutexLista);
					listaDeMensajes.push_back(mensajeObj);
					pthread_mutex_unlock (&mutexLista);
				}
			}*/
			listaDibujables.clear();
			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				DibujableServer nuevo;
				nuevo.setId(i);
				nuevo.setSpriteId("player");
				nuevo.setX(1+rand() % (150));
				//nuevo.setY(ALTO_VENTANA - 50);
				nuevo.setY(ALTO_VENTANA-100);
				nuevo.setSpX (0);
				nuevo.setSpY(1);
				listaDibujables.push_back(nuevo);
			}
			break;
		}
	
	
		default:
			//cout << "No deberia ingresar aca" << endl;
			break;
		}

}
	//close(newsockfd);
	pthread_exit(NULL);
}

mySocketSrv :: mySocketSrv(char* puerto, char* xml){
	//cargo fondos
	archivoXml = xml;
	cargarFondos(xml);
	cantidadDeJugadores(xml);

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		cout << "Error en la apertura del socket" << endl;
		loggearInterno( " ERROR EN LA APERTURA DEL SOCKET");
	}
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(this->puerto);

	/*//cargo un fondo cualquiera
	//ver los valores del id
	DibujableServer nuevo;
	nuevo.setId(0);
	nuevo.setSpriteId("fondo");
	nuevo.setX(0);
	nuevo.setY(0);
	listaDibujables.push_back(nuevo);*/
}

void mySocketSrv::bindear(){
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		cout << "ERROR on binding" << endl;
		loggearInterno( " ERROR ON BINDING " );
		exit(0);
	}
}

void mySocketSrv::escuchar(){
	listen(sockfd,6);
	loggearInterno( " INICIO DE ACTIVIDAD DEL SERVIDOR");
	cout << "Escuchando cliente...\n" << endl;
}

void mySocketSrv::aceptarClientes(){
	bool activo = true;
	socklen_t clilen;
	while (activo) {
		int newsockfd;
		sockaddr cli_addr;
		clilen = sizeof(cli_addr);
		pthread_t thread;//CREO UN THREAD

		newsockfd = accept(sockfd, &cli_addr, &clilen);
		if (newsockfd < 0){
			cout << "ERROR on accept" << endl;
			loggearInterno( " ERROR ON ACCEPT");
		}else {
			pthread_create(&thread, NULL, atender_cliente, (void*)(long)newsockfd);
			/*printf("Conectado!\n");
			printf("Leyendo mensaje...\n");*/
		}

	} /* end of while */
}

void mySocketSrv::loggearInterno(string mensaje){
	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);
	char buffTime[20];
	struct tm *sTm;

	time_t now = time (0);
	sTm = gmtime (&now);
	strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S", sTm);

	archLog << buffTime << " MENSAJE INTERNO : " << mensaje << endl;

	archLog.close();
}

void mySocketSrv::cerrar(){
	close(this->sockfd);
	loggearInterno( " SE CERRO EL SOCKET");
}

mySocketSrv::~mySocketSrv(){}
