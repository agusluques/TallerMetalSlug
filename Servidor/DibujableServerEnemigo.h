#ifndef DIBUJABLESERVERENEMIGO_H_
#define DIBUJABLESERVERENEMIGO_H_

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <list>

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

	bool estaVivo;

	int ultimoId;

	time_t tiempoFinDisparo;
	time_t tiempoUltimoEnemigo;

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
	bool dispararRifle();
	void huirDerecha();
	void caerHelicoptero();
	bool dispararBazooka();
	void moverseEnPantalla(int camaraX);
	void largarNuevoEnemigo(list<DibujableServerEnemigo> *listaEnemigos);
	void quieto();
	bool mover(int camaraX, list<DibujableServerEnemigo> *listaEnemigos);
	void matar();
	void morir();
	void volverAlPrincipio();

	int velocidadXJugador();

	bool estaConectado();
	void conectar();
	void desconectar();

	bool estaVisible(int camaraX);

	virtual ~DibujableServerEnemigo();
};

#endif /* DIBUJABLESERVERENEMIGO_H_ */
