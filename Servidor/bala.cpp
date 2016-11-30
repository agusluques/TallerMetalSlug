#include "bala.h"
#include <iostream>

using namespace std;

bala::bala(int xPos, int yPos, int usr, int id, int direccionDeDisparo, int tipoDeBala, int tipoEnemigo){
	this->id = id;
	this->x = xPos;
	this->y = yPos;
	this->direccionDisparo = direccionDeDisparo;
	this->tipoBala = tipoDeBala;
	this->spY = 0;
	this->tipoEnemigo= tipoEnemigo;
	visible = true;

	switch(direccionDisparo){
	//DIRECCIONES DE DISPAROS: der diagder arriba diagizq izq abajoizq abajo abajoder
	case 0:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x += 90;
			y -= 25;
		}

		xVel = 30;
		yVel = 0;

		break;
	case 1:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x += 70;
			y -= 120;
		}

		xVel = 30;
		yVel = -30;
		break;
	case 2:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x -= 10;
			y += 20;
		}
		if (tipoBala == 3){
			x -= 10;
			y -= 160;
		}
		if(tipoBala == 4){
			x += 50;
		}
		if (tipoBala == 5){
			x -= 10;
			y += 20;
		}

		xVel = 0;
		yVel = -30;
		break;
	case 3:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x -= 90;
			y -= 120;
		}

		xVel = -30;
		yVel = -30;
		break;
	case 4:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x -= 140;
			y -= 25;
		}

		xVel = -30;
		yVel = 0;
		break;
	case 5:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x -= 110;
			y += 90;
		}
		if (tipoBala == 7){
			x += 30;
			y += 20;
		}

		xVel = -30;
		yVel = 30;
		break;
	case 6:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x -= 10;
			y += 90;
		}
		if (tipoBala == 6){
			x += 30;
			y += 20;
		}

		xVel = 0;
		yVel = 30;
		break;
	case 7:
		if (tipoBala == 1){
			x += 30;
			y += 20;
		}
		if (tipoBala == 2){
			x += 30;
			y += 20;
		}
		if (tipoBala == 3){
			x += 90;
			y += 80;
		}
		if (tipoBala == 7){
			x += 30;
			y += 20;
		}

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

int bala::getTipo(){
	return this->tipoEnemigo;
}

void bala::setTipo(int tipo){
	this-> tipoEnemigo = tipo;
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
	return ((x >= camaraX && x <= (camaraX + 800)) && ((y > 0) && (y < 600)) ); //PASARLE ANCHO DE LA VENTANA!!!!
}

void bala::mover(){
	/*
	 * 0 pistolita
	 * 1 H
	 * 2 R
	 * 3 S
	 */
	//aumentar velocidad de R..
	if (tipoBala == 3){
		spY += 1;
		if (spY > 5) {
			spY = 6;
			x = -100;
		}

		if(xVel != 0) x+= xVel/10;
		if(yVel != 0) y+= yVel/10;

	} else {
		x += xVel;
		y += yVel;
	}
}

bala::~bala(){}
