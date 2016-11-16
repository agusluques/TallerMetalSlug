#include "Bonus.h"

Bonus::Bonus(int xPos, int yPos,  int id,string path){
	this->id = id;
	this->xPos = xPos;
	this->yPos = yPos;
	this->sprite = path;
}

int Bonus::getPosX(){
	return this->xPos;
}

int Bonus::getPosY(){
	return this->yPos;
}

int Bonus::getId(){
	return this->id;
}



void Bonus::setPosX(int x){
	this->xPos = x;
}

void Bonus::setPosY(int y){
	this->yPos = y;
}


Bonus::~Bonus(){}