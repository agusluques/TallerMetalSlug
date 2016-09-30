#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

class mensajeClass
{
private:
	char usrAutor[50];
	char usrDestino[50];
	//char mensaje;
	std::string mensaje;
	int idObjeto;
public:
	mensajeClass();
	mensajeClass(char * autor, char * destino, char * mensajeEnviado, int id);
	std::string nombreDestinatario();
	std::string nombreAutor();
	std::string getMensaje();
	int getidObjeto();
	//~mensajeClass();
	
};

#endif
