#ifndef DIBUJABLESERVER_H_
#define DIBUJABLESERVER_H_

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

static int VELOCIDADX_AIRE = 8;
static int VELOCIDAD_CAMINANDO = 10;
static int VELOCIDAD_JUGADOR;
static int HMAX_SALTO = 300;
static int VELY_SUBIDA = 30;

class DibujableServer {
public:
	int id;
	string spriteId;
	int x;
	int y;
	int spX;
	int spY;
	bool estaOnline;

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
	bool mover();
	void volverAlPrincipio();

	bool estaConectado();
	void conectar();
	void desconectar();

	virtual ~DibujableServer();
};

#endif /* DIBUJABLESERVER_H_ */
