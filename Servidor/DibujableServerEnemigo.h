#ifndef DIBUJABLESERVERENEMIGO_H_
#define DIBUJABLESERVERENEMIGO_H_

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>

using namespace std;

static int VELOCIDADX_AIREENEMIGO = 18;
static int VELOCIDAD_CAMINANDOENEMIGO = 12;
static int HMAX_SALTOENEMIGO = 300;
static int VELY_SUBIDAENEMIGO = 30;

class DibujableServerEnemigo {
public:
	int id;
	int tipoEnemigo;
	int x;
	int y;
	int spX;
	int spY;
	bool actuo;
	bool salto;

	time_t tiempoFinDisparo;

	bool estaEnElPiso;
	bool caminaDerecha;
	bool caminaIzquierda;
	char flip;

	float mVelX;
	float mVelY;

	DibujableServerEnemigo();
	void setX(int pos);
	void setY(int pos);
	void setSpX(int pos);
	void setSpY(int pos);
	void setTipoEnemigo(int tipo);
	void setId(int id);
	void setFlip(char flip);
	int getX();
	int getY();
	int getSpX();
	int getSpY();
	int getId();

	void saltar();
	void caminar(int camaraX);
	void caminarDerecha();
	void caminarIzquierda();
	void ataqueSalto();
	void dispararRifle();
	void huirDerecha();
	void caerHelicoptero();
	void dispararBazooka();
	void quieto();
	bool mover(int camaraX);
	void volverAlPrincipio();

	int velocidadXJugador();

	bool estaConectado();
	void conectar();
	void desconectar();

	bool estaVisible(int camaraX);

	virtual ~DibujableServerEnemigo();
};

#endif /* DIBUJABLESERVERENEMIGO_H_ */
