#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(){}


mensajeClass::mensajeClass(char * autor, char * destino, int id, int x, int y){
	strcpy(this->usrAutor, autor);
	strcpy(this->usrDestino, destino);
	this->idObjeto = id;
	this->x = x;
	this->y = y;
}

string mensajeClass::nombreDestinatario(){
	return this->usrDestino;
}

string mensajeClass::nombreAutor(){
	return this->usrAutor;
}

int mensajeClass::getX(){
	return this->x;
}

int mensajeClass::getY(){
	return this->y;
}


int mensajeClass::getidObjeto(){
	return this->idObjeto;
}
