#include "socket.hpp"
#include <pthread.h>
#include <list>
#include <fstream>
#include "mensajeClass.hpp"
#include "usuarioClass.hpp"
#include "DibujableServer.h"

#define DEBUG 2

list<DibujableServer> listaDibujables;

list<mensajeClass> listaDeMensajes;
list<string> listaDeUsuarios;
char* archivoUsuarios;
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

void responderLogin(int newsockfd, int respuesta, string mensaje){
	char* cstr = new char [mensaje.length()+1];
	strcpy (cstr, mensaje.c_str());
	int tamanioMensaje = mensaje.length()+1;

	enviarMensaje(newsockfd, &respuesta, sizeof(int));
	enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
	enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));

	delete[] cstr;
}

void buscarNombreUsuario(char *nombreRetorno, int numeroUsuario){
	list<string>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(*it).c_str());
}

void agregaraLista(int numeroCliente, int usrDest, char *mensaje){
	char nombreAutor[50];
	char nombreDestino[50];
	buscarNombreUsuario(nombreAutor, numeroCliente);
	buscarNombreUsuario(nombreDestino, usrDest);

	mensajeClass mensajeObj(nombreAutor, nombreDestino, mensaje, numeroCliente);

	int a = pthread_mutex_trylock(&mutexLista);
	/*if (a != 0) {
		cout<<"Otro hilo usando la lista"<<endl;
	}*/

	listaDeMensajes.push_back(mensajeObj);
	loggear(mensajeObj.nombreAutor(),mensajeObj.nombreDestinatario(),mensajeObj.getMensaje());
	pthread_mutex_unlock (&mutexLista);
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
			char mensaje[256];

			//comparo con todos los usuarios q esten si existe devuelvo error
			for (list<string>::iterator i = listaDeUsuarios.begin(); i != listaDeUsuarios.end(); ++i) {
				if ((*i).compare(nombre) == 0){
					respuesta = false;
					strcpy(mensaje,"El nombre ya esta en uso");
				}
			}

			if (respuesta){
				strcpy(mensaje,"Nombre ingresado");
				listaDeUsuarios.push_back(nombre);

				numeroCliente = listaDeUsuarios.size();

				//creo el dibujable del nuevo cliente
				DibujableServer nuevo;
				nuevo.setId(listaDeUsuarios.size());
				nuevo.setSpriteId("player");
				nuevo.setX(1 + rand() % (150));
				nuevo.setY(1 + rand() % (150));
				listaDibujables.push_back(nuevo);
			}

			responderLogin(newsockfd,respuesta,mensaje);

			break;
		}
		case '2':{
			//cout << "Entro a /2 que es desconectar" << endl;
			abierto = false;

			list<string>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			it = listaDeUsuarios.erase(it);
			//nose si esta bien este .erase
			break;
		}
		case '5': {
			//cout << "Entro a /5 que es recibir" << endl;

			for (list<mensajeClass>::iterator i = listaDeMensajes.begin(); i != listaDeMensajes.end(); ++i) {
				char nombre[50];
				buscarNombreUsuario(nombre, numeroCliente);
				if ((*i).nombreDestinatario().compare(nombre) == 0 ){
					string mensaje = (*i).getMensaje();
					char* cstr = new char [mensaje.length()+1];
					strcpy (cstr, mensaje.c_str());
					int tamanioMensaje = mensaje.length()+1;
					int idObjeto = (*i).getidObjeto();

					string usuario = (*i).nombreAutor();
					char* cstr2 = new char [usuario.length()+1];
					strcpy (cstr2, usuario.c_str());
					int tamanioUsuario = usuario.length()+1;

					enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
					enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));
					enviarMensaje(newsockfd, &idObjeto, sizeof(int));

					delete[] cstr2;
					delete[] cstr;

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

			break;
		}

		case 'U':{
			/* ESTO ES PARA MODIFICAR LA LISTA DE OBJ DEL SERVIDOR..
			 * Q CUANDO UN CLIENTE NUEVO SE CONECTA DURANTE EL JUEGO, COPIA ESOS DATOS
			//modifico el dibujable de la lista del servidor
			for (list<DibujableServer>::iterator it = listaDibujables.begin(); it != listaDibujables.end(); ++it) {
				if ((*it).id == numeroCliente){
					//hacer metodo mover up y q sume solo en la clase dibujableServer...
					it->x = it->x - 2;
				}
			}*/

			//envio a todos el mensaje de q se modifico un objeto
			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				char codigo[1];
				strcpy(codigo,"U");
				//el servidor va a poner en la lista los msjs para todos
				//el msj contiene el id del objeto a mover y su nueva posicion
				agregaraLista(numeroCliente, i, codigo);
			}

			break;
		}
		case 'D':{
			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				char codigo[1];
				strcpy(codigo,"D");
				agregaraLista(numeroCliente, i, codigo);
			}
			break;
		}
		case 'L':{
			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				char codigo[1];
				strcpy(codigo,"L");
				agregaraLista(numeroCliente, i, codigo);
			}
			break;
		}
		case 'R':{
			for(int i = 1; i <= listaDeUsuarios.size(); i++){
				char codigo[1];
				strcpy(codigo,"R");
				agregaraLista(numeroCliente, i, codigo);
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

mySocketSrv :: mySocketSrv(char* archusr, char* puerto){
	archivoUsuarios = archusr;
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

	//cargo un fondo cualquiera
	//ver los valores del id
	DibujableServer nuevo;
	nuevo.setId(0);
	nuevo.setSpriteId("fondo");
	nuevo.setX(0);
	nuevo.setY(0);
	listaDibujables.push_back(nuevo);
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
