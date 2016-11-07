/*
 * Plataforma.cpp
 *
 *  Created on: 2/11/2016
 *      Author: pablo
 */

#include "Plataforma.h"

using namespace std;

Plataforma::Plataforma() {
	// TODO Auto-generated constructor stub

}

void Plataforma::setX(int x){

	this->x= x;

}

void Plataforma::setY(int Y){

	this->y= y;

}

void Plataforma::setLong(int longitud){

	this->longitud= longitud;

}

int Plataforma::getX(){

	return x;

}

int Plataforma::getY(){

	return y;

}

int Plataforma::getLong(){

	return longitud;

}

Plataforma::~Plataforma() {
	// TODO Auto-generated destructor stub
}

