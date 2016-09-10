#ifndef USUARIOCLASS_H_
#define USUARIOCLASS_H_

class usuarioClass
{
public:
	char * nombre;
	char * pass;
	bool conectado;
	usuarioClass(char * usuario, char * contraseña);
	//~usuarioClass();
	
};

#endif