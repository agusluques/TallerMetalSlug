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
#include <arpa/inet.h>
#include <vector>
#include <pthread.h>

#include "Grafica.h"

using namespace std;

class mySocket{
	private:
		int sockfd;
		int puerto;
		struct sockaddr_in server;
		//string address;
		//struct hostent* server;
		bool conectado;
		Grafica grafica;
	public:
		int numeroCliente;

		mySocket(char* puerto, char* IP);
		bool autenticar(string nombre);
		void conectar();
		int getFD();
		void enviarMensaje();
		void enviarMensaje(int usuario, char* mensaje, int tamanio);
		bool enviarMensaje(void* mensaje, int tamanioMensaje);
		bool recibirMensaje(bool &pasarNivel);
		bool recibirMensaje(void* buffer, int tamanio);
		int cantidadUsuariosDisponibles();
		void pedirUsuarios();
		void cerrar();
		void cerrarGrafica();
		void desconectar();
		bool conexion();

		void cargarDibujables();
		bool iniciarGrafica(bool &pasarNivel);
		~mySocket();
};



#endif /* SOCKET_H_ */
