/*
 * DibujableServerAdicional.cpp
 *
 *  Created on: 23/11/2016
 *      Author: pablo
 */

#include "DibujableServerAdicional.h"

DibujableServerAdicional::DibujableServerAdicional() {

	id= 0;
	aumentable = 0;
}

void DibujableServerAdicional::setId(int id) {
	this->id = id;
}

void DibujableServerAdicional::setAumentable (int cantidad){

  this->aumentable = cantidad;
}

void DibujableServerAdicional::aumentar(int cantidad){

	this->aumentable += cantidad;
}

int DibujableServerAdicional::getAumentable (){

	return this->aumentable;
}

int DibujableServerAdicional::getId(){
	return this->id;
}


DibujableServerAdicional::~DibujableServerAdicional() {

}

