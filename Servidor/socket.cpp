#include "socket.hpp"
#include <pthread.h>
#include <map>
#include <fstream>
#include <ctime>
#include <list>


struct paquete{
	int tipo; // tipo de estructura (autenticacion, mensajes)
	char usuario[50];
	char pass[50];
	char mensaje[256];
	char destinatario[50];
};

list<paquete> listaDeMensajes;
pthread_mutex_t mutexLista = PTHREAD_MUTEX_INITIALIZER;

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	unsigned char buffer[sizeof(struct paquete)];
	int n;
	int newsockfd = (long)arg;
	bool abierto = true;

	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);

	//INSERTAR EN EL MAPA LOS USUARIOS DESDE UN CSV (*)
	map<string,string> mapa;
	ifstream file("usuarios.csv"); //aca va la ruta de argv

	while(!file.eof()) {
		string usr;
		string pass;

		getline(file,usr,';');
		getline(file,pass);

		if(file.eof())
			break;

		mapa.insert(std::pair<string,string>(usr,pass));
	}
	//IMPRIMIR MAPA (*)
	for(map<string,string>::iterator it = mapa.begin(); it != mapa.end(); ++it) {
		cout << (*it).first << " : " << (*it).second << endl;
	}
	cout << endl;
	//(*) ESTO SE HACE EN OTRO LADO

	while (abierto){
		struct paquete paqueteAEnviar;
		struct paquete paqueteRecibido;

		char buffTime[20];
		struct tm *sTm;

		//READ
		bzero(buffer,sizeof(struct paquete));
		n = read(newsockfd,buffer,sizeof(struct paquete));
		if (n < 0) cout << "Error en la lectura" << endl;

		memcpy(&paqueteRecibido, buffer, sizeof(struct paquete));

		map<string, string>::iterator it;

		time_t now = time (0);
		sTm = gmtime (&now);
		strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S ", sTm);

		switch(paqueteRecibido.tipo){
		case 1: //CASO AUTENTICACION
			//VALIDO USUARIO Y CONTRASEÑA
			it = mapa.find(paqueteRecibido.usuario); //PARA Q NO ME JODA
			if(it != mapa.end()) {
				if ( mapa[paqueteRecibido.usuario].compare(paqueteRecibido.pass) == 0){
					strcpy(paqueteAEnviar.mensaje,"Login Correcto");
					paqueteAEnviar.tipo = true;
					cout << paqueteRecibido.usuario << " inicio sesion correctamente" << endl;
					archLog << buffTime << paqueteRecibido.usuario << " inicio sesion correctamente" << endl;

				}else{
					strcpy(paqueteAEnviar.mensaje,"Contraseña Incorrecta");
					paqueteAEnviar.tipo = false;
					archLog << buffTime << paqueteRecibido.usuario << " ingreso una contraseña incorrecta" << endl;
				}
			}else{
				strcpy(paqueteAEnviar.mensaje,"Usuario Incorrecto");
				paqueteAEnviar.tipo = false;
				archLog << buffTime << " Se ingreso un usuario incorrecto" << endl;
			}

			//RESPONDO
			bzero(buffer,sizeof(struct paquete));
			memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));
			n = write(newsockfd,buffer, sizeof(struct paquete));
			if (n < 0){
				cout << "ERROR writing to socket" << endl;
				close(newsockfd);
				abierto = false;
			}

			break;
		case 2: {//CASO MENSAJES
			// LOCK EL MUTEX PARA AGREGAR EL PAQUETE ENTERO A LISTA DE MENSAJES
			int a = pthread_mutex_trylock(&mutexLista);
			if (a != 0) {
				cout<<"Otro hilo usando la lista"<<endl;
				}
			//pthread_mutex_lock (&mutexLista);
			sleep(30);
			listaDeMensajes.push_back(paqueteRecibido);
			pthread_mutex_unlock (&mutexLista);

			//cout << "De: " << paqueteRecibido.usuario << endl; //llamar campo1, campo2
			//cout << "Para: " << paqueteRecibido.pass << endl;  // lo hace mas general

			//Falta validar q el tamaño del mensaje sea el correcto
			strcpy(paqueteAEnviar.mensaje,"Mensaje recibido con exito");
			paqueteAEnviar.tipo = true;

			cout << "Mensaje: " << paqueteRecibido.mensaje << endl;
			archLog << buffTime << " Se envio un mensaje de: para: mensaje: " << paqueteRecibido.mensaje << endl;

			//RESPONDO
			bzero(buffer,sizeof(struct paquete));
			memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));
			n = write(newsockfd,buffer, sizeof(struct paquete));
			if (n < 0){
				cout << "ERROR writing to socket" << endl;
				close(newsockfd);
				abierto = false;
			}

			break;}
		case 3: //CASO DESCONECTAR
			abierto = false;
			cout << paqueteRecibido.usuario << " cerro sesion" << endl;
			archLog << buffTime << paqueteRecibido.usuario << " cerro sesion correctamente" << endl;
			//VER CUANDO SE CIERRA SESION FORZADAMENTE

			strcpy(paqueteAEnviar.mensaje,"sesion cerrada correctamente");
			paqueteAEnviar.tipo = true;

			//RESPONDO
			bzero(buffer,sizeof(struct paquete));
			memcpy(buffer, &paqueteAEnviar, sizeof(struct paquete));
			n = write(newsockfd,buffer, sizeof(struct paquete));
			if (n < 0){
				cout << "ERROR writing to socket" << endl;
				close(newsockfd);
				abierto = false;
			}

			break;
		}
	}

	archLog.close();
	//close(newsockfd);*/
	return NULL;
}

mySocketSrv :: mySocketSrv(char* puerto){

	this->puerto = atoi(puerto);
	cout << "PUERTO: " << this->puerto << endl;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout << "Error en la apertura del socket" << endl;
	bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(this->puerto);

}

void mySocketSrv::bindear(){
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		cout << "ERROR on binding" << endl;
}

void mySocketSrv::escuchar(){
	listen(sockfd,6);
	printf("Escuchando cliente...\n");
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
		if (newsockfd < 0)
			cout << "ERROR on accept" << endl;
		else {
			pthread_create(&thread, NULL, atender_cliente, (void*)(long)newsockfd);
			printf("Conectado!\n");
			printf("Leyendo mensaje...\n");
		}

	} /* end of while */
}

void mySocketSrv::enviarMensaje(string mensaje){

	char* cstr = new char [mensaje.length()+1];
	strcpy (cstr, mensaje.c_str());

	int n = write(sockfd,cstr,mensaje.length());
	if (n < 0)
		cout << "Error en la escritura" << endl;
}

void mySocketSrv::cerrar(){
	close(this->sockfd);
}

mySocketSrv::~mySocketSrv(){}
