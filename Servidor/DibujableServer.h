#ifndef DIBUJABLESERVER_H_
#define DIBUJABLESERVER_H_

#include <iostream>
#include <cstring>
#include <string>
#include "Escenario.h"

using namespace std;

static int VELOCIDADX_AIRE = 13;
static int VELOCIDAD_CAMINANDO = 15;
static int HMAX_SALTO = 200;
static int VELY_SUBIDA = 30;
static int NIVEL_PLAT = 360;
static int NIVEL_PISO = 500;


class DibujableServer {
public:
	int basePlataforma;
	int id;
	string spriteId;
	int x;
	int y;
	int spX;
	int spY;
	int spXaux;
	int spYaux;
	int spXinicial;
	bool estaOnline;
	bool estaDisparando;
	bool apunta;
	bool estaVivo;
	
	int score;
	int vida;
	int tipoDeArma;
	int cantBalas;

	Escenario *escenario;

	bool estaEnPlataforma;
	bool estaEnElPiso;
	bool caminaDerecha;
	bool caminaIzquierda;
	char flip;

	float mVelX;
	float mVelY;

	DibujableServer(bool modoDios, Escenario *escenario);
	void setX(int pos);
	void setY(int pos);
	void setSpX(int pos);
	void setSpY(int pos);
	void setSpriteId(string idSprite);
	void setId(int id);
	int getX();
	int getY();
	int getSpX();
	int getSpY();
	int getId();
	int getScore();

	void aumentarDano (int cantidad);

	void cancheriar();
	void saltar();
	void apuntarArriba();
	void apuntarAbajo();
	void apuntarDiagDer();
	void apuntarDiagIzq();
	void caminarDerecha();
	void caminarIzquierda();
	void quieto();
	bool mover(int camaraX);
	void volverAlPrincipio();
	bool disparar(int direccionDisparo);
	void matar();
	void morir();
	void choca();
	void actualizarBonus(int tipoBonus);

	int velocidadXJugador();

	bool estaConectado();
	void conectar();
	void desconectar();

	virtual ~DibujableServer();
};

#endif /* DIBUJABLESERVER_H_ */
