#include "Bonus.h"

Bonus::Bonus(int xPos, int yPos,  int tipo, int Id){
	this->xPos = xPos;
	this->yPos = yPos;
	this->tipoBonus = tipo;
	this->Id = Id;
}

int Bonus::getPosX(){
	return this->xPos;
}

int Bonus::getPosY(){
	return this->yPos;
}

int Bonus::getTipoBonus(){
	return this->tipoBonus;
}

int Bonus::getId(){
	return this->Id;
}

void Bonus::setId(int Id){
	this->Id = Id; 
}

bool Bonus::estaVisible(int camaraX){
	return ( getPosX() >= camaraX && getPosX() <= (camaraX + 800) ); //PASARLE ANCHO DE LA VENTANA!!!!
}

void Bonus::setPosX(int x){
	this->xPos = x;
}

void Bonus::setPosY(int y){
	this->yPos = y;
}

void Bonus::setTipoBonus(int tipo){
	this->tipoBonus = tipo;
}

Bonus::~Bonus(){}