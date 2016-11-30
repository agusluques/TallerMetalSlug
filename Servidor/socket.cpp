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
#include "DibujableServerAdicional.h"
#include "Escenario.h"
#include <ctime>

#define DEBUG 2

using namespace rapidxml;
using namespace std;

list<int> listaFDClientes;
list<DibujableServer*> listaDibujables;
list<DibujableServer*> listaEnergias;
list<DibujableServerAdicional*> listaScores;
list<FondoServer*> listaFondos;
int ANCHO_VENTANA;
int ALTO_VENTANA;
int VELOCIDAD_JUGADOR;
//char* archivoXml;
string archivoXml;

list<mensajeClass*> listaDeMensajes;
list<usuarioClass*> listaDeUsuarios;

//esto se lee del xml, en caso que no haya nada es 2 por defecto.
int cantidadJugadores = 2;
int cantidadJugadoresConectados = 0;
int modoJuego = 1; //default individual multijugador
//int modoPrueba = 0; //default modo de prueba OFF
int nivelActual = 1;

int topeSalto = 20;
bool avanzar;
bool modoDios = false;
bool pasarDeNivel = false;

int camaraX = 0;
int camaraSet = 0;
bool jefePresente = false;
bool estaEnPantallaScore = false;

ContenedorEnemigos contenedorEnemigos;
ContenedorBalas contenedorBalas;
ContenedorBonus contenedorBonus;
Escenario escenario;

time_t tiempoEsperaScore;

//list<mensajeClass> listaDeMensajesAlCliente;
pthread_mutex_t mutexLista = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaUsuarios = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaDibujables = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaBalas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaFD = PTHREAD_MUTEX_INITIALIZER;

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
	int bytesEnviados = 0;
	bool errorSocket = false;

	/*while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = send(sockfd, mensaje, tamanioMensaje - bytesEnviados, MSG_NOSIGNAL);
		if(n < 0){
			errorSocket = true;
		} else {
			bytesEnviados += n;
		}
	}
	return errorSocket;*/

	int n = write(sockfd, mensaje, tamanioMensaje);
	//int n = send(sockfd, mensaje, tamanioMensaje, MSG_NOSIGNAL);
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
			modoDios = atoi(prueba->value());
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

		FondoServer* nuevo = new FondoServer;
		nuevo->setAncho(atoi(anchoFondo->value()));
		string spriteID(fondoXML->value());
		nuevo->setSpriteId(spriteID);
		nuevo->setZindex(atoi(zFondo->value()));
		listaFondos.push_back(nuevo);

		capa1 = capa1->next_sibling();
	}

}

void cargarBonus(){

	//contenedorBonus.clear();

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
		int tipoBonus = rand() % 8;
		if(tipoBonus > 6){
			tipoBonus = 6;
		}
		//tipoBonus = 1;
		int x = sum + 800;
		sum = x;
		int y = ALTO_VENTANA - 60;

		int a = pthread_mutex_lock(&mutexContenedorBonus);
		contenedorBonus.nuevoBonus(x,y,tipoBonus);
		pthread_mutex_unlock (&mutexContenedorBonus);
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
		int tipoBonus = rand() % 8;
		if(tipoBonus > 6){
			tipoBonus = 6;
		}
		//tipoBonus = 1;
		int x = sum + 800;
		sum = x;
		int y = ALTO_VENTANA - 60;

		int a = pthread_mutex_lock(&mutexContenedorBonus);
		contenedorBonus.nuevoBonus(x,y,tipoBonus);
		pthread_mutex_unlock (&mutexContenedorBonus);
	}
}

string parseXMLPj(){
	const char *cstr = archivoXml.c_str();
	file<> xmlFile(cstr);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data()); 

	xml_node<> *sprites = doc.first_node("sprites");
	xml_node<> *pj = sprites->first_node("personaje");
	xml_node<> *path = pj->first_node("path");

	string retorno(path->value());
	return retorno;

}

void buscarNombreUsuario(char *nombreRetorno, int numeroUsuario){
	int a = pthread_mutex_lock(&mutexListaUsuarios);

	list<usuarioClass*>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(**it).nombreUsuario().c_str());

	pthread_mutex_unlock (&mutexListaUsuarios);
}

void enviarMensajeAConectados(string mensaje){
	//envio a todos los q esten online el mensaje
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(3,(**it).numCliente(),mensaje);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void enviarScoreAConectados (int numeroCliente, int score){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(12,(**it).numCliente(),numeroCliente,score);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void enviarDanoAConectados (int numeroCliente, int vida){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(13,(**it).numCliente(),numeroCliente,vida);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void agregaraLista(int numeroCliente, int usrDest, int x, int y, int spx, int spy, char flip, bool avanzar, int tipo){
	char nombreAutor[50] = "tu vieja";
	mensajeClass* mensajeObj = new mensajeClass(nombreAutor, usrDest, numeroCliente, x, y, spx, spy, flip, avanzar, tipo);

	listaDeMensajes.push_back(mensajeObj);
}

void enviarAConectados(int numeroCliente, int nuevaCordX, int nuevaCordY, int nuevoSpX, int nuevoSpY, char flip, bool avanzar, int tipo){
	//envio a todos los q esten online el mensaje de q se modifico un objeto
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			agregaraLista(numeroCliente, (**it).numCliente(), nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar, tipo);
		}
	}
}

void quitarEnemigo(int numeroCliente, int usrDest, int x, int y, int spx, int spy, char flip){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(0, (**it).numCliente(), numeroCliente, x, y, spx, spy, flip, 2);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void enviarBalasAConectados(int idBala, int x, int y, int direccion, int tipoBala, int spY){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(spY, (**it).numCliente(), idBala, x, y, direccion, tipoBala, 'D', 6);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void enviarBonusAConectados(int idBonus, int x, int y, int tipoBonus){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(0, (**it).numCliente(), idBonus, x, y, 0, tipoBonus, 'D', 9);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void quitarBalas(int idBala){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(0, (**it).numCliente(), idBala, 0, 0, 0, 0, 'D', 8);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void quitarBonus(int idBonus){
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(0, (**it).numCliente(), idBonus, 0, 0, 0, 0, 'D', 10);
			listaDeMensajes.push_back(mensajeObj);
		}
	}
}

void avanzarPantallaScore(){
	for (list<DibujableServer*>::iterator itD = listaDibujables.begin(); itD != listaDibujables.end(); ++itD) {
		for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
			if((**it).estaConectado()){
				char nombreAutor[50] = "tu vieja";
				mensajeClass* mensajeObj = new mensajeClass(nombreAutor, (**it).numCliente(), (*itD)->id, (*itD)->x, (*itD)->y, 0, 9, (*itD)->flip, 0, 0);
				listaDeMensajes.push_back(mensajeObj);
				mensajeObj = new mensajeClass(0, (**it).numCliente(), 0, 0, 0, 0, 0, '0', 20);
				listaDeMensajes.push_back(mensajeObj);
			}
		}
	}

	estaEnPantallaScore = true;
	//seteo un tiempo de espera..
	tiempoEsperaScore = time(NULL);
}

bool puedeSalirScore(){
	time_t aux = time(NULL);
	if(tiempoEsperaScore + 4 <= aux) return true;
	else return false;
}

void avanzarAlSiguienteNivel(){
	jefePresente = false;
	estaEnPantallaScore = false;
	listaDeMensajes.clear();
	contenedorEnemigos.cargarEnemigosDelNivel(nivelActual,ALTO_VENTANA);
	contenedorBalas.listaDeBalas.clear();
	contenedorBonus.listaBonus.clear();

	cargarBonus();
	//listaEnergias.clear();
	//listaScores.clear();

	//contenedorBalas.clear();
	//contenedorBonus.cargarBonusNivel();

	//envio a conectados q cierren grafica
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			char nombreDestino[50];
			//buscarNombreUsuario(nombreDestino, (**it).numCliente());

			mensajeClass* mensajeObj = new mensajeClass(4, (**it).numCliente());
			listaDeMensajes.push_back(mensajeObj);
		}

	}

	for(int i = 1; i <= listaDeUsuarios.size(); i++){
		list<DibujableServer*>::iterator it = listaDibujables.begin();
		advance(it, i-1);

		(*it)->quieto();
		(*it)->mVelX = 0;
		(*it)->mVelY = 0;
		(*it)->estaDisparando = false;
		(*it)->estaEnElPiso = true;
		(*it)->apunta = false;
		(*it)->score = 0;
		if(((*it)->vida) > 0){
			(*it)->vida = 0;
		}


		//string spriteId = parseXMLPj();
		//(*it)->setSpriteId(spriteId);
		(*it)->setX(1+rand() % (150));
		(*it)->setY(ALTO_VENTANA-100);
		//me saca a la momia si esta desconectado..
		(*it)->setSpX(0);
		(*it)->setSpY(1);
	}

	//envio a conectados q abran grafica
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {

		if((**it).estaConectado()){
			char nombreDestino[50];
			//buscarNombreUsuario(nombreDestino, (**it).numCliente());

			mensajeClass* mensajeObj = new mensajeClass(5, (**it).numCliente());
			listaDeMensajes.push_back(mensajeObj);
		}

	}

	cout << "FIN SIG NIVEL.." << endl;

	/*
	for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {
		if((**it).estaConectado()){
			mensajeClass* mensajeObj = new mensajeClass(0, (**it).numCliente(), 0, 0, 0, 0, 0, 'D', 20);
			listaDeMensajes.push_back(mensajeObj);
		}
	}*/

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

				pthread_mutex_lock(&mutexListaDibujables);
				list<DibujableServer*>::iterator it = listaDibujables.begin();
				advance(it, numeroCliente-1);

				//it->quieto();

				//it->estaConectado = false;
				nuevaCordX = (*it)->x;
				nuevaCordY = (*it)->y;
				//momia
				nuevoSpX = (*it)->spX = 1;
				nuevoSpY = (*it)->spY = 1;
				flip = (*it)->flip;
				(*it)->desconectar();
				pthread_mutex_unlock (&mutexListaDibujables);
				pthread_mutex_lock(&mutexListaUsuarios);
				list<usuarioClass*>::iterator it2 = listaDeUsuarios.begin();
				advance(it2, numeroCliente-1);

				//ver si conviene desconectar al usuario desde aca..
				(*it2)->desconectar();

				//envio a todos los q esten online el mensaje de q se modifico un objeto
				enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false,0);

				//envio un msj a todos los q esten online q se desconecto el usuario
				string mensaje = (**it2).nombreUsuario() + " Se ha desconectado";
				enviarMensajeAConectados(mensaje);
				pthread_mutex_unlock (&mutexListaUsuarios);
			}
		} else {
			int data = read(newsockfd, &codigo, sizeof(char));
		}

		pthread_mutex_lock(&mutexLista);

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
			for (list<usuarioClass*>::iterator i = listaDeUsuarios.begin(); i != listaDeUsuarios.end(); ++i) {

				if ((**i).nombreUsuario().compare(nombre) == 0){
					if(!(**i).estaConectado()){
						estabaDesconectado = true;
						(*i)->conectar();
						numeroCliente = (*i)->numCliente();

						//mandar msj a todos q volvio asi no esta mas gris
						int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
						char flip;
						list<DibujableServer*>::iterator it = listaDibujables.begin();
						advance(it, numeroCliente-1);

						nuevaCordY = (*it)->y;
						nuevaCordX = (*it)->x;
						//parado
						nuevoSpX = (*it)->spX = 0;
						nuevoSpY = (*it)->spY = 1;
						flip = (*it)->flip;

						(*it)->conectar();
						//envio a todos los q esten online el mensaje de q se modifico un objeto
						enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false,0);
					}

					respuesta = false;
					strcpy(mensaje,"El nombre ya esta en uso");
				}
			}

			if (respuesta){
				strcpy(mensaje,"Bienvenido");
				usuarioClass* nuevoUsuario = new usuarioClass(nombre, listaDeUsuarios.size()+1);
				listaDeUsuarios.push_back(nuevoUsuario);

				numeroCliente = listaDeUsuarios.size();

				//creo el dibujable del nuevo cliente
				DibujableServer* nuevo = new DibujableServer(modoDios, &escenario);
				nuevo->setId(listaDeUsuarios.size());
				string spriteId = parseXMLPj();
				nuevo->setSpriteId(spriteId);
				nuevo->setX(1 + rand() % (150));
				nuevo->setY(ALTO_VENTANA - 100);
				nuevo->setSpX (0);
				nuevo->setSpY(1);

				//creo dibujable energia
				DibujableServer* nuevo2 = new DibujableServer(modoDios, &escenario);
				nuevo2->setId(listaDeUsuarios.size());
				nuevo2->setSpY(0);

				//creo dibujable score
				DibujableServerAdicional* scoreNuevo = new DibujableServerAdicional;
				scoreNuevo->setId(listaDeUsuarios.size());
				scoreNuevo->setAumentable(0);

				listaDibujables.push_back(nuevo);
				listaEnergias.push_back(nuevo2);
				listaScores.push_back(scoreNuevo);

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
			list<usuarioClass*>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			(*it)->desconectar();
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
			cout << "TAMANO: " << tamano << endl;
			enviarMensaje(newsockfd, &tamano, sizeof(int));
			if(nivelActual == 1){
				for(list<FondoServer*>::iterator i =listaFondos.begin(); i != listaFondos.end();++i){
					int ancho = (**i).ancho;
					int z = (**i).zindex;
					int tamFondoId = (**i).spriteId.length() + 1;
					//cout << "tamFondoId: " << tamFondoId << endl;
					char spriteId[tamFondoId];
					strcpy(spriteId, (**i).spriteId.c_str());

					//cout << "spriteId: " << spriteId << endl;

					enviarMensaje(newsockfd, &tamFondoId, sizeof(int));
					enviarMensaje(newsockfd, &spriteId, sizeof(char)*tamFondoId);
					enviarMensaje(newsockfd, &ancho, sizeof(int));
					//enviarMensaje(newsockfd, &z, sizeof(int));
				}
			} else if(nivelActual == 2){

				string fondo1 = "l2fondo1.png";
				string fondo2 = "l2fondo2.png";
				string fondo3 = "l2fondo3.png";

				int tamFondo1 = fondo1.length() + 1;
				int tamFondo2 = fondo2.length() + 1;
				int tamFondo3 = fondo3.length() + 1;

				char spriteId1[tamFondo1];
				strcpy(spriteId1, "l2fondo1.png");
				char spriteId2[tamFondo2];
				strcpy(spriteId2, "l2fondo2.png");
				char spriteId3[tamFondo3];
				strcpy(spriteId3, "l2fondo3.png");


				int ancho1 = 1000;
				int ancho2 = 2000;
				int ancho3 = 4000;

				enviarMensaje(newsockfd, &tamFondo1, sizeof(int));
				enviarMensaje(newsockfd, &spriteId1, sizeof(char)*tamFondo1);
				enviarMensaje(newsockfd, &ancho1, sizeof(int));

				enviarMensaje(newsockfd, &tamFondo2, sizeof(int));
				enviarMensaje(newsockfd, &spriteId2, sizeof(char)*tamFondo2);
				enviarMensaje(newsockfd, &ancho2, sizeof(int));

				enviarMensaje(newsockfd, &tamFondo3, sizeof(int));
				enviarMensaje(newsockfd, &spriteId3, sizeof(char)*tamFondo3);
				enviarMensaje(newsockfd, &ancho3, sizeof(int));
			} else if(nivelActual == 3){

				string fondo1 = "l3fondo1.png";
				string fondo2 = "l3fondo2.png";
				string fondo3 = "l3fondo3.png";

				int tamFondo1 = fondo1.length() + 1;
				int tamFondo2 = fondo2.length() + 1;
				int tamFondo3 = fondo3.length() + 1;

				char spriteId1[tamFondo1];
				strcpy(spriteId1, "l3fondo1.png");
				char spriteId2[tamFondo2];
				strcpy(spriteId2, "l3fondo2.png");
				char spriteId3[tamFondo3];
				strcpy(spriteId3, "l3fondo3.png");


				int ancho1 = 1000;
				int ancho2 = 2000;
				int ancho3 = 4000;

				enviarMensaje(newsockfd, &tamFondo1, sizeof(int));
				enviarMensaje(newsockfd, &spriteId1, sizeof(char)*tamFondo1);
				enviarMensaje(newsockfd, &ancho1, sizeof(int));

				enviarMensaje(newsockfd, &tamFondo2, sizeof(int));
				enviarMensaje(newsockfd, &spriteId2, sizeof(char)*tamFondo2);
				enviarMensaje(newsockfd, &ancho2, sizeof(int));

				enviarMensaje(newsockfd, &tamFondo3, sizeof(int));
				enviarMensaje(newsockfd, &spriteId3, sizeof(char)*tamFondo3);
				enviarMensaje(newsockfd, &ancho3, sizeof(int));
			}
			break;
		}
		case '5': {
			//cout << "ENTRO A BUSCAR MSJS NUEVOS" << endl;
			for (list<mensajeClass*>::iterator i = listaDeMensajes.begin(); i != listaDeMensajes.end(); ++i) {
				//if ((**i).nombreDestinatario().compare(nombre) == 0 ){
				if ((*i)->usrDestino == numeroCliente){
					//cout << "TENGO UN MENSAJE: "<< (*i)->usrDestino << endl;
					int corte = 1;
					enviarMensaje(newsockfd, &corte, sizeof(int));

					//envio tipo de mensaje
					int tipoMensaje = (**i).getTipoMensaje();
					enviarMensaje(newsockfd, &tipoMensaje, sizeof(int));
					switch(tipoMensaje){
					case 1:{
						//envio de grafica
						int xCord = (**i).getX();
						int yCord = (**i).getY();
						int spX = (**i).getSpX();
						int spY = (**i).getSpY();
						int idObjeto = (**i).getidObjeto();
						bool puedeAvanzar = (**i).getBoolAvanzar();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &spX, sizeof(int));
						enviarMensaje(newsockfd, &spY, sizeof(int));
						enviarMensaje(newsockfd, &(**i).flip, sizeof(char));
						enviarMensaje(newsockfd, &puedeAvanzar, sizeof(bool));
						enviarMensaje(newsockfd, &(*i)->tipoDibujable, sizeof(int));

						enviarMensaje(newsockfd, &camaraX, sizeof(int));

						break;
					}
					case 2:{
						//envio de bajas de enemigos
						int idObjeto = (**i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}
					case 3:{
						//envio de mensajes
						string mensaje = (**i).getMensaje();
						char* cstr = new char [mensaje.length()+1];
						strcpy (cstr, mensaje.c_str());
						int tamanioMensaje = mensaje.length()+1;

						enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
						enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));

						break;
					}
					case 6:{
						//envio de balas
						int xCord = (**i).getX();
						int yCord = (**i).getY();
						int direccion = (**i).getSpX();
						int tipo = (**i).getSpY();
						int idObjeto = (**i).getidObjeto();
						int spY = (**i).avanzar;

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &direccion, sizeof(int));
						enviarMensaje(newsockfd, &tipo, sizeof(int));
						enviarMensaje(newsockfd, &spY, sizeof(int));

						break;
					}

					case 9:{
						int xCord = (**i).getX();
						int yCord = (**i).getY();
						int idObjeto = (**i).getidObjeto();
						int tipo = (**i).getSpY();

						enviarMensaje(newsockfd, &idObjeto, sizeof(int));
						enviarMensaje(newsockfd, &xCord, sizeof(int));
						enviarMensaje(newsockfd, &yCord, sizeof(int));
						enviarMensaje(newsockfd, &tipo, sizeof(int));

						break;
					}

					case 10:{
						//envio baja de bonus
						int idObjeto = (**i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}

					case 8:{
						//envio bajas de balas
						int idObjeto = (**i).getidObjeto();
						enviarMensaje(newsockfd, &idObjeto, sizeof(int));

						break;
					}

					case 11:{
						int score = 10;
						enviarMensaje(newsockfd, &score, sizeof(int) );

						break;
					}

					case 12:{
						int score = (**i).getScore();
						int autor = (**i).numAutor();

						enviarMensaje(newsockfd, &autor, sizeof(int));
						enviarMensaje(newsockfd, &score, sizeof(int));

						break;

					}

					case 13:{
						int vida = (**i).getEnergia();
						int autor = (**i).numAutor();

						enviarMensaje(newsockfd, &autor, sizeof(int));
						enviarMensaje(newsockfd, &vida, sizeof(int));

						break;
					}

					case 20:{
						cout << "LEE EL 20" << endl;
						//PASAR DE NIVEL
						break;
					}

					default:
						break;

					}

					delete (*i);

					i = listaDeMensajes.erase(i);
					i--;
				}
			}

			int tamanioMensaje = 0;
			enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));

			list<int>::iterator itFD = listaFDClientes.begin();
			if(newsockfd == (*itFD)){
				//if(numeroCliente == 1){
				//cout << "MUEVO ENEMIGOS" << endl;
				//BUSCO ENEMIGOS
				list<DibujableServerEnemigo> listaEnemigosActivos;
				list<DibujableServerEnemigo> listaEnemigosDeBaja;

				contenedorEnemigos.buscarActivos(camaraX,&listaEnemigosActivos, &listaEnemigosDeBaja, &contenedorBalas);

				//BUSCO BALAS
				list<bala> listaBalasActivas;
				list<bala> listaBalasDeBaja;

				contenedorBalas.buscarActivas(camaraX, &listaBalasActivas, &listaBalasDeBaja);

				list<DibujableServerEnemigo> listaEnemigosDisparados;
				contenedorBalas.detectarColisionPlataforma(&escenario);
				contenedorBalas.detectarColisiones(&listaBalasDeBaja, &listaEnemigosActivos, &listaEnemigosDisparados, &listaScores, &listaDibujables, modoJuego);

				for (list<DibujableServerAdicional*>::iterator itScore = listaScores.begin(); itScore != listaScores.end(); ++itScore) {
					enviarScoreAConectados((*itScore)->id,(*itScore)->getAumentable());
				}

				int iCont = 0;
				for (list<DibujableServer*>::iterator itUser = listaDibujables.begin(); itUser != listaDibujables.end(); ++itUser) {		
					if(((*itUser)->vida) == 10){
						(*itUser)->desconectar();
						iCont++;
					}
					if(((*itUser)->vida) != ((*itUser)->vidaAnterior)){
						enviarDanoAConectados((*itUser)->id,(*itUser)->vida);
					}
				}

				if(cantidadJugadores == iCont){
					cout << "GAME OVER!" << endl;
				}


				pasarDeNivel = contenedorEnemigos.matarEnemigos(camaraX, listaEnemigosDisparados);

				//hay que enviar a conectados un msjedel tipo pasaste de nivel y cuando
				//los conectados lo reciban, le envian al servidor un mensaje con un codigo
				//para que muestre una pantalla con los scores
				if (pasarDeNivel){
					//agusss : ya esta la lista de "DibujableServerAdicional" esta el metodo getAumentable que seria el SCORE actual para pasarles, Pablo.
					nivelActual++;

					if (nivelActual <= 3){
						contenedorEnemigos.killAll(&listaEnemigosActivos, &listaEnemigosDeBaja);
						avanzarPantallaScore();
					}

					//PARAR TODOS LOS MENSAJES POSIBLES SINO ROMPE!!!!!

				}else{
					list<Bonus> listaBonusActivos;
					list<Bonus> listaBonusDeBaja;

					contenedorBonus.buscarActivos(camaraX, &listaBonusActivos, &listaBonusDeBaja);

					//list<Bonus> listaBonusAgarrados;

					int usrKillAll;
					if(contenedorBonus.detectarColision(&listaBonusDeBaja, &listaBonusActivos, &listaDibujables, usrKillAll)){
						//Si entra es porque agarro el bonus de Kill All
						contenedorEnemigos.killAll(&listaEnemigosActivos, &listaEnemigosDeBaja, usrKillAll, &listaScores, modoJuego);
					}

					for (list<bala>::iterator itBalas = listaBalasActivas.begin(); itBalas != listaBalasActivas.end(); ++itBalas) {
						enviarBalasAConectados(itBalas->id,itBalas->x,itBalas->y,itBalas->direccionDisparo, itBalas->tipoBala, itBalas->spY);
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

					for (list<Bonus>::iterator itBonus = listaBonusActivos.begin(); itBonus != listaBonusActivos.end(); ++itBonus) {
						enviarBonusAConectados(itBonus->getId(), itBonus->getPosX(), itBonus->getPosY(), itBonus->getTipoBonus());
					}

					for (list<Bonus>::iterator itBonus = listaBonusDeBaja.begin(); itBonus != listaBonusDeBaja.end(); ++itBonus) {
						quitarBonus(itBonus->getId());
					}
				}

				if(estaEnPantallaScore){
					if(puedeSalirScore()){
						cout << "PUDE SALIR DEL SCORE" << endl;
						avanzar = false;
						camaraX = 0;
						camaraSet = 0;
						avanzarAlSiguienteNivel();
					}else cout << "NO PUDE SALIR DEL SCORE" << endl;
				}
			}

			break;
		}

		case '6': {
			//peticion de cantidad de objetos a dibujar
			//cout << "PIDO CANT OBJ A DIBUJAR" << endl;
			int cantidad = listaDibujables.size();
			enviarMensaje(newsockfd,&cantidad,sizeof(int));

			break;
		}

		case '7': {
			//cout << "PIDO SI PUEDO EMEPZAR A JUGAR" << endl;
			//pido si puede empezar el juego
			int respuesta = 1;
			if (listaDeUsuarios.size() < cantidadJugadores){
				respuesta = 0;
			} else if (listaDeUsuarios.size() > cantidadJugadores){
				respuesta = -1;
				list<usuarioClass*>::iterator it = listaDeUsuarios.begin();
				advance(it, numeroCliente-1);
				delete (*it);
				listaDeUsuarios.erase(it);

				list<DibujableServer*>::iterator it2 = listaDibujables.begin();
				advance(it2, numeroCliente-1);
				delete (*it2);
				listaDibujables.erase(it2);

			}
			enviarMensaje(newsockfd,&respuesta,sizeof(int));

			break;
		}

		case '8': {
			//cout << "CASE 8" << endl;
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;
			for (list<DibujableServer*>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				//empiezo de nuevo
				nuevaCordY = (*it)->y = ALTO_VENTANA - 100;
				nuevaCordX = (*it)->x = 100;
				nuevoSpX = (*it)->spX; //le dejo el mismo sprite.. sino poner if estaConectado
				nuevoSpY = (*it)->spY;
				flip = 'D';

				enviarAConectados((*it)->id,nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, avanzar,0);
			}
			break;
		}

		case '9':{
			//cout << "ENVIO DIBUJABLES" << endl;
			//caso envio objetos dibujables
			int numeroUsuario;
			recibirMensaje(newsockfd, &numeroUsuario, sizeof(int));
			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroUsuario-1);

			//idObjeto
			enviarMensaje(newsockfd,&((*it)->id),sizeof(int));

			//spriteId
			int tamSpriteId = (*it)->spriteId.length() + 1;
			//cout << "tamSpriteId: " << tamSpriteId << endl;
			enviarMensaje(newsockfd,&tamSpriteId,sizeof(int));

			char spriteId[tamSpriteId];
			strcpy(spriteId, (*it)->spriteId.c_str());
			//cout << "spriteId: " << spriteId << endl;
			enviarMensaje(newsockfd,spriteId,sizeof(char)*tamSpriteId);

			enviarMensaje(newsockfd,&(**it).x,sizeof(int));
			enviarMensaje(newsockfd,&(**it).y,sizeof(int));
			enviarMensaje(newsockfd,&(**it).spX,sizeof(int));
			enviarMensaje(newsockfd,&(**it).spY,sizeof(int));
			enviarMensaje(newsockfd,&(**it).flip,sizeof(char));
			break;
		}

		case 'a':{
			//cout << "PIDO ENERGIAS" << endl;
			int cant = listaEnergias.size();
			enviarMensaje(newsockfd, &cant, sizeof(int));
			for (list<DibujableServer*>::iterator it = listaEnergias.begin(); it != listaEnergias.end(); ++it)
			{
				int id = (**it).getId();
				int spY = (**it).getSpY();
				//int tamaño spriteID
				enviarMensaje(newsockfd, &id, sizeof(int));
				enviarMensaje(newsockfd, &spY, sizeof(int));

			}
			break;
		}

		case 'c':{
			//cout << "ENVIO CAM X" << endl;
			enviarMensaje(newsockfd,&camaraX,sizeof(int));

			break;
		}

		case 'M':{
			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if(!pasarDeNivel){
				bool seMovio = (*it)->mover(camaraX);
				//enviar si realmente hay cambios..
				if (seMovio) {
					VELOCIDAD_JUGADOR = (*it)->velocidadXJugador();
					enviarAConectados(numeroCliente, (*it)->x, (*it)->y, (*it)->spX, (*it)->spY, (*it)->flip, avanzar,0);

					//CALCULO DE LA CAMARA
					if(!jefePresente){
						int xMin, xMax;
						xMin = 10000;

						bool avanzarCamara = false;
						for (list<DibujableServer*>::iterator i = listaDibujables.begin(); i != listaDibujables.end(); ++i) {
							if((*i)->estaConectado()){
								//si alguno supero la mitad de la pantalla avanzo camara
								if((*i)->x >= camaraX + (ANCHO_VENTANA/2)-80) avanzarCamara = true;
								if((*i)->x < xMin) xMin = (*i)->x;
							}
						}

						if (camaraX >= 1000){ //8075
							avanzarCamara = false;
							jefePresente = true;
							contenedorEnemigos.iniciarJefe(camaraX, nivelActual);
						}

						if(avanzarCamara){
							if(xMin > camaraX){
								camaraX += VELOCIDAD_JUGADOR;
							}
						}

						//itero desconectados
						for (list<DibujableServer*>::iterator it2 = listaDibujables.begin(); it2 != listaDibujables.end(); ++it2) {
							if (!(*it2)->estaConectado()){
								if ((*it2)->x <= camaraX){
									(*it2)->x = camaraX;//adelanto a la momia
									enviarAConectados((*it2)->id , (*it2)->x, (*it2)->y, (*it2)->spX, (*it2)->spY, (*it2)->flip, false,0);
								}
							}
						}
					}
				}
			}else pasarDeNivel = false;

			break;
		}

		case 'J':{
			//cout << "SALTO" << endl;
			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			if((*it)->estaVivo && (!pasarDeNivel))
				(*it)->saltar();

			break;
		}

		case 'U':{
			//cout << "UP" << endl;
			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x)>(camaraX)) {
					(*it)->apuntarArriba();
				}else (*it)->quieto();

			break;
		}

		case 'D':{
			//cout << "DOWN" << endl;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x) > (camaraX)){
					(*it)->apuntarAbajo();
				}else (*it)->quieto();

			break;
		}

		case 'L':{
			//cout << "LEFT" << endl;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x) > (camaraX)){
					(*it)->caminarIzquierda();
				}else (*it)->quieto();

			break;
		}
		case 'R':{
			//cout << "RIGHT" << endl;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x) < (camaraX + ANCHO_VENTANA)-80){ // - lo q ocupa el sprite en pantalla..
					(*it)->caminarDerecha();
				}else (*it)->quieto();

			break;
		}

		case 'E':{
			//cout << "DIAGDER" << endl;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x) > (camaraX)){
					(*it)->apuntarDiagDer();
				}else (*it)->quieto();

			break;
		}

		case 'Q':{
			//cout << "DIAGIZQ" << endl;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if (((*it)->x) > (camaraX)){
					(*it)->apuntarDiagIzq();
				}else (*it)->quieto();

			break;
		}

		case 'C':{
			//cout << "CERRAR GRAFICA" << endl;

			//caso cerrar ventana grafica
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);
			nuevaCordX = (*it)->x;
			nuevaCordY = (*it)->y;
			//momia
			nuevoSpX = (*it)->spX = 1;
			nuevoSpY = (*it)->spY = 1;
			flip = (*it)->flip;
			(*it)->desconectar();

			list<usuarioClass*>::iterator it2 = listaDeUsuarios.begin();
			advance(it2, numeroCliente-1);
			(*it2)->desconectar();

			//envio a todos los q esten online el mensaje de q se modifico un objeto
			enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false, 0);

			//envio un msj a todos los q esten online q se desconecto el usuario
			string mensaje = (**it2).nombreUsuario() + " Se ha desconectado";

			enviarMensajeAConectados(mensaje);
			inicioGrafica = false;

			pthread_mutex_lock(&mutexListaFD);
			for(list<int>::iterator itFD = listaFDClientes.begin(); itFD != listaFDClientes.end(); ++itFD){
				if((*itFD) == newsockfd){
					itFD = listaFDClientes.erase(itFD);
					itFD--;
				}
			}
			pthread_mutex_unlock(&mutexListaFD);

			break;
		}
		case 'S':{
			//cout << "STOP" << endl;

			//caso se queda parado
			int nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY;
			char flip;

			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel)){
				(*it)->quieto();

				//sprite parado
				nuevaCordX = (*it)->x;
				nuevaCordY = (*it)->y;
				nuevoSpX = (*it)->spX;
				nuevoSpY = (*it)->spY;
				flip = (*it)->flip;

				//envio a todos los q esten online el mensaje de q se modifico un objeto
				enviarAConectados(numeroCliente, nuevaCordX, nuevaCordY, nuevoSpX, nuevoSpY, flip, false, 0);
			}
			break;
		}

		case 'i':{
			//cout << "PIDO Q TODOS CIERREN LA GRAFICA" << endl;

			//envio a conectados q cierren grafica
			for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {

				if((**it).estaConectado()){
					char nombreDestino[50];
					//buscarNombreUsuario(nombreDestino, (**it).numCliente());

					mensajeClass* mensajeObj = new mensajeClass(4, (**it).numCliente());
					listaDeMensajes.push_back(mensajeObj);
				}

			}

			avanzar = false;
			camaraX = 0;
			camaraSet = 0;

			for(int i = 1; i <= listaDeUsuarios.size(); i++){

				list<DibujableServer*>::iterator it = listaDibujables.begin();
				advance(it, i-1);

				//DibujableServer nuevo;
				string spriteId = parseXMLPj();
				(*it)->setSpriteId(spriteId);
				(*it)->setX(1+rand() % (150));
				(*it)->setY(ALTO_VENTANA-100);
				//me saca a la momia si esta desconectado..
				//it->setSpX(0);
				//it->setSpY(1);

			}

			//envio a conectados q abran grafica
			for (list<usuarioClass*>::iterator it = listaDeUsuarios.begin(); it != listaDeUsuarios.end(); ++it) {

				if((**it).estaConectado()){
					char nombreDestino[50];
					//buscarNombreUsuario(nombreDestino, (**it).numCliente());

					mensajeClass* mensajeObj = new mensajeClass(5, (**it).numCliente());
					listaDeMensajes.push_back(mensajeObj);
				}

			}
			break;
		}

		case 'd': {
			//cout << "DISPARO" << endl;

			int direccionDisparo;
			recibirMensaje(newsockfd, &direccionDisparo, sizeof(int));

			//busco quien la disparo..
			list<DibujableServer*>::iterator it = listaDibujables.begin();
			advance(it, numeroCliente-1);

			if((*it)->estaVivo && (!pasarDeNivel))
				if( (*it)->disparar(direccionDisparo) ){
					int x = (*it)->getX();
					int y = (*it)->getY();
					int usr = (*it)->getId();
					char flip = (*it)->flip;
					int tipoDeArma = (*it)->tipoDeArma;

					if(direccionDisparo == -1){
						if(flip == 'D') direccionDisparo = 0;
						else direccionDisparo = 4;
					}

					contenedorBalas.nuevaBala(x, y, usr, direccionDisparo, tipoDeArma, 0); //pasar tipo de disparo..
				}

			break;
		}

		default:
			//cout << "No deberia ingresar aca" << endl;
			break;
		}

		pthread_mutex_unlock (&mutexLista);
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

	//cargo enemigos nivel 1
	contenedorEnemigos.cargarEnemigosDelNivel(nivelActual, ALTO_VENTANA);

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
			pthread_mutex_lock(&mutexListaUsuarios);
			pthread_create(&thread, NULL, atender_cliente, (void*)(long)newsockfd);
			listaFDClientes.push_back(newsockfd);
			cantidadJugadoresConectados++;
			cout << "TOTAL: " << cantidadJugadores << endl;
			cout << "CONECTADOS: " << cantidadJugadoresConectados << endl;
			/*printf("Conectado!\n");
			printf("Leyendo mensaje...\n");*/
			pthread_mutex_unlock(&mutexListaUsuarios);
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
	for(list<FondoServer*>::iterator i =listaFondos.begin(); i != listaFondos.end();++i){
		delete (*i);
		i = listaFondos.erase(i);
		i--;

	}

	close(this->sockfd);
	loggearInterno( " SE CERRO EL SOCKET");
}

mySocketSrv::~mySocketSrv(){}
