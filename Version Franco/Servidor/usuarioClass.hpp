#ifndef USUARIOCLASS_H_
#define USUARIOCLASS_H_

#include <string>
#include <fstream>

class usuarioClass
{
public:
	char nombre[50];
	char pass[50];
	bool conectado;
	usuarioClass();
	usuarioClass(char * usuario, char * contrasenia);
	bool estaConectado();
	void desconectar();
	bool validarUsuario(char *contrasenia, char *mensaje);
	void loggear(std::string mensaje);
	std::string nombreUsuario();
	// ~usuarioClass();
	
};

#endif
