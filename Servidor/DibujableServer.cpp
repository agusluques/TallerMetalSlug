#include "DibujableServer.h"

#include <cstring>
#include <string>

DibujableServer::DibujableServer(bool modoDios, Escenario *escenario) {
	estaEnElPiso = true;
	caminaDerecha = false;
	caminaIzquierda = false;
	estaEnPlataforma = false;
	estaDisparando = false;
	mVelX = 0;
	mVelY = 0;
	conectar();
	basePlataforma = 0;
	this->escenario= escenario;

	score = 0;
	if(modoDios){
		vida = -10000;
	} else {
		vida = 0;
		vidaAnterior = 0;
	}

	tipoDeArma = 1;
	cantBalas = 200;

	estaVivo = true;

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
int DibujableServer::getScore(){
	return this->score;
}

void DibujableServer::aumentarDano (int cantidad){
	this->vidaAnterior = vida;
	this->vida += cantidad;
	if(vida > 10){
		vida=10;
	}
}

void DibujableServer::cancheriar(){
	spX = 0;
	spY = 9;
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
		if (escenario->colisionPlataforma(this->x,this->y,this->caminaIzquierda,this->caminaDerecha) && !estaEnPlataforma){
			this->choca();
		}
	}
}

bool DibujableServer::disparar(int direccionDisparo){
	bool rta = false;

	if((cantBalas == 0) && (tipoDeArma != 1)){
		tipoDeArma = 1;
		cantBalas = 200;
	}

	if((cantBalas > 0)&&(!estaDisparando)){
		cantBalas--;

		if(!estaDisparando){
			estaDisparando = true;
			rta = true;

			if(direccionDisparo == -1){
				if(flip == 'D') direccionDisparo = 0;
				else direccionDisparo = 4;
			}

			switch(direccionDisparo){
			case 0:
				spX = 0;
				spY = 3;
				spXinicial = 0;
				spXaux = 0;
				spYaux = 3;

				break;

			case 1:
				spX = 1;
				spY = 7;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 7;

				break;

			case 2:
				spX = 1;
				spY = 4;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 4;

				break;

			case 3:
				spX = 1;
				spY = 7;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 7;
				strcpy(&flip,"I");

				break;

			case 4:
				spX = 0;
				spY = 3;
				spXinicial = 0;
				spXaux = 0;
				spYaux = 3;

				break;

			case 5:
				spX = 1;
				spY = 8;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 8;
				strcpy(&flip,"I");
				break;

			case 6:
				//				if(!estaEnElPiso){
				spX = 1;
				spY = 5;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 5;

				//					estaDisparando = false;
				//				}
				break;

			case 7:
				spX = 1;
				spY = 8;
				spXinicial = 1;
				spXaux = 1;
				spYaux = 8;
				break;

			}
		}
		return rta;

	}else
		return rta;

}

void DibujableServer::matar(){
	mVelX = 0;
	mVelY = 0;

	estaVivo = false;
	spY = 6;
	spX = -1;
}

void DibujableServer::morir(){
	spX += 1;
	if (spX > 7){
		spX = 7;
	}
}

int DibujableServer::velocidadXJugador(){
	return mVelX;
}


void DibujableServer::apuntarArriba(){
	spY = 4;
	spX = 0;

	apunta = true;
}

void DibujableServer::apuntarAbajo(){
	if(!estaEnElPiso){
		spY = 5;
		spX = 0;

		apunta = true;
	}
}

void DibujableServer::apuntarDiagDer(){
	spY = 7;
	spX = 1;

	apunta = true;
}

void DibujableServer::apuntarDiagIzq(){
	spY = 7;
	spX = 1;

	apunta = true;

	flip = 'D';
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

void DibujableServer::choca(){

	mVelY = -16;
}

bool DibujableServer::mover(int xCamara){
	bool meMovi = false;

	if (!estaEnElPiso){// && (y >= HMAX_SALTO)){
		//no esta en el piso entonces voy haciendolo caer (como una gravedad)
		mVelY += 3;
	}

	if(mVelX != 0 || mVelY != 0 || estaDisparando){
		meMovi = true;
	}

	if(!estaVivo){
		morir();
		meMovi = true;
	}

	if (apunta) {
		meMovi = true;
		apunta = false;
	}

	if(estaDisparando) {
		spXaux += 1;

		if( (mVelX == 0 && mVelY == 0) || !estaEnElPiso){
			spX = spXaux;
			spY = spYaux;
		}

		if(spXaux >= spXinicial + 3) {
			if(spXaux == spXinicial + 5)
				estaDisparando = false;

			spX = spXinicial;
		}
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
		if (escenario->verificarPlataforma(this->x,this->y)){
			mVelY = 0;
			y = NIVEL_PLAT;
			spY = 1;
			//spX = 0;
			estaEnPlataforma = true;
		}
	}

	//una vez arriba de la plataforma, verifico sino me sali
	if (estaEnPlataforma){
		if (escenario->salirPlataforma(this->x,this->y)){
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

void DibujableServer::actualizarBonus(int tipoBonus){
	switch(tipoBonus){
	case 0:
		//0- recargar arma 1
		tipoDeArma = 1;
		cantBalas += 150;

		break;
	case 1:
		//1- pasar a arma 2
		tipoDeArma = 2;
		cantBalas = 30;

		break;
	case 2:
		//2- recargar arma 2
		if(tipoDeArma == 2){
			cantBalas += 15;
		}
		break;
	case 3:
		//3- pasar a arma 3
		tipoDeArma = 3;
		cantBalas = 30;

		break;
	case 4:
		//4- recargar arma 3
		if(tipoDeArma == 3){
			cantBalas += 10;
		}
		break;
	case 5:
		//5- bonus de vida
		if(vida > 0){
			vida = 0;
		}
		break;
	case 6:
		//6- bonus de killall
		break;

	}

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

