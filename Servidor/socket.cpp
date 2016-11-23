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
#include "bala.h"
#include "Bonus.h"
#include "ContenedorEnemigos.h"
#include "ContenedorBalas.h"
#include "ContenedorBonus.h"
#include "DibujableServerEnemigo.h"

#define DEBUG 2

using namespace rapidxml;
using namespace std;

list<int> listaFDClientes;
list<DibujableServer> listaDibujables;
list<DibujableServer> listaEnergias;
list<FondoServer> listaFondos;
int ANCHO_VENTANA;
int ALTO_VENTANA;
int VELOCIDAD_JUGADOR;
//char* archivoXml;
string archivoXml;

list<mensajeClass> listaDeMensajes;
list<usuarioClass> listaDeUsuarios;

//esto se lee del xml, en caso que no haya nada es 2 por defecto.
int cantidadJugadores = 2;
int cantidadJugadoresConectados = 0;
int modoJuego = 1; //default individual multijugador
int modoPrueba = 0; //default modo de prueba OFF

int topeSalto = 20;
bool avanzar;

int camaraX = 0;
int camaraSet = 0;
bool jefePresente = false;

ContenedorEnemigos contenedorEnemigos;
ContenedorBalas contenedorBalas;
ContenedorBonus contenedorBonus;

//list<mensajeClass> listaDeMensajesAlCliente;
pthread_mutex_t mutexLista = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaUsuarios = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaDibujables = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaBalas = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexContenedorEnemigos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexContenedorBalas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexContenedorBonus = PTHREAD_MUTEX_INITIALIZER;

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
	/*int acumulador = 0, bytesRecibidos = 0;
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
	}*/
	int i = read(sockfd, buffer, tamanio);

	//char* tmp = (char*)buffer;
	//int* tmp = (int*)buffer;
	//cout << "RECIBE: " << (*tmp) << endl;
}

bool enviarMensaje(int sockfd, void* mensaje, int tamanioMensaje){
	/*
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
	 */
	bool errorSocket = false;
	int n = write(sockfd, mensaje, tamanioMensaje);
	if(n < 0) errorSocket = true;

	//char* tmp = (char*)mensaje;
	//	int* tmp = (int*)mensaje;
	//	cout << "ENVIA: " << (*tmp) << endl;

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

void modoDeJuego(char* xml){
	file<> xmlFile(xml);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags

	xml_node<> *juego = doc.first_node("juego");
	xml_node<> *modo = juego->first_node("modo_de_juego");
	if (modo != NULL){
		modoJuego = atoi(modo->value());
		xml_node<> *prueba = modo->next_sibling("prueba");
		if (prueba != NULL){
			modoPrueba = atoi(prueba->value());
		}
	}



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

void cargarBonus(char* xml){
	file<> xmlFile(xml);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags

	xml_node<> *bonus = doc.first_node("bonus");
	xml_node<> *path = bonus->first_node("path");
	int sum = 0;
	for (int i = 0; i < 20; ++i)
	{
		//7 tipos de bonus:
		//0- recargar arma 1
		//1- pasar a arma 2
		//2- recargar arma 2
		//3- pasar a arma 3
		//4- recargar arma 3
		//5- bonus de vida
		//6- bonus de killall
		int tipoBonus = rand() % 7;
		int x = sum + 800;
		sum = x;
		int y = ALTO_VENTANA - 60;

		int a = pthread_mutex_trylock(&mutexContenedorBonus);
		contenedorBonus.nuevoBonus(x,y,tipoBonus);
		pthread_mutex_unlock (&mutexContenedorBonus);
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
	int a = pthread_mutex_trylock(&mutexListaUsuarios);

	list<usuarioClass>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(*it).nombreUsuario().c_str());

	pthread_mutex_unlock (&mutexListaUsuarios);
}

void agregaraLista(int numeroCliente, int usrDest, int x, int y, int spx, int spy, char flip, bool avanzar, int tipo){
	char nombreAutor[50];
	char nombreDestino[50];
	buscarNombreUsuario(nombreAutor, numeroCliente);
	buscarNombreUsuario(nombreDestino, usrDest);

	mensajeClass mensajeObj(nombreAutor, nombreDestino, numeroCliente, x, y, spx, spy, flip, avanzar, tipo);

	int a = pthread_mutex_trylock(&mutexLista);

	listaDeMensajes.push_back(mensajeObj);
	//loggear(mensajeObj.nombreAutor(),mensajeObj.nombreDestinatario(),mensajeObj.getMensaje());
	pthread_mutex_unlock (&mutexLista);
}

void enviarMensajeAConectados(string mensaje){
	//envio a todos los q esten online el mensaje
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
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
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void enviarAConectados(int numeroCliente, int nuevaCordX, int nuevaCordY, int nuevoSpX, int nuevoSpY, char flip, bool avanzar, int tipo){
	//envio a todos los q esten online el mensaje de q se modifico un objeto
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		
		if((*it).estaConectado()){
			agregaraLista(numeroCliente, (*it).numCliente(), nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar, tipo);
		}

	}
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void quitarEnemigo(int numeroCliente, int usrDest, int x, int y, int spx, int spy, char flip){
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			char nombreAutor[50];
			char nombreDestino[50];
			buscarNombreUsuario(nombreAutor, numeroCliente);
			buscarNombreUsuario(nombreDestino, (*it).numCliente());

			mensajeClass mensajeObj(nombreAutor, nombreDestino, numeroCliente, x, y, spx, spy, flip, 2);

			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void enviarBalasAConectados(int idBala, int x, int y, int direccion, int tipoBala){
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			char nombreAutor[50];
			char nombreDestino[50];
			buscarNombreUsuario(nombreAutor, idBala);
			buscarNombreUsuario(nombreDestino, (*it).numCliente());

			mensajeClass mensajeObj(nombreAutor, nombreDestino, idBala, x, y, direccion, tipoBala, 'D', 6);

			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void enviarBonusAConectados(int idBonus, int x, int y, int tipoBonus){
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			char nombreAutor[50];
			char nombreDestino[50];
			buscarNombreUsuario(nombreAutor, idBonus);
			buscarNombreUsuario(nombreDestino, (*it).numCliente());
			mensajeClass mensajeObj(nombreAutor, nombreDestino, idBonus, x, y, 0, tipoBonus, 'D', 9);

			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void quitarBalas(int idBala){
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			char nombreAutor[50];
			char nombreDestino[50];
			buscarNombreUsuario(nombreAutor, idBala);
			buscarNombreUsuario(nombreDestino, (*it).numCliente());

			mensajeClass mensajeObj(nombreAutor, nombreDestino, idBala, 0, 0, 0, 0, 'D', 8);

			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
	pthread_mutex_unlock (&mutexListaUsuarios);
}

void quitarBonus(int idBonus){
	int a = pthread_mutex_trylock(&mutexListaUsuarios);
	for (list<usuarioClass>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((*it).estaConectado()){
			char nombreAutor[50];
			char nombreDestino[50];
			buscarNombreUsuario(nombreAutor, idBonus);
			buscarNombreUsuario(nombreDestino, (*it).numCliente());

			mensajeClass mensajeObj(nombreAutor, nombreDestino, idBonus, 0, 0, 0, 0, 'D', 10);

			int a = pthread_mutex_trylock(&mutexLista);
			listaDeMensajes.push_back(mensajeObj);
			pthread_mutex_unlock (&mutexLista);
		}
	}
	pthread_mutex_unlock (&mutexListaUsuarios);
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
				abierto = false;

				//abierto = false;

				//list<usuarioClass>::iterator it = listaDeUsuarios.begin();
				//advance(it, numeroCliente-1);
				//it->desconectar();

				//caso cerrar ventana grafica
				int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
				char flip;

				int b = pthread_mutex_trylock(&mutexListaDibujables);
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
				pthread_mutex_unlock (&mutexListaDibujables);
				int a = pthread_mutex_trylock(&mutexListaUsuarios);
				list<usuarioClass>::iterator it2 = listaDeUsuarios.begin();
				advance(it2, numeroCliente-1);

				//ver si conviene desconectar al usuario desde aca..
				it2->desconectar();

				//envio a todos los q esten online el mensaje de q se modifico un objeto
				enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false,0);

				//envio un msj a todos los q esten online q se desconecto el usuario
				string mensaje = (*it2).nombreUsuario() + " Se ha desconectado";
				enviarMensajeAConectados(mensaje);
				pthread_mutex_unlock (&mutexListaUsuarios);
			}
		} else {
			int data = read(newsockfd, &codigo, sizeof(char));
		}

		//int data = read(newsockfd, &codigo, sizeof(char));
		//		cout << "RECIBE: " << codigo << endl;

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
			int a = pthread_mutex_trylock(&mutexListaUsuarios);
			for (list<usuarioClass>::iterator i = listaDeUsuarios.begin(); i != listaDeUsuarios.end(); ++i) {
				
				if ((*i).nombreUsuario().compare(nombre) == 0){
					if(!(*i).estaConectado()){
						estabaDesconectado = true;
						i->conectar();
						numeroCliente = i->numCliente();

						//mandar msj a todos q volvio asi no esta mas gris
						int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
						char flip;
						int b = pthread_mutex_trylock(&mutexListaDibujables);
						list<DibujableServer>::iterator it = listaDibujables.begin();
						advance(it, numeroCliente-1);

						nuevaCordY = it->y;
						nuevaCordX = it->x;
						//parado
						nuevoSpX = it->spX = 0;
						nuevoSpY = it->spY = 1;
						flip = it->flip;

						it->conectar();
						pthread_mutex_unlock (&mutexListaDibujables);
						//envio a todos los q esten online el mensaje de q se modifico un objeto
						enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false,0);
					}

					respuesta = false;
					strcpy(mensaje,"El nombre ya esta en uso");
				}
			}
			pthread_mutex_unlock (&mutexListaUsuarios);

			if (respuesta){
				strcpy(mensaje,"Bienvenido");
				int a = pthread_mutex_trylock(&mutexListaUsuarios);
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

				//creo dibujable energia
				DibujableServer nuevo2;
				nuevo2.setId(listaDeUsuarios.size());
				pthread_mutex_unlock (&mutexListaUsuarios);
				//string energiaId = "1barra.png";
				//nuevo2.setSpriteId(energiaId);
				nuevo2.setSpY(0);

				int b = pthread_mutex_trylock(&mutexListaDibujables);
				listaDibujables.push_back(nuevo);
				listaEnergias.push_back(nuevo2);
				pthread_mutex_unlock (&mutexListaDibujables);

				//AGREGO UN ENEMIGO PARA PROBAR AL PPIO
				//NO VAN ACA SINO X CADA UNO Q INICIA SE CARGAN!!!!!!!!
				//camina solo 3
				//tira tiros 2
				//ataque salto 1
				//cagon 4

				//HACER METODO contenedorEnemigos.cargarEnemigosNivel1();
				int aa = pthread_mutex_trylock(&mutexContenedorEnemigos);

				contenedorEnemigos.nuevoEnemigo(750, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(800, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(1000, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(1200, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(1400, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(1650, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(1800, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(1850, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(2000, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(2250, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(2500, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(2700, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(3000, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(3300, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(3700, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(4000, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(4050, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(4200, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(4400, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(4600, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(4650, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(4800, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(4950, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(5000, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(5250, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(5300, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(5400, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(5600, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(5800, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(6000, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(6200, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(6250, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(6400, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(6500, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(6700, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(6850, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(7000, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(7150, ALTO_VENTANA - 100, 3);
				contenedorEnemigos.nuevoEnemigo(7200, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(7350, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(7400, ALTO_VENTANA - 100, 2);
				contenedorEnemigos.nuevoEnemigo(7500, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(7600, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(7800, ALTO_VENTANA - 100, 1);
				contenedorEnemigos.nuevoEnemigo(7900, ALTO_VENTANA - 100, 4);
				contenedorEnemigos.nuevoEnemigo(8000, ALTO_VENTANA - 100, 2);

				pthread_mutex_unlock (&mutexContenedorEnemigos);

			}else if(estabaDesconectado){
				strcpy(mensaje,"Bienvenido nuevamente");

				/*list<DibujableServer>::iterator it2 = listaDibujables.begin();
					advance(it2, numeroCliente-1);
					it2->conectar();*/
				pthread_mutex_unlock (&mutexListaUsuarios);
				respuesta = true;
			} else {pthread_mutex_unlock (&mutexListaUsuarios);}

			responderLogin(newsockfd,respuesta,mensaje, numeroCliente);

			break;
		}
		case '2':{
			//cout << "Entro a /2 que es desconectar" << endl;
			abierto = false;
			int a = pthread_mutex_trylock(&mutexListaUsuarios);
			list<usuarioClass>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			it->desconectar();
			pthread_mutex_unlock (&mutexListaUsuarios);
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
				int tamFondoId = (*i).spriteId.length() + 1;
				//cout << "tamFondoId: " << tamFondoId << endl;
				char spriteId[tamFondoId];
				strcpy(spriteId, (*i).spriteId.c_str());

				//cout << "spriteId: " << spriteId << endl;

				enviarMensaje(newsockfd, &tamFondoId, sizeof(int));
				enviarMensaje(newsockfd, &spriteId, sizeof(char)*tamFondoId);
				enviarMensaje(newsockfd, &ancho, sizeof(int));
				//enviarMensaje(newsockfd, &z, sizeof(int));
			}
			break;
		}
		case '5': {
			char nombre[50];
			buscarNombreUsuario(nombre, numeroCliente);

			int a = pthread_mutex_trylock(&mutexLista);
			for (list<mensajeClass>::iterator i = listaDeMensajes.begin(); i != listaDeMensajes.end(); ++i) {
				if ((*i).nombreDestinatario().compare(nombre) == 0 ){
					int corte = 1;
					enviarMensaje(newsockfd, &corte, sizeof(int));

					//envio tipo de mensaje
					int tipoMensaje = (*i).getTipoMensaje();
					enviarMensaje(newsockfd, &tipoMensaje, sizeof(int));

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
						enviarMensaje(newsockfd, &i->tipoDibujable, sizeof(int));

						enviarMensaje(newsockfd, &camaraX, sizeof(int));

						break;
					}
					case 2:{
						//envio de bajas de enemigos
						int idObjeto = (*i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}
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
					case 6:{
						//envio de balas
						int xCord = (*i).getX();
						int yCord = (*i).getY();
						int direccion = (*i).getSpX();
						int tipo = (*i).getSpY();
						int idObjeto = (*i).getidObjeto();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &direccion, sizeof(int));
						enviarMensaje(newsockfd, &tipo, sizeof(int));

						break;
					}

					case 9:{
						int xCord = (*i).getX();
						int yCord = (*i).getY();
						int idObjeto = (*i).getidObjeto();
						int tipo = (*i).getSpY();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &tipo, sizeof(int));

						break;
					}

					case 10:{
						//envio baja de bonus
						int idObjeto = (*i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}

					case 8:{
						//envio bajas de balas
						int idObjeto = (*i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}

					default:
						break;

					}

					i = listaDeMensajes.erase(i);
					i--;
				}
				pthread_mutex_unlock (&mutexLista);
			}

			int tamanioMensaje = 0;
			enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));

			if(numeroCliente == 1){

			//BUSCO ENEMIGOS
			list<DibujableServerEnemigo> listaEnemigosActivos;
			list<DibujableServerEnemigo> listaEnemigosDeBaja;

			int aa = pthread_mutex_trylock(&mutexContenedorEnemigos);
			int bb = pthread_mutex_trylock(&mutexContenedorBalas);

			contenedorEnemigos.buscarActivos(camaraX,&listaEnemigosActivos, &listaEnemigosDeBaja, &contenedorBalas);

			pthread_mutex_unlock (&mutexContenedorBalas);
			pthread_mutex_unlock (&mutexContenedorEnemigos);

			//BUSCO BALAS
			list<bala> listaBalasActivas;
			list<bala> listaBalasDeBaja;

			int bbb = pthread_mutex_trylock(&mutexContenedorBalas);
			contenedorBalas.buscarActivas(camaraX, &listaBalasActivas, &listaBalasDeBaja);

			list<DibujableServerEnemigo> listaEnemigosDisparados;
			contenedorBalas.detectarColisiones(&listaBalasDeBaja, &listaEnemigosActivos, &listaEnemigosDisparados);
			pthread_mutex_unlock (&mutexContenedorBalas);

			int aaa = pthread_mutex_trylock(&mutexContenedorEnemigos);
			contenedorEnemigos.matarEnemigos(camaraX, listaEnemigosDisparados);
			pthread_mutex_unlock (&mutexContenedorEnemigos);

			list<Bonus> listaBonusActivos;
			list<Bonus> listaBonusDeBaja;
			
			int a = pthread_mutex_trylock(&mutexContenedorBonus);
			contenedorBonus.buscarActivos(camaraX, &listaBonusActivos, &listaBonusDeBaja);
			pthread_mutex_unlock (&mutexContenedorBonus);



			//list<Bonus> listaBonusAgarrados;
			int zzz = pthread_mutex_trylock(&mutexListaDibujables);
			contenedorBonus.detectarColision(&listaBonusDeBaja, &listaBonusActivos, &listaDibujables);
			pthread_mutex_unlock (&mutexListaDibujables);

			for (list<bala>::iterator itBalas = listaBalasActivas.begin(); itBalas != listaBalasActivas.end(); ++itBalas) {
				enviarBalasAConectados(itBalas->id,itBalas->x,itBalas->y,itBalas->direccionDisparo, itBalas->tipoBala);
			}

			for (list<bala>::iterator itBalas = listaBalasDeBaja.begin(); itBalas != listaBalasDeBaja.end(); ++itBalas) {
				quitarBalas(itBalas->id);
			}

			for (list<DibujableServerEnemigo>::iterator itEnemigos = listaEnemigosActivos.begin(); itEnemigos != listaEnemigosActivos.end(); ++itEnemigos) {
				enviarAConectados(itEnemigos->id , itEnemigos->x, itEnemigos->y, itEnemigos->spX, itEnemigos->spY, itEnemigos->flip, true, itEnemigos->tipoEnemigo);
			}

			for (list<DibujableServerEnemigo>::iterator itEnemigos = listaEnemigosDeBaja.begin(); itEnemigos != listaEnemigosDeBaja.end(); ++itEnemigos) {
				quitarEnemigo(itEnemigos->id , itEnemigos->x, itEnemigos->y, itEnemigos->spX, itEnemigos->spY, itEnemigos->flip, false);
			}
			//se puede hacer lo mismo con los tiros

			for (list<Bonus>::iterator itBonus = listaBonusActivos.begin(); itBonus != listaBonusActivos.end(); ++itBonus) {
				enviarBonusAConectados(itBonus->getId(), itBonus->getPosX(), itBonus->getPosY(), itBonus->getTipoBonus());
			}

			for (list<Bonus>::iterator itBonus = listaBonusDeBaja.begin(); itBonus != listaBonusDeBaja.end(); ++itBonus) {
				quitarBonus(itBonus->getId());
			}

			}

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
			int respuesta = 1;
			int a = pthread_mutex_trylock(&mutexListaUsuarios);
			if (listaDeUsuarios.size() < cantidadJugadores){
				respuesta = 0;
			} else if (listaDeUsuarios.size() > cantidadJugadores){
				respuesta = -1;
				list<usuarioClass>::iterator it = listaDeUsuarios.begin();
				advance(it, numeroCliente-1);
				listaDeUsuarios.erase(it);
				
				int b = pthread_mutex_trylock(&mutexListaDibujables);
				list<DibujableServer>::iterator it2 = listaDibujables.begin();
				advance(it2, numeroCliente-1);
				listaDibujables.erase(it2);
				pthread_mutex_unlock (&mutexListaDibujables);

			}
			pthread_mutex_unlock (&mutexListaUsuarios);
			enviarMensaje(newsockfd,&respuesta,sizeof(int));

			break;
		}

		case '8': {
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;
			int b = pthread_mutex_trylock(&mutexListaDibujables);
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				//empiezo de nuevo
				nuevaCordY = it->y = ALTO_VENTANA - 100;
				nuevaCordX = it->x = 100;
				nuevoSpX = it->spX; //le dejo el mismo sprite.. sino poner if estaConectado
				nuevoSpY = it->spY;
				flip = 'D';

				enviarAConectados(it->id,nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar,0);
			}
			pthread_mutex_unlock (&mutexListaDibujables);
			break;
		}

		case '9':{
			//caso envio objetos dibujables
			int numeroUsuario;
			recibirMensaje(newsockfd, &numeroUsuario, sizeof(int));
			int b = pthread_mutex_trylock(&mutexListaDibujables);
			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroUsuario-1);

			//idObjeto
			enviarMensaje(newsockfd,&it->id,sizeof(int));

			//spriteId
			int tamSpriteId = it->spriteId.length() + 1;
			//cout << "tamSpriteId: " << tamSpriteId << endl;
			enviarMensaje(newsockfd,&tamSpriteId,sizeof(int));

			char spriteId[tamSpriteId];
			strcpy(spriteId, it->spriteId.c_str());
			//cout << "spriteId: " << spriteId << endl;
			enviarMensaje(newsockfd,spriteId,sizeof(char)*tamSpriteId);

			enviarMensaje(newsockfd,&(*it).x,sizeof(int));
			enviarMensaje(newsockfd,&(*it).y,sizeof(int));
			enviarMensaje(newsockfd,&(*it).spX,sizeof(int));
			enviarMensaje(newsockfd,&(*it).spY,sizeof(int));
			enviarMensaje(newsockfd,&(*it).flip,sizeof(char));
			pthread_mutex_unlock (&mutexListaDibujables);
			break;
		}

		case 'a':{
			int cant = listaEnergias.size();
			enviarMensaje(newsockfd, &cant, sizeof(int));
			int b = pthread_mutex_trylock(&mutexListaDibujables);
			for (list<DibujableServer>::iterator it = listaEnergias.begin(); it != listaEnergias.end(); ++it)
			{
				int id = (*it).getId();
				int spY = (*it).getSpY();
				//int tamaño spriteID
				enviarMensaje(newsockfd, &id, sizeof(int));
				enviarMensaje(newsockfd, &spY, sizeof(int));

			}
			pthread_mutex_unlock (&mutexListaDibujables);
			break;
		}

		case 'c':{
			//envio X camara
			enviarMensaje(newsockfd,&camaraX,sizeof(int));

			break;
		}

		case 'M':{
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			bool seMovio = it->mover(camaraX);

			//enviar si realmente hay cambios..
			if (seMovio) {
				VELOCIDAD_JUGADOR = it->velocidadXJugador();
				enviarAConectados(numeroCliente, it->x, it->y, it->spX, it->spY, it->flip, avanzar,0);

				//CALCULO DE LA CAMARA
				if(!jefePresente){
					int xMin, xMax;
					xMin = 10000;

					bool avanzarCamara = false;
					for (list<DibujableServer>::iterator i = listaDibujables.begin(); i != listaDibujables.end(); ++i) {
						if(i->estaConectado()){
							//si alguno supero la mitad de la pantalla avanzo camara
							if(i->x >= camaraX + (ANCHO_VENTANA/2)-80) avanzarCamara = true;
							if(i->x < xMin) xMin = i->x;
						}
					}

					pthread_mutex_unlock (&mutexListaDibujables); //esto capaz tarde mucho COMENTARIO

					if (camaraX >= 8075){  //quitar para hacer escenario infinito
						avanzarCamara = false;
						jefePresente = true;
						int aaa = pthread_mutex_trylock(&mutexContenedorEnemigos);
						contenedorEnemigos.iniciarJefe(camaraX);
						pthread_mutex_unlock (&mutexContenedorEnemigos);
					}

					if(avanzarCamara){
						if(xMin > camaraX){
							camaraX += VELOCIDAD_JUGADOR;
						}
					}

					int a = pthread_mutex_trylock(&mutexListaDibujables);
					//itero desconectados
					for (list<DibujableServer>::iterator it2 = listaDibujables.begin(); it2 != listaDibujables.end(); ++it2) {
						if (!it2->estaConectado()){
							if (it2->x <= camaraX){
								it2->x = camaraX;//adelanto a la momia
								enviarAConectados(it2->id , it2->x, it2->y, it2->spX, it2->spY, it2->flip, false,0);
							}
						}
					}
					pthread_mutex_unlock (&mutexListaDibujables);
				}
			}else pthread_mutex_unlock (&mutexListaDibujables); //esto capaz tarde mucho COMENTARIO

			break;
		}

		case 'U':{
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			it->saltar();

			pthread_mutex_unlock (&mutexListaDibujables);

			break;
		}

		case 'L':{
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if ((it->x) > (camaraX)){
				it->caminarIzquierda();
			}else it->quieto();

			pthread_mutex_unlock (&mutexListaDibujables);

			break;
		}
		case 'R':{
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			//camino hasta ANCHO_VENTANA (uso toda la pantalla)
			if ((it->x) < (camaraX + ANCHO_VENTANA)-80){ // - lo q ocupa el sprite en pantalla..
				it->caminarDerecha();
			}else it->quieto();

			if(camaraX >= 7800){

				avanzar = false;

				// hacer aparecer el BOOS
			}


			/* escenario infinito
				if(camaraX >= 8075){
					for (list<DibujableServer>::iterator it2 = listaDibujables.begin(); it2 != listaDibujables.end(); ++it2) {
						it2->x = it2->x - camaraX;
						enviarAConectados(it2->id , it2->x, it2->y, it2->spX, it2->spY, it2->flip, false);
					}
					camaraX = 0;
					camaraSet = 0;
					avanzar = false;
				}
			 */

			pthread_mutex_unlock (&mutexListaDibujables);

			break;
		}
		case 'C':{
			//caso cerrar ventana grafica
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			nuevaCordX = it->x;
			nuevaCordY = it->y;
			//momia
			nuevoSpX = it->spX = 1;
			nuevoSpY = it->spY = 1;
			flip = it->flip;
			it->desconectar();
			pthread_mutex_unlock (&mutexListaDibujables);

			int a = pthread_mutex_trylock(&mutexListaUsuarios);
			list<usuarioClass>::iterator it2 = listaDeUsuarios.begin();
			advance(it2, numeroCliente-1);
			it2->desconectar();

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false, 0);

			//envio un msj a todos los q esten online q se desconecto el usuario
			string mensaje = (*it2).nombreUsuario() + " Se ha desconectado";
			pthread_mutex_unlock (&mutexListaUsuarios);
			enviarMensajeAConectados(mensaje);
			inicioGrafica = false;

			break;
		}
		case 'S':{
			//caso se queda parado
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;
			int b = pthread_mutex_trylock(&mutexListaDibujables);

			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			it->quieto();


			//sprite parado
			nuevaCordX = it->x;
			nuevaCordY = it->y;
			nuevoSpX = it->spX;
			nuevoSpY = it->spY;
			flip = it->flip;
			pthread_mutex_unlock (&mutexListaDibujables);
			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false, 0);

			break;
		}

		case 'i':{
			//envio a conectados q cierren grafica
			int b = pthread_mutex_trylock(&mutexListaUsuarios);
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
			pthread_mutex_unlock (&mutexListaUsuarios);

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;

			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				int b = pthread_mutex_trylock(&mutexListaDibujables);

				list<DibujableServer>::iterator it = listaDibujables.begin();
				advance(it, i-1);

				//DibujableServer nuevo;
				char* spriteId = parseXMLPj();
				it->setSpriteId(spriteId);
				it->setX(1+rand() % (150));
				it->setY(ALTO_VENTANA-100);
				//me saca a la momia si esta desconectado..
				//it->setSpX(0);
				//it->setSpY(1);
				pthread_mutex_unlock (&mutexListaDibujables);

			}

			//envio a conectados q abran grafica
			int bb = pthread_mutex_trylock(&mutexListaUsuarios);
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
			pthread_mutex_unlock (&mutexListaUsuarios);
			break;
		}

		case 'd': {
			int direccionDisparo;
			recibirMensaje(newsockfd, &direccionDisparo, sizeof(int));

			//busco quien la disparo..
			int a = pthread_mutex_trylock(&mutexListaDibujables);
			list<DibujableServer>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			if( it->disparar() ){
				int x = it->getX();
				int y = it->getY();
				int usr = it->getId();
				char flip = it->flip;
				//int tipoDisparo = it->tipoDisparo();
				pthread_mutex_unlock (&mutexListaDibujables);

				if(direccionDisparo == -1){
					if(flip == 'D') direccionDisparo = 0;
					else direccionDisparo = 4;
				}

				int bb = pthread_mutex_trylock(&mutexContenedorBalas);

				contenedorBalas.nuevaBala(x+30, y+20, usr, direccionDisparo, 0); //pasar tipo de disparo..

				pthread_mutex_unlock (&mutexContenedorBalas);

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

mySocketSrv::mySocketSrv(char* puerto, string xml){
	//cargo fondos
	archivoXml = xml;
	cout << "ARCHIVO XML: " << archivoXml << endl;
	char* s2;
	s2 = (char *)alloca(xml.size() + 1);
	memcpy(s2, xml.c_str(), xml.size() + 1);

	cargarFondos(s2);
	cargarBonus(s2);
	modoDeJuego(s2);
	if (modoJuego != 3){
		cantidadDeJugadores(s2);
	} else cantidadJugadores = 4;


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
	while (activo && (cantidadJugadoresConectados<cantidadJugadores)) {
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
			listaFDClientes.push_back(newsockfd);
			cantidadJugadoresConectados++;
			cout << "TOTAL: " << cantidadJugadores << endl;
			cout << "CONECTADOS: " << cantidadJugadoresConectados << endl;
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

bool mySocketSrv::enviarMensaje(int sockfd, void* mensaje, int tamanioMensaje){

	bool errorSocket = false;
	int n = write(sockfd, mensaje, tamanioMensaje);
	if(n < 0) errorSocket = true;

	return errorSocket;
}

void mySocketSrv::cerrar(){

	int corte = 1;
	int tipoMsj = 7;
	for (list<int>::iterator it = listaFDClientes.begin(); it != listaFDClientes.end(); ++it){
		enviarMensaje((*it), &corte, sizeof(int));
		enviarMensaje((*it), &tipoMsj, sizeof(int));
	}

	close(this->sockfd);
	loggearInterno( " SE CERRO EL SOCKET");
}

mySocketSrv::~mySocketSrv(){}
