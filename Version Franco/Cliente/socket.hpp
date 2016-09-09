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
#include <cstring>
#include <fstream>
#include<arpa/inet.h>

using namespace std;

class mySocket{
	private:
		int sockfd;
		int puerto;
		struct sockaddr_in server;
		//string address;
		//struct hostent* server;

	public:
		mySocket(char* puerto, char* IP);
		void conectar();
		void enviarMensaje();
		void enviarMensaje(string mensaje);
		void enviarMensaje(int usuario, char* mensaje, int tamanio);
		void enviarMensaje(void* mensaje, int tamanioMensaje);
		void recibirMensaje();
		void recibirMensaje(void* buffer, int tamanio);
		void cerrar();
		~mySocket();
};



#endif /* SOCKET_H_ */
