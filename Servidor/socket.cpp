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
//char* archivoXml;
string archivoXml;

list<mensajeClass> listaDeMensajes;
list<usuarioClass> listaDeUsuarios;
int xMin;
int xMax;

//esto se lee del xml, en caso que no haya nada es 2 por defecto.
int cantidadJugadores = 2;

int topeSalto = 20;

bool avanzar;

int camaraX = 0;
int camaraSet = 0;

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
	bool errorSocket = false;
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesRecibidos = read(sockfd, buffer, tamanio);
		if ((bytesRecibidos < 0) && (errorSocket == false)){
			cout << "Error al recibir datos SOCKET" << endl;
			loggearServer( " ERROR AL RECIBIR DATOS SOCKET");
			errorSocket = true;
		} else {
			acumulador += bytesRecibidos;
		}
	}
}

bool enviarMensaje(int sockfd, void* mensaje, int tamanioMensaje){
	int bytesEnviados = 0;
	bool errorSocket = false;

	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
		if(n < 0){
			errorSocket = true;
		} else {
			bytesEnviados += n;
		}
	}
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
	while (capa1){
		xml_node<> *fondoXML = capa1->first_node("imagen_fondo");
		xml_node<> *anchoFondo = fondoXML->next_sibling("ancho");
		xml_node<> *zFondo = anchoFondo->next_sibling("z-index");

		FondoServer nuevo;
		nuevo.setAncho(atoi(anchoFondo->value()));
		nuevo.setSpriteId(fondoXML->value());
		nuevo.setZindex(atoi(zFondo->value()));
		listaFondos.push_back(nuevo);

		capa1 = capa1->next_sibling();
	}


}

char* parseXMLPj(){
	const char *cstr = archivoXml.c_str();
	file<> xmlFile(cstr);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data()); 

	xml_node<> *sprites = doc.first_node("sprites");
	xml_node<> *pj = sprites->first_node("personaje");
	xml_node<> *path = pj->first_node("path");

	return path->value();

}

void buscarNombreUsuario(char *nombreRetorno, int numeroUsuario){
	list<usuarioClass>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(*it).nombreUsuario().c_str());
}

void agregaraLista(int numeroCliente, int usrDest, int x, int y, int spx, int spy, char flip, bool avanzar){
	char nombreAutor[50];
	char nombreDestino[50];
	buscarNombreUsuario(nombreAutor, numeroCliente);
	buscarNombreUsuario(nombreDestino, usrDest);

	mensajeClass mensajeObj(nombreAutor, nombreDestino, numeroCliente, x, y, spx, spy, flip, avanzar);

	int a = pthread_mutex_trylock(&mutexLista);

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

void enviarAConectados(int numeroCliente, int nuevaCordX, int nuevaCordY, int nuevoSpX, int nuevoSpY, char flip, bool avanzar){
	//envio a todos los q esten online el mensaje de q se modifico un objeto
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			agregaraLista(numeroCliente, (*it).numCliente(), nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar);
		}
	}
}

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	//char buffer[TAM_MAX];
	int newsockfd = (long)arg;
	bool abierto = true;
	bool inicioGrafica = false;
	int numeroCliente;
	usuarioClass* userPoint= NULL;


	while (abierto){

		char codigo;

		if(inicioGrafica){
			struct timeval tv;
			tv.tv_sec = 10;  // 10 Secs Timeout
			tv.tv_usec = 0;  // Not init'ing this can cause strange errors
			setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

			int data = read(newsockfd, &codigo, sizeof(char));
			if(data < 0){
				cout << "Se cayo la conexion con el cliente " << endl;

				/*
				abierto = false;

				list<usuarioClass>::iterator it = listaDeUsuarios.begin();
				advance(it, numeroCliente-1);
				it->desconectar();*/

				//caso cerrar ventana grafica
				int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
				char flip;

				list<DibujableServer>::iterator it = listaDibujables.begin();
				advance(it, numeroCliente-1);
				//it->quieto();

				//it->estaConectado = false;
				nuevaCordX = it->x;
				nuevaCordY = it->y;
				//momia
				nuevoSpX = it->spX = 1;
				nuevoSpY = it->spY = 1;
				flip = it->flip;
				it->desconectar();

				list<usuarioClass>::iterator it2 = listaDeUsuarios.begin();
				advance(it2, numeroCliente-1);

				//ver si conviene desconectar al usuario desde aca..
				it2->desconectar();

				//envio a todos los q esten online el mensaje de q se modifico un objeto
				enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false);

				//envio un msj a todos los q esten online q se desconecto el usuario
				string mensaje = (*it2).nombreUsuario() + " Se ah desconectado";
				enviarMensajeAConectados(mensaje);
			}
		} else {

			int data = read(newsockfd, &codigo, sizeof(char));
		}

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
						i->conectar();
						numeroCliente = i->numCliente();

						//mandar msj a todos q volvio asi no esta mas gris
						int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
						char flip;

						list<DibujableServer>::iterator it = listaDibujables.begin();
						advance(it, numeroCliente-1);

						nuevaCordY = it->y;
						nuevaCordX = it->x;
						//parado
						nuevoSpX = it->spX = 0;
						nuevoSpY = it->spY = 1;
						flip = it->flip;

						it->conectar();

						//envio a todos los q esten online el mensaje de q se modifico un objeto
						enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false);
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
				char* spriteId = parseXMLPj();
				nuevo.setSpriteId(spriteId);
				nuevo.setX(1 + rand() % (150));
				nuevo.setY(ALTO_VENTANA - 100);
				nuevo.setSpX (0);
				nuevo.setSpY(1);
				listaDibujables.push_back(nuevo);
			}else if(estabaDesconectado){
				strcpy(mensaje,"Bienvenido nuevamente");

				/*list<DibujableServer>::iterator it2 = listaDibujables.begin();
				advance(it2, numeroCliente-1);
				it2->conectar();*/

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
			//cargarFondos(archivoXml);
			enviarMensaje(newsockfd, &ANCHO_VENTANA, sizeof(int));
			enviarMensaje(newsockfd, &ALTO_VENTANA, sizeof(int));

			break;
		}
		case '4':{
			//cout << "Entro a /4 que es fondos" << endl;
			//cargarFondos(archivoXml);
			inicioGrafica = true;
			int tamano = listaFondos.size();
			enviarMensaje(newsockfd, &tamano, sizeof(int));
			for(list<FondoServer>::iterator i =listaFondos.begin(); i != listaFondos.end();++i){
				int ancho = (*i).ancho;
				int z = (*i).zindex;
				int tamFondoId = (*i).spriteId.size();
				cout << "tamFondoId: " << tamFondoId << endl;
				char spriteId[tamFondoId];
				strcpy(spriteId, (*i).spriteId.c_str());

				cout << "spriteId: " << spriteId << endl;

				enviarMensaje(newsockfd, &tamFondoId, sizeof(int));
				enviarMensaje(newsockfd, &spriteId, sizeof(char)*tamFondoId);
				enviarMensaje(newsockfd, &ancho, sizeof(int));
				//enviarMensaje(newsockfd, &z, sizeof(int));
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

					//cout << "TIPO MSJ: " << tipoMensaje << endl;

					switch(tipoMensaje){
					case 1:{
						//envio de grafica
						int xCord = (*i).getX();
						int yCord = (*i).getY();
						int spX = (*i).getSpX();
						int spY = (*i).getSpY();
						int idObjeto = (*i).getidObjeto();
						bool puedeAvanzar = (*i).getBoolAvanzar();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &spX, sizeof(int));
						enviarMensaje(newsockfd, &spY, sizeof(int));
						enviarMensaje(newsockfd, &(*i).flip, sizeof(char));
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
					default:
						break;

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

		case '8': {
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;

			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				//empiezo de nuevo
				nuevaCordY = it->y = ALTO_VENTANA - 100;
				nuevaCordX = it->x = 100;
				nuevoSpX = it->spX; //le dejo el mismo sprite.. sino poner if estaConectado
				nuevoSpY = it->spY;
				flip = 'D';

				enviarAConectados(it->id,nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar);
			}

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
			int tamSpriteId = (*it).spriteId.size();

			cout << "tamSpriteId: " << tamSpriteId << endl;
			char spriteId[tamSpriteId];
			enviarMensaje(newsockfd,&tamSpriteId,sizeof(int));
			strcpy(spriteId, (*it).spriteId.c_str());

			cout << "spriteId: " << spriteId << endl;

			enviarMensaje(newsockfd,spriteId,tamSpriteId);

			//posicion x
			enviarMensaje(newsockfd,&(*it).x,sizeof(int));

			//posicion y
			enviarMensaje(newsockfd,&(*it).y,sizeof(int));

			enviarMensaje(newsockfd,&(*it).spX,sizeof(int));

			enviarMensaje(newsockfd,&(*it).spY,sizeof(int));

			//flip
			enviarMensaje(newsockfd,&(*it).flip,sizeof(char));

			break;
		}
		case 'c':{
			//envio X camara
			enviarMensaje(newsockfd,&camaraX,sizeof(int));
			//envio cameraSet
			enviarMensaje(newsockfd,&camaraSet,sizeof(int));

			break;
		}

		case 'M':{
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			bool seMovio = it->mover();
			//enviar si realmente hay cambios..
			if (seMovio) {
				if (it->getX() < xMin) xMin = it->getX();
				enviarAConectados(numeroCliente, it->x, it->y, it->spX, it->spY, it->flip, avanzar);

				//itero desconectados
				for (list<DibujableServer>::iterator it2 = listaDibujables.begin(); it2 != listaDibujables.end(); ++it2) {
					if (!it2->estaConectado()){
						if (it2->x <= camaraX){
							it2->x = camaraX;//adelanto a la momia
							enviarAConectados(it2->id , it2->x, it2->y, it2->spX, it2->spY, it2->flip, false);
						}
					}
				}
			}

			break;
		}

		case 'U':{
			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			it->saltar();

			break;
		}

		case 'L':{
			int xCamara;
			recibirMensaje(newsockfd, &xCamara, sizeof(int));
			camaraX = xCamara;

			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			//if ((it->x) > (xCamara)){
			if ((it->x) > (camaraX)){
				it->caminarIzquierda();
			}else it->quieto();
			//ver si realmente hace falta dejarlo quieto o si saco el else va igual....

			break;
		}
		case 'R':{
			int totalDesplazar;

			int xCamara;
			recibirMensaje(newsockfd, &xCamara, sizeof(int));
			camaraX = xCamara;

			xMin = 10000;
			xMax = 1;
			bool hayDesconectados = false;

			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if(it->estaConectado()){
					if((*it).x < xMin){
						xMin = it->x;
					}
					if((*it).x > xMax){
						xMax = it->x;
					}
				}
			}

			totalDesplazar = ((xMin) + ANCHO_VENTANA/2); //de aca se regula hasta donde llega el sprite en la pantalla;

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			if ((it->x) < (totalDesplazar)){
				it->caminarDerecha();
			}else it->quieto();

			avanzar = false;

			int delta;
			delta = xMax - xMin;
			if(delta < ANCHO_VENTANA/2){
				avanzar = true;
				if (it->x > camaraSet)
					camaraSet = it->x;
			}

			cout <<"camaraSet: " << camaraSet << endl;
			cout <<"XMIN: " << xMin << endl;
			cout <<"XMAX: " << xMax << endl;
			cout <<"totalDesplazar: " << totalDesplazar << endl;

			break;
		}
		case 'C':{
			//caso cerrar ventana grafica
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			nuevaCordX = it->x;
			nuevaCordY = it->y;
			//momia
			nuevoSpX = it->spX = 1;
			nuevoSpY = it->spY = 1;
			flip = it->flip;
			it->desconectar();

			list<usuarioClass>::iterator it2 = listaDeUsuarios.begin();
			advance(it2, numeroCliente-1);
			it2->desconectar();

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false);

			//envio un msj a todos los q esten online q se desconecto el usuario
			string mensaje = (*it2).nombreUsuario() + " Se ah desconectado";
			enviarMensajeAConectados(mensaje);

			break;
		}
		case 'S':{
			//caso se queda parado
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			it->quieto();

			//sprite parado
			nuevaCordX = it->x;
			nuevaCordY = it->y;
			nuevoSpX = it->spX;
			nuevoSpY = it->spY;
			flip = it->flip;

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false);

			break;
		}

		case 'i':{
			//envio a conectados q cierren grafica
			for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
				if((*it).estaConectado()){
					char nombreDestino[50];
					buscarNombreUsuario(nombreDestino, (*it).numCliente());

					mensajeClass mensajeObj(4, nombreDestino);
					int a = pthread_mutex_trylock(&mutexLista);
					listaDeMensajes.push_back(mensajeObj);
					pthread_mutex_unlock (&mutexLista);
				}
			}

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;

			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				list<DibujableServer>::iterator it = listaDibujables.begin();
				advance(it, i-1);

				DibujableServer nuevo;
				char* spriteId = parseXMLPj();
				it->setSpriteId(spriteId);
				it->setX(1+rand() % (150));
				it->setY(ALTO_VENTANA-100);
				//me saca a la momia si esta desconectado..
				//it->setSpX(0);
				//it->setSpY(1);
			}

			//envio a conectados q abran grafica
			for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
				if((*it).estaConectado()){
					char nombreDestino[50];
					buscarNombreUsuario(nombreDestino, (*it).numCliente());

					mensajeClass mensajeObj(5, nombreDestino);
					int a = pthread_mutex_trylock(&mutexLista);
					listaDeMensajes.push_back(mensajeObj);
					pthread_mutex_unlock (&mutexLista);
				}
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

mySocketSrv :: mySocketSrv(char* puerto, string xml){
	//cargo fondos
	archivoXml = xml;
	cout << "ARCHIVO XML: " << archivoXml << endl;
	char* s2;
	s2 = (char *)alloca(xml.size() + 1);
	memcpy(s2, xml.c_str(), xml.size() + 1);

	cargarFondos(s2);
	cantidadDeJugadores(s2);

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
