#ifndef MENSAJECLASS_H_
#define MENSAJECLASS_H_

using namespace std;

class mensajeClass
{
private:
	int idusrAutor;
	char usrAutor[50];
	int usrDestino;
	int idObjeto;
	int x;
	int y;
	int spX;
	int spY;
	int score;

	int tipoMensaje;
	string mensaje;
public:
	int tipoDibujable;
	int avanzar;
	char flip;

	mensajeClass();
	mensajeClass(char * autor, int destino, int id, int x, int y, int spx, int spy, char flip, bool avanzar, int tipoDibujable);
	mensajeClass(char * autor, int destino, int id, int x, int y, int spx, int spy, char flip, int tipo);
	mensajeClass(int tipo, int destino, string mensajeEnviado);
	mensajeClass(int tipo, int destino);
	mensajeClass(int tipo, int destino, int autor, int score);
	mensajeClass(int aux, int  destino, int id, int x, int y, int spx, int spy, char flip, int tipo);

	int nombreDestinatario();
	std::string nombreAutor();
	int numAutor();
	int getidObjeto();
	int getX();
	int getY();
	int getSpX();
	int getSpY();
	int getScore();
	int getEnergia();
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
