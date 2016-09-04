#include "socket.hpp"
#include <pthread.h>

void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	char buffer[TAM_MAX];
    int n;
    int newsockfd = (long)arg;
    bool abierto = true;

    while (abierto){
        //READ
        bzero(buffer,TAM_MAX);
        n = read(newsockfd,buffer,TAM_MAX-1);
        if (n < 0){
        	cout << "ERROR, no se pudo leer el mensaje" << endl;
        } else {
        	if(buffer[0] == '/'){
        		abierto = false;
        		cout << "ENTRO A LO QUE ESTOY PROBANDO!!!!" << endl;
        	}
            cout << "Mensaje:" << buffer << endl;
        }

        //WRITE
        char msg[256]="Mensaje recibido por el servidor\n";
        n = write(newsockfd,msg,strlen(msg));
        if (n < 0){
        	cout << "ERROR writing to socket" << endl;
			close(newsockfd);
			abierto = false;
        }
     }
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
