#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

using namespace std;

class mensajeClass
{
private:
	char usrAutor[50];
	char usrDestino[50];
	int idObjeto;
	int x;
	int y;
	int spX;
	int spY;
	bool avanzar;

	int tipoMensaje;
	string mensaje;
public:
	char flip;

	mensajeClass();
	mensajeClass(char * autor, char * destino, int id, int x, int y, int spx, int spy, char flip, bool avanzar);
	mensajeClass(int tipo, char *destino, string mensajeEnviado);
	std::string nombreDestinatario();
	std::string nombreAutor();
	int getidObjeto();
	int getX();
	int getY();
	int getSpX();
	int getSpY();
	int getBoolAvanzar();

	int getTipoMensaje(){
		return tipoMensaje;
	}
	string getMensaje(){
		return mensaje;
	}

	//~mensajeClass();
	
};

#endif
