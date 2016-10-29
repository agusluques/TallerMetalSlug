#include "DibujableServer.h"

#include <cstring>
#include <string>

DibujableServer::DibujableServer() {
	estaEnElPiso = true;
	caminaDerecha = false;
	caminaIzquierda = false;
	mVelX = 0;
	mVelY = 0;
	conectar();

	flip = 'D';
}

void DibujableServer::setX(int pos) {
	x = pos;
}

void DibujableServer::setY(int pos) {
	y = pos;
}

void DibujableServer::setSpY(int pos) {
	this->spY = pos;
}

void DibujableServer::setSpX(int pos) {
	this->spX = pos;
}

void DibujableServer::setSpriteId(string idSprite) {
	//strcpy(spriteId,idSprite.c_str());
	spriteId = idSprite;
}

void DibujableServer::setId(int id) {
	this->id = id;
}

int DibujableServer::getX(){
	return this->x;
}
int DibujableServer::getY(){
	return this->y;
}
int DibujableServer::getSpX(){
	return this->spX;
}
int DibujableServer::getSpY(){
	return this->spY;
}
int DibujableServer::getId(){
	return this->id;
}

void DibujableServer::saltar(){
	if(estaEnElPiso){
		mVelY = -VELY_SUBIDA;
		//si estaba llendo para la derecha
		if (caminaDerecha) {
			mVelX = VELOCIDADX_AIRE;
			strcpy(&flip,"D");
		}
		//si estaba llendo para la izq
		if (caminaIzquierda) {
			mVelX = -VELOCIDADX_AIRE;
			strcpy(&flip,"I");
		}

		spY = 2;
		spX = 0;

		estaEnElPiso = false;
	}
}

void DibujableServer::caminarDerecha(){

	if (estaEnElPiso) {
		mVelX = VELOCIDAD_CAMINANDO;
		spY = 0;
		spX += 1;
		if (spX > 5) spX = 0;
	}
	else {
		mVelX = VELOCIDADX_AIRE;
		spY = 2;
		spX = 0;
	}

	caminaDerecha = true;
	caminaIzquierda = false;

	strcpy(&flip,"D");
}

void DibujableServer::caminarIzquierda(){
	if (estaEnElPiso) {
		mVelX = -VELOCIDAD_CAMINANDO;
		spY = 0;
		spX += 1;
		if (spX > 5) spX = 0;
	}
	else {
		mVelX = -VELOCIDADX_AIRE;
		spY = 2;
		spX = 0;
	}

	caminaDerecha = false;
	caminaIzquierda = true;

	strcpy(&flip,"I");
}

void DibujableServer::quieto(){
	mVelX = 0;

	caminaDerecha = false;
	caminaIzquierda = false;

	if (estaEnElPiso) spY = 1;
	else spY = 2;
	spX = 0;

}

bool DibujableServer::mover(){
	bool meMovi = false;

	if (!estaEnElPiso){// && (y >= HMAX_SALTO)){
		//no esta en el piso entonces voy haciendolo caer (como una gravedad)
		mVelY += 3;
	}

	if(mVelX != 0 || mVelY != 0){
		meMovi = true;
	}

	if(y <= HMAX_SALTO){
		mVelY = VELY_SUBIDA;
	}

	x += mVelX;
	y += mVelY;

	//ACA HAY Q PONERLO EN EL PISO SI SE PASA...
	if (y > 500) {
		mVelY = 0;
		y = 500;
		spY = 1;
		spX = 0;
		estaEnElPiso = true;
	}

	return meMovi;
}

void DibujableServer::volverAlPrincipio(){
	x = 0; 
	//mVelX = 0;
	//mVelY = 0;

}

bool DibujableServer::estaConectado(){
	return this->estaOnline;
}

void DibujableServer::conectar(){
	this->estaOnline = true;
}

void DibujableServer::desconectar(){
	this->estaOnline = false;
}

DibujableServer::~DibujableServer() {
	// TODO Auto-generated destructor stub
}

