#ifndef USUARIOCLASS_H_
#define USUARIOCLASS_H_

#include <string>

class usuarioClass
{
public:
	char nombre[50];
	char pass[50];
	bool conectado;
	usuarioClass(char * usuario, char * contrasenia);
	bool estaConectado();
	bool validarUsuario(char *contrasenia, char *mensaje);
	std::string nombreUsuario();
	//~usuarioClass();
	
};

#endif
