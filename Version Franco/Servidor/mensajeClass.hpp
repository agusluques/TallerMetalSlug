#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

class mensajeClass
{
public:
	char usrAutor[50];
	char usrDestino[50];
	char mensaje[256];
	mensajeClass(char * autor, char * destino, char * mensajeEnviado);
	//~mensajeClass();
	
};

#endif
