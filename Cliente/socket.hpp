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
#include <vector>

using namespace std;

class mySocket{
	private:
		int sockfd;
		int puerto;
		char* IP;
		struct sockaddr_in serv_addr;
		struct hostent* server;

	public:
		mySocket(char* puerto, char* IP);
		bool autenticar();
		bool conectar();
		void desconectar();
		void cargarUsuariosDisponibles(vector<string> *usuariosDisponibles);
		void mostrarUsuariosDisponibles(vector<string> usuariosDisponibles);
		void enviarMensaje();
		void enviarMensaje(int usuario, char* mensaje, int tamanio);
		void cerrar();
		~mySocket();
};



#endif /* SOCKET_H_ */
