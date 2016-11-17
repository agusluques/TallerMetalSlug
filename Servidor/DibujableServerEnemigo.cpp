#include "DibujableServerEnemigo.h"

#include <cstring>
#include <string>

DibujableServerEnemigo::DibujableServerEnemigo() {
	estaEnElPiso = true;
	caminaDerecha = false;
	caminaIzquierda = false;
	mVelX = 0;
	mVelY = 0;

	actuo = false;
	salto = false;

	flip = 'D';
}

void DibujableServerEnemigo::setX(int pos) {
	x = pos;
}

void DibujableServerEnemigo::setY(int pos) {
	y = pos;
}

void DibujableServerEnemigo::setSpY(int pos) {
	this->spY = pos;
}

void DibujableServerEnemigo::setSpX(int pos) {
	this->spX = pos;
}

void DibujableServerEnemigo::setTipoEnemigo(int tipo) {
	//strcpy(spriteId,idSprite.c_str());
	this->tipoEnemigo = tipo;
}

void DibujableServerEnemigo::setId(int id) {
	this->id = id;
}

void DibujableServerEnemigo::setFlip(char flip){
	this->flip = flip;
}

int DibujableServerEnemigo::getX(){
	return this->x;
}
int DibujableServerEnemigo::getY(){
	return this->y;
}
int DibujableServerEnemigo::getSpX(){
	return this->spX;
}
int DibujableServerEnemigo::getSpY(){
	return this->spY;
}
int DibujableServerEnemigo::getId(){
	return this->id;
}

void DibujableServerEnemigo::saltar(){
	if(estaEnElPiso){
		mVelY = -VELY_SUBIDAENEMIGO;
		//si estaba llendo para la derecha
		if (caminaDerecha) {
			mVelX = VELOCIDADX_AIREENEMIGO;
			strcpy(&flip,"D");
		}
		//si estaba llendo para la izq
		if (caminaIzquierda) {
			mVelX = -VELOCIDADX_AIREENEMIGO;
			strcpy(&flip,"I");
		}

		spY = 2;
		spX = 0;

		estaEnElPiso = false;
	}
}

void DibujableServerEnemigo::caminar(int camaraX){
	spY = 7;
	spX += 1;
	if (spX > 11) spX = 0;

	if(x <= camaraX + 200){
		mVelX = VELOCIDAD_CAMINANDOENEMIGO;
		strcpy(&flip,"I");
	}else if(x >= camaraX + 700){
		mVelX = -VELOCIDAD_CAMINANDOENEMIGO;
		strcpy(&flip,"D");
	}
}

void DibujableServerEnemigo::caminarDerecha(){
	if (estaEnElPiso) {
		mVelX = VELOCIDAD_CAMINANDOENEMIGO;
		spY = 7;
		spX += 1;
		if (spX > 11) spX = 0;
	}
	else {
		mVelX = VELOCIDADX_AIREENEMIGO;
		spY = 2;
		spX = 0;
	}

	caminaDerecha = true;
	caminaIzquierda = false;

	strcpy(&flip,"I");
}

void DibujableServerEnemigo::caminarIzquierda(){
	if (estaEnElPiso) {
		mVelX = -VELOCIDAD_CAMINANDOENEMIGO;
		spY = 7;
		spX += 1;
		if (spX > 11) spX = 0;
	}
	else {
		mVelX = -VELOCIDADX_AIREENEMIGO;
		spY = 2;
		spX = 0;
	}

	caminaDerecha = false;
	caminaIzquierda = true;

	strcpy(&flip,"D");
}

void DibujableServerEnemigo::ataqueSalto(){
	if(!salto){
		mVelY = -VELY_SUBIDAENEMIGO + 5;
		mVelX = -VELOCIDADX_AIREENEMIGO - 5;
		salto = true;
		spY = 2;
		spX = 0;
		estaEnElPiso = false;
		strcpy(&flip,"D");
	}else{
		spX += 1;
		if (spX == 13) spX = 12;

		if (estaEnElPiso){
			mVelX = 0;
			actuo = true;
		}
	}
}

void DibujableServerEnemigo::dispararRifle(){
	spX += 1;
	if (spX > 9){
		//if (cantidadTiros == cantidadTirosActual)
		//	actuo = true; //para dejar de disparar
		//	spX = 9;
		//else cantidadTirosActual += 1;
		spX = 0; //para volver a disparar..
	}
}

void DibujableServerEnemigo::huirDerecha(){
	//se asusta y dsps corre para la derecha
	spX += 1;

	if (spX > 2){
		if(spY == 1){
			spY = 0;
			spX = -1;
		}
		mVelX = VELOCIDAD_CAMINANDOENEMIGO;
		spY = 0;
		spX += 1;
		if (spX > 5) spX = 0;
	}

	caminaDerecha = false;
	caminaIzquierda = true;

	strcpy(&flip,"D");
}

void DibujableServerEnemigo::caerHelicoptero(){
	if(y > 400){
		spX += 1;
		if(spX == 4) spX = 3;
	}
	mVelY -= 1;
}

void DibujableServerEnemigo::dispararBazooka(){
	spX += 1;
	if(spX == 2) {
		spX = 0;
		actuo = true;
		tiempoFinDisparo = time(NULL);
	}
}

void DibujableServerEnemigo::quieto(){
	mVelX = 0;

	caminaDerecha = false;
	caminaIzquierda = false;

	if (estaEnElPiso) spY = 1;
	else spY = 2;
	spX = 0;

}

bool DibujableServerEnemigo::mover(int camaraX){
	//me muevo dependiendo de la camara y del tipo del tipo de enemigo
	/* 1)camina izquierda buscando pegarte
	 * 2)dispara y se agacha
	 * 3)parado boludiando.. ve a player y le tira tiros
	 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
	 * 5)soldados jefe1
	 */
	switch(this->tipoEnemigo){
	case 1:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if(!actuo) ataqueSalto();
		}else caminarIzquierda();
		break;

	case 2:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if(!actuo) dispararRifle(); //pasarle cantidad de tiros...
		}else{
			strcpy(&flip,"D");
			spX = 0;
			spY = 3;
		}
		break;

	case 3:
		//camina y vuelve hasta ancho pantalla..
		caminar(camaraX);
		break;

	case 4:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			huirDerecha();
			//MATARLO CUANDO DESAPARECE XQ SINO QUEDA TRABADO EN LA PANTALLA..
		}else quieto();
		break;

	case 5:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if (!estaEnElPiso){
				spY = 4;
				caerHelicoptero();
			} else {
				spY = 5;
				if (!actuo) dispararBazooka();
				else if(tiempoFinDisparo+1 <= time(NULL)) {
					actuo = false;
					estaEnElPiso = false;
					y = 100;
				}
			}
		}
		break;

	case 6:
		//jefe1
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			spY = 6;
			spX += 1;
			if (spX == 2) spX = 0;
		}
		break;

	default:
		break;
	}

	bool meMovi = false;

	if (!estaEnElPiso){// && (y >= HMAX_SALTO)){
		//no esta en el piso entonces voy haciendolo caer (como una gravedad)
		mVelY += 3;
	}

	if(mVelX != 0 || mVelY != 0){
		meMovi = true;
	}else return false;

	if(y <= HMAX_SALTOENEMIGO){
		mVelY = VELY_SUBIDAENEMIGO;
	}

	x += mVelX;
	y += mVelY;

	//ACA HAY Q PONERLO EN EL PISO SI SE PASA...
	if (y > 500) {
		mVelY = 0;
		y = 500; //ancho - 100
		//spY = 1;
		//spX = -1;
		estaEnElPiso = true;
	}

	return meMovi;
}

bool DibujableServerEnemigo::estaVisible(int camaraX){
	return ( x >= camaraX && x <= (camaraX + 800) ); //PASARLE ANCHO DE LA VENTANA!!!!
}

DibujableServerEnemigo::~DibujableServerEnemigo() {
	// TODO Auto-generated destructor stub
}

