#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

class mensajeClass
{
public:
	char * usrAutor;
	char * usrDestino;
	char * mensaje;
	mensajeClass(char * autor, char * destino, char * mensajeEnviado);
	//~mensajeClass();
	
};

#endif