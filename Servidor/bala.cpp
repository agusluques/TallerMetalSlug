#include "bala.h"

bala::bala(int xPos, int yPos, int usr, int id, int direccionDeDisparo, int tipoDeBala){
	this->id = id;
	this->x = xPos;
	this->y = yPos;
	this->direccionDisparo = direccionDeDisparo;
	this->tipoBala = tipoDeBala;
	this->spY = 0;

	switch(tipoBala){
	case 0:
		x += 30;
		y += 20;
		break;

	case 1:
		x += 30;
		y += 20;
		break;

	case 2:
		x += 30;
		y += 20;
		break;

	case 3:
		x += 60;
		y -= 30;
		break;
	}

	switch(direccionDisparo){
	//DIRECCIONES DE DISPAROS: der diagder arriba diagizq izq abajoizq abajo abajoder
	case 0:
		xVel = 30;
		yVel = 0;
		break;
	case 1:
		xVel = 30;
		yVel = -30;
		break;
	case 2:
		xVel = 0;
		yVel = -30;
		break;
	case 3:
		xVel = -30;
		yVel = -30;
		break;
	case 4:
		xVel = -30;
		yVel = 0;
		break;
	case 5:
		xVel = -30;
		yVel = 30;
		break;
	case 6:
		xVel = 0;
		yVel = -30;
		break;
	case 7:
		xVel = 30;
		yVel = 30;
		break;
	}

	this->usr = usr;
}

int bala::getPosX(){
	return this->x;
}

int bala::getPosY(){
	return this->y;
}

float bala::getVelX(){
	return this->xVel;
}

float bala::getVelY(){
	return this->yVel;
}

int bala::getUsr(){
	return this->usr;
}

int bala::getId(){
	return this->id;
}

void bala::setPosX(int x){
	this->x = x;
}

void bala::setPosY(int y){
	this->y = y;
}

void bala::setVelX(int vel){
	this->xVel = vel;
}

void bala::setVelY(int vel){
	this->yVel = vel;
}

void bala::setUsr(int usr){
	this->usr = usr;
}

bool bala::estaVisible(int camaraX){
	return ( x >= camaraX && x <= (camaraX + 800) ); //PASARLE ANCHO DE LA VENTANA!!!!
}

void bala::mover(){
	/*
	 * 0 pistolita
	 * 1 H
	 * 2 R
	 * 3 S
	 */
//aumentar velocidad de R..
	if (tipoBala != 3){
		x += xVel;
		y += yVel;
	}else{
		spY += 1;
		if (spY > 5) {
			spY = 6;
			x = -100;
		}
	}

}

bala::~bala(){}
