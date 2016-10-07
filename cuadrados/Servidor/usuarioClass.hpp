#ifndef USUARIOCLASS_H_
#define USUARIOCLASS_H_

#include <string>
#include <fstream>

class usuarioClass
{
private:
	char nombre[50];
	bool conectado;
	int numeroCliente;
public:
	usuarioClass();

	usuarioClass(char *usuario, int numCliente);
	bool estaConectado();
	void conectar();
	void desconectar();
	std::string nombreUsuario();
	int numCliente(){
		return this->numeroCliente;
	}

	void loggear(std::string mensaje);

	~usuarioClass();
};

#endif
