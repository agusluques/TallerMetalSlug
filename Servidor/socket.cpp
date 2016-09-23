#include "socket.hpp"
#include <pthread.h>
#include <list>
#include <fstream>
#include "mensajeClass.hpp"
#include "usuarioClass.hpp"

#define DEBUG 2


list<mensajeClass> listaDeMensajes;
list<usuarioClass> listaDeUsuarios;
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

void cargarUsuarios(){
	//INSERTAR EN LA LISTA LOS USUARIOS DESDE UN CSV (*)
	ifstream file(archivoUsuarios);

	while(!file.eof()) {
		string usr;
		string pass;

		getline(file,usr,';');
		getline(file,pass);

		if(file.eof())
			break;

		char usuario[50];
		strcpy(usuario,usr.c_str());
		char contrasenia[50];
		strcpy(contrasenia,pass.c_str());

		usuarioClass nuevoUsuario(usuario,contrasenia);

		listaDeUsuarios.push_back(nuevoUsuario);
	}

	file.close();

}

void recibirMensaje(int sockfd, void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;
	bool errorSocket = false;
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesRecibidos = read(sockfd, buffer, tamanio);
		if (bytesRecibidos == (-1)){
			cout << "Error al recibir datos SOCKET" << endl;
			loggearServer( " ERROR AL RECIBIR DATOS SOCKET");
			errorSocket = true;
		} else {
			acumulador += bytesRecibidos;
		}
	}
}

void enviarMensaje(int sockfd, void* mensaje, int tamanioMensaje){
	int bytesEnviados = 0;
	bool errorSocket = false;
	while((bytesEnviados < tamanioMensaje) && (!errorSocket)){
		int n = write(sockfd, mensaje, tamanioMensaje - bytesEnviados);
		if(n < 0){
			errorSocket = true;
		}
		bytesEnviados += n;
	}
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
	list<usuarioClass>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	strcpy(nombreRetorno,(*it).nombreUsuario().c_str());
}

usuarioClass buscarUsuario(int numeroUsuario){
	list<usuarioClass>::iterator it = listaDeUsuarios.begin();
	advance(it, numeroUsuario-1);
	return (*it);
}

void agregaraLista(int numeroCliente, int usrDest, char * mensaje){
	char nombreAutor[50];
	char nombreDestino[50];
	buscarNombreUsuario(nombreAutor, numeroCliente);
	buscarNombreUsuario(nombreDestino, usrDest);
	mensajeClass mensajeObj(nombreAutor, nombreDestino, mensaje);

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

		int tamanio = (sizeof(char));
		char codigo;
		recibirMensaje(newsockfd, &codigo, tamanio);
		//cout << "Codigo: " << codigo << endl;

		switch(codigo){

		case '0':
		{
			char cod = '0';
			enviarMensaje(newsockfd, &cod, sizeof(char));
			//cout << "Entro a /0 que es para detectar la conexion" << endl;
			//cout << "Si imprime esto esta conectado todavia" << endl;
			break;
		}


		case '1': {
			//cout << "Entro a /1 que es conectar " << endl;
			int tamLista = listaDeUsuarios.size();
			enviarMensaje(newsockfd, &tamLista, sizeof(int));
			int tamContrasenia, usr;
			recibirMensaje(newsockfd, &usr, sizeof(int));
			char nombre[50];
			buscarNombreUsuario(nombre,usr);
			//cout << "Nombre de Usuario: " << nombre << endl;

			recibirMensaje(newsockfd, &tamContrasenia, sizeof(int));
			//cout << "Tamaño Contraseña: " << tamContrasenia << endl;
			char contrasenia[tamContrasenia];
			recibirMensaje(newsockfd, &contrasenia, sizeof(char)*tamContrasenia);

			//cout << "Contraseña: " << contrasenia << endl;

			//VALIDO DATOS ASUMO QUE EL USUARIO ES CORRECTO
			usuarioClass usuario = buscarUsuario(usr);
			numeroCliente = usr;

			char mensaje[256];
			bool respuesta = usuario.validarUsuario(contrasenia,mensaje);

			if (respuesta){
				list<usuarioClass>::iterator it = listaDeUsuarios.begin();
				advance(it, usr-1);
				memcpy(&(*it),&usuario,sizeof(usuarioClass));
				userPoint=&usuario;
			}

			responderLogin(newsockfd,respuesta,mensaje);

			break;
		}
		case '2':{
			//cout << "Entro a /2 que es desconectar" << endl;
			abierto = false;
			userPoint->desconectar();

			list<usuarioClass>::iterator it = listaDeUsuarios.begin();
			advance(it, numeroCliente-1);
			memcpy(&(*it),userPoint,sizeof(usuarioClass));

			break;
		}
		case '3':
			//cout << "Entro a /3 que es salir" << endl;
			break;

		case '4':
		{
			//cout << "Entro a /4 que es enviar" << endl;
			int tam, usrDest;
			recibirMensaje(newsockfd, &usrDest, sizeof(int));
			//cout << "Numero de usuario Destino: " << usrDest << endl;
			recibirMensaje(newsockfd, &tam, sizeof(int));
			//cout << "Tamanio del mensaje: " << tam << endl;
			//char mensaje[(tam+1)];
			char mensaje[tam];
			recibirMensaje(newsockfd, &mensaje, sizeof(char)*tam);
			//mensaje[tam] = '\n';
			//cout << "Mensaje: " << mensaje << endl;


			if(usrDest == listaDeUsuarios.size()+1){ //caso envio a todos
				for(int i = 1; i <= listaDeUsuarios.size(); i++){
					agregaraLista(numeroCliente, i, mensaje);
				}
			}else	agregaraLista(numeroCliente, usrDest, mensaje);
			break;
		}
		case '5':
		{
			//cout << "Entro a /5 que es recibir" << endl;

			//cout << "cant msjs: " << listaDeMensajes.size() << endl;

			for (list<mensajeClass>::iterator i = listaDeMensajes.begin(); i != listaDeMensajes.end(); ++i)
			{
				char nombre[50];
				buscarNombreUsuario(nombre, numeroCliente);
				if ((*i).nombreDestinatario().compare(nombre) == 0 ){
					string mensaje = (*i).getMensaje();
					char* cstr = new char [mensaje.length()+1];
					strcpy (cstr, mensaje.c_str());
					int tamanioMensaje = mensaje.length()+1;

					string usuario = (*i).nombreAutor();
					char* cstr2 = new char [usuario.length()+1];
					strcpy (cstr2, usuario.c_str());
					int tamanioUsuario = usuario.length()+1;

					/*cout<<"tam mensaje; " <<tamanioMensaje<<endl;
					cout<<"mensaje; "<< cstr<<endl;
					cout<<"tam usuario; " <<tamanioUsuario<<endl;
					cout<<"tusuario; "<< cstr2<<endl;*/
					enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
					enviarMensaje(newsockfd, cstr, tamanioMensaje*(sizeof(char)));
					enviarMensaje(newsockfd, &tamanioUsuario, sizeof(int));
					enviarMensaje(newsockfd, cstr2, tamanioUsuario*(sizeof(char)));

					delete[] cstr2;
					delete[] cstr;

					i = listaDeMensajes.erase(i);
					i--;
				}
			}

			int tamanioMensaje = 0;

			enviarMensaje(newsockfd, &tamanioMensaje, sizeof(int));
			userPoint->recibir();
			break;
		}

		case '6':
			//cout << "Entro a /6 que es Lorem Ipsum" << endl;
			//Recibir los mensajes
			break;

		case '7':{
			//cout << "entro a /7 que es peticion de usuarios" << endl;

			int numeroUsuario;
			recibirMensaje(newsockfd, &numeroUsuario, sizeof(int));

			usuarioClass usuario = buscarUsuario(numeroUsuario);

			int tamMensaje = sizeof(usuario.nombre);
			char mensaje[tamMensaje];

			enviarMensaje(newsockfd,&tamMensaje,sizeof(int));
			strcpy(mensaje, usuario.nombre);
			enviarMensaje(newsockfd,mensaje,tamMensaje);

			break;
		}
		case '8':{
			//cout << "entro a /8 que es peticion de cantidad de usuarios" << endl;
			int cantidad = listaDeUsuarios.size();
			enviarMensaje(newsockfd,&cantidad,sizeof(int));

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

	cargarUsuarios();
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

void mySocketSrv::enviarMensaje(string mensaje){

	char* cstr = new char [mensaje.length()+1];
	strcpy (cstr, mensaje.c_str());

	int n = write(sockfd,cstr,mensaje.length());
	if (n < 0){
		cout << "Error en la escritura" << endl;
	    loggearInterno( " ERROR EN LA ESCRITURA");
	}
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
