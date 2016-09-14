#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

class mensajeClass
{
private:
	char usrAutor[50];
	char usrDestino[50];
	//char mensaje[256];
	std::string mensaje;
public:

	mensajeClass();
	mensajeClass(char * autor, char * destino, char * mensajeEnviado);
	std::string nombreDestinatario();
	std::string nombreAutor();
	std::string getMensaje();
	//~mensajeClass();
	
};

#endif
