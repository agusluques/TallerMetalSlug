#include "socket.hpp"
#include <pthread.h>

void recibirMensaje(int sockfd, void* buffer, int tamanio){
	int acumulador = 0, bytesRecibidos = 0;
	bool errorSocket = false;
	while ((acumulador < tamanio) && (errorSocket == false)){
		bytesRecibidos = read(sockfd, buffer, tamanio);
		if (bytesRecibidos == (-1)){
			cout << "Error al recibir datos SOCKET" << endl;
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

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	//char buffer[TAM_MAX];
    int newsockfd = (long)arg;
    bool abierto = true;

    while (abierto){

    	int tamanio = (sizeof(char));
    	char codigo;
    	recibirMensaje(newsockfd, &codigo, tamanio);
    	cout << "Codigo: " << codigo << endl;

    	switch(codigo){

    				case '0':
    					cout << "Entro a /0 que es para detectar la conexion" << endl;
    					cout << "Si imprime esto esta conectado todavia" << endl;
    					break;

    				case '1':
        				cout << "Entro a /1 que es conectar" << endl;
        				//Enviar Lista
        				break;

        			case '2':
        				cout << "Entro a /2 que es descsonectar" << endl;
        				abierto = false;
        				break;

        			case '3':
        				cout << "Entro a /3 que es salir" << endl;
						break;

        			case '4':
        			{
        				cout << "Entro a /4 que es enviar" << endl;
        				int tam, usr;
        				recibirMensaje(newsockfd, &usr, sizeof(int));
        				cout << "Numero de Usuario: " << usr << endl;
        				recibirMensaje(newsockfd, &tam, sizeof(int));
        				cout << "Tamanio del mensaje: " << tam << endl;
        				//char mensaje[(tam+1)];
        				char mensaje[tam];
        				recibirMensaje(newsockfd, &mensaje, sizeof(char)*tam);
        				//mensaje[tam] = '\n';
        				cout << "Mensaje: " << mensaje << endl;
        				//agregaraLista(int usr, char* msj);
        				break;
        			}
        			case '5':
        				cout << "Entro a /5 que es recibir" << endl;
        				//Mandarle los mensajes
        				break;

        			case '6':
        				cout << "Entro a /6 que es Lorem Ipsum" << endl;
        				//Recibir los mensajes
        				break;

        			default:
        				cout << "No deberia ingresar aca" << endl;
        				break;
        		}
     }
    //close(newsockfd);
    pthread_exit(NULL);
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
