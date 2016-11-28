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

	estaVivo = true;

	flip = 'D';
}

void DibujableServerEnemigo::setVida(int vida){
	this->vida = vida;
}

void DibujableServerEnemigo::setFuerza(int fuerza){
	this->fuerza = fuerza;
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

void DibujableServerEnemigo::setPunto(int punto){
	this->punto=punto;
}

int DibujableServerEnemigo::getVida(){
	return this->vida;
}

int DibujableServerEnemigo::getFuerza(){
   return this->fuerza;
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
int DibujableServerEnemigo::getPunto(){
	return this->punto;
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

bool DibujableServerEnemigo::dispararRifle(){
	if(spX == 0){
		spX += 1;
		return true;
	}

	spX += 1;
	if(spX > 9) {
		spX = 0;
		actuo = true;
		tiempoFinDisparo = time(NULL);
	}
	return false;
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
	spX += 1;
	if(spX > 3) spX = 3;
}

bool DibujableServerEnemigo::dispararBazooka(){
	if(spX == 0){
		spX += 1;
		return true;
	}

	spX += 1;
	if(spX == 2) {
		spX = 0;
		actuo = true;
		tiempoFinDisparo = time(NULL);
	}
	return false;
}

void DibujableServerEnemigo::moverseEnPantalla(int camaraX){
	spY = 6;
	spX += 1;
	if (spX == 2) spX = 0;

	if(x <= camaraX + 200){
		mVelX = 8;
		strcpy(&flip,"D");
	}
	else
	if(x >= camaraX + 400){
		mVelX = -8;
		strcpy(&flip,"I");
	}
}

void DibujableServerEnemigo::largarNuevoEnemigo(list<DibujableServerEnemigo> *listaEnemigos){
	DibujableServerEnemigo nuevo;
	nuevo.setId(ultimoId += 1); //ver q id le seteo...
	//char* spriteId = parseXMLPj();
	nuevo.setTipoEnemigo(5);
	nuevo.setX(x + 50);
	nuevo.setY(y);
	nuevo.setSpX(-1);
	nuevo.setSpY(5);
	nuevo.setFlip('D');
	nuevo.estaEnElPiso = false;
	nuevo.setFuerza(2);
	nuevo.setVida(1);
	nuevo.setPunto(20);

	listaEnemigos->push_back(nuevo);

	actuo = true;
	tiempoUltimoEnemigo = time(NULL);
}

void DibujableServerEnemigo::quieto(){
	mVelX = 0;

	caminaDerecha = false;
	caminaIzquierda = false;

	if (estaEnElPiso) spY = 1;
	else spY = 2;
	spX = 0;

}

void DibujableServerEnemigo::quitarEnergia (int golpe){
	this-> vida = vida - golpe;

}

bool DibujableServerEnemigo::mover(int camaraX, list<DibujableServerEnemigo> *listaEnemigos){
	//me muevo dependiendo de la camara y del tipo del tipo de enemigo
	/* 1)camina izquierda buscando pegarte (cuchillo)
	 * 2)dispara y se agacha
	 * 3)parado boludiando.. ve a player y le tira tiros
	 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
	 * 5)soldados jefe1
	 * 6)jefe1
	 */

	bool dispare = false;

	switch(this->tipoEnemigo){
	case 0:
		morir();
		break;

	case 10:
		morirBoss();
		break;

	case 1:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if(!actuo) ataqueSalto();
		}else caminarIzquierda();
		break;

	case 2:
		if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if (!actuo) dispare = dispararRifle();
			else if(tiempoFinDisparo+2 <= time(NULL)) {
				actuo = false;
			}
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
		//if(x <= camaraX + (800-80)){ // ancho pantalla - tam sprite
			if (!estaEnElPiso){
				spY = 4;
				caerHelicoptero();
			} else {
				spY = 5;
				if (!actuo) dispare = dispararBazooka();
				else if(tiempoFinDisparo+1 <= time(NULL)) {
					actuo = false;
				}
			}
		//}
		break;

	case 6:
		//jefe1
		moverseEnPantalla(camaraX);

		if (!actuo && x<=camaraX+(800-200)) {
			largarNuevoEnemigo(listaEnemigos);
		}
		else if(tiempoUltimoEnemigo+3 <= time(NULL)) {
			actuo = false;
		}

		break;

	default:
		break;
	}

	if (!estaEnElPiso){// && (y >= HMAX_SALTO)){
		//no esta en el piso entonces voy haciendolo caer (como una gravedad)
		mVelY += 3;
	}

	if(!estaEnElPiso && y <= HMAX_SALTOENEMIGO){
		mVelY = VELY_SUBIDAENEMIGO;
	}

	x += mVelX;
	y += mVelY;

	//ACA HAY Q PONERLO EN EL PISO SI SE PASA...
	if (!estaEnElPiso && y > 500) {
		mVelY = 0;
		y = 500; //ancho - 100
		//spY = 1;
		if(this->tipoEnemigo == 5) spX = -1; //buscar manera mas copada
		estaEnElPiso = true;
	}

	return dispare;
}

void  DibujableServerEnemigo::morir(){
	spX += 1;
	if (spX > 6){
		spX = 0;
		x = -100;
	}
}

void  DibujableServerEnemigo::morirBoss(){
	spX += 1;
	if (spX > 5){
		spX = 2;
		x = -100;
	}
}

bool  DibujableServerEnemigo::matar(){
	mVelX = 0;
	mVelY = 0;

	estaVivo = false;
	if (tipoEnemigo == 6){
		spY = 6;
		spX = 1;

		tipoEnemigo = 10; //muriendo boss

		return true;
		
	} else {
		spY = 8;
		spX = -1;

		tipoEnemigo = 0; //muriendo

		return false;
	}
	
}

bool DibujableServerEnemigo::estaVisible(int camaraX){
	return ( x >= camaraX && x <= (camaraX + 800) ); //PASARLE ANCHO DE LA VENTANA!!!!
}

DibujableServerEnemigo::~DibujableServerEnemigo() {
	// TODO Auto-generated destructor stub
}

