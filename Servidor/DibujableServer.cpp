#include "DibujableServer.h"

#include <cstring>
#include <string>

DibujableServer::DibujableServer() {
	estaEnElPiso = true;
	caminaDerecha = false;
	caminaIzquierda = false;
	estaEnPlataforma = false;
	mVelX = 0;
	mVelY = 0;
	conectar();
	basePlataforma = 0;

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

int DibujableServer::velocidadXJugador(){
	return mVelX;
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

bool DibujableServer::mover(int xCamara){
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
	//para q no se valla de la pantalla...
	if ( (x < xCamara) || (x > xCamara+(800-80)) ) x -= mVelX; //lo vuelvo a como estaba antes..
	y += mVelY;


	//verifico que no este en una plataforma
	if (!estaEnPlataforma){
       if (escenario.verificarPlataforma(this->x,this->y)){
    	   mVelY = 0;
		   y = NIVEL_PLAT;
           spY = 1;
		   spX = 0;
		   estaEnPlataforma = true;
       }
	}

	//una vez arriba de la plataforma, verifico sino me sali
	if (estaEnPlataforma){
      if (escenario.salirPlataforma(this->x,this->y)){
		estaEnPlataforma = false;
		estaEnElPiso = false;
      }else{ // lo pongo de nuevo en la plataforma si se pasa.
          if(y > NIVEL_PLAT){
        	  mVelY = 0;
        	  y = NIVEL_PLAT;
      		  spY = 1;
      		  spX = 0;
        	  estaEnElPiso = true;
          }

      }
	}


	//ACA HAY Q PONERLO EN EL PISO SI SE PASA...
	if ( y > NIVEL_PISO) {
		mVelY = 0;
		y = NIVEL_PISO;
		spY = 1;
		spX = 0;
		estaEnElPiso = true;
	}

	return meMovi;
}

void DibujableServer::volverAlPrincipio(){
	x = x - 8300;
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

