#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

class mensajeClass
{
private:
	char usrAutor[50];
	char usrDestino[50];
	int idObjeto;
	int x;
	int y;
public:
	mensajeClass();
	mensajeClass(char * autor, char * destino, int id, int x, int y);
	std::string nombreDestinatario();
	std::string nombreAutor();
	int getidObjeto();
	int getX();
	int getY();
	//~mensajeClass();
	
};

#endif
