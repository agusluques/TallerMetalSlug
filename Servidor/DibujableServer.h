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
	bool estaOnline;
	bool estaDisparando;

	int tipoDeArma;

	Escenario escenario;

	bool estaEnPlataforma;
	bool estaEnElPiso;
	bool caminaDerecha;
	bool caminaIzquierda;
	char flip;

	float mVelX;
	float mVelY;

	DibujableServer();
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

	void saltar();
	void caminarDerecha();
	void caminarIzquierda();
	void quieto();
	bool mover(int camaraX);
	void volverAlPrincipio();
	bool disparar();
	void choca();
	void actualizarBonus(int tipoBonus);

	int velocidadXJugador();

	bool estaConectado();
	void conectar();
	void desconectar();

	virtual ~DibujableServer();
};

#endif /* DIBUJABLESERVER_H_ */
