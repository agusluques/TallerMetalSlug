#include "bala.h"

bala::bala(int xPos, int yPos, int usr, bool dir, int id){
	this->id = id;
	this->xPos = xPos;
	this->yPos = yPos;
	this->xVel = 30;
	this->yVel = 0;
	this->usr = usr;
	this->direccion = dir;
}

int bala::getPosX(){
	return this->xPos;
}

int bala::getPosY(){
	return this->yPos;
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
	this->xPos = x;
}

void bala::setPosY(int y){
	this->yPos = y;
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

bool bala::getDireccion(){
	return this->direccion;
}

void bala::setDireccion(bool dir){
	this->direccion = dir;
}

bala::~bala(){}