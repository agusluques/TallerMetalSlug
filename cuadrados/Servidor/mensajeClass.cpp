#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(){}


mensajeClass::mensajeClass(char * autor, char * destino, int id, int x, int y, int spx, int spy, bool avanzar){
	strcpy(this->usrAutor, autor);
	strcpy(this->usrDestino, destino);
	this->idObjeto = id;
	this->x = x;
	this->y = y;
	this->spX = spx;
	this->spY = spy;
	this->avanzar = avanzar;

	tipoMensaje = 1;
}

mensajeClass::mensajeClass(int tipo, char *destino, string mensajeEnviado){
	this->mensaje = string(mensajeEnviado);
	strcpy(this->usrDestino, destino);
	tipoMensaje = tipo;
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

int mensajeClass::getSpX(){
	return this->spX;
}

int mensajeClass::getSpY(){
	return this->spY;
}

int mensajeClass::getidObjeto(){
	return this->idObjeto;
}

int mensajeClass::getBoolAvanzar(){
	return this->avanzar;
}
