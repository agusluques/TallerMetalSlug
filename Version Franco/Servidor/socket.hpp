#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>
#define TAM_MAX 8192

using namespace std;

class mySocketSrv{
	private:
		int sockfd;
		int puerto;
		struct sockaddr_in serv_addr;
		int numeroCliente;

	public:
		
		mySocketSrv(char* archusr, char* puerto);
		void bindear();
		void escuchar();
		void enviarMensaje(string mensaje);
		//void recibirMensaje(void* buffer, int tamanio);
		void aceptarClientes();
		void cerrar();
		void setNumeroCliente(int usr);
		int getNumeroCliente();
		~mySocketSrv();
};



#endif /* SOCKET_H_ */
