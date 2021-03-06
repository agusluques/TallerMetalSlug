#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(){}

mensajeClass::mensajeClass(char * autor, int destino, int id, int x, int y, int spx, int spy, char flip, bool avanzar, int tipoDibujable){
	//cout << "1: " << destino << endl;
	//strcpy(this->usrAutor, autor);
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	this->idObjeto = id;
	this->x = x;
	this->y = y;
	this->spX = spx;
	this->spY = spy;
	this->flip = flip;
	this->avanzar = avanzar;

	this->tipoDibujable = tipoDibujable;
	this->tipoMensaje = 1;
}

mensajeClass::mensajeClass(char * autor, int destino, int id, int x, int y, int spx, int spy, char flip, int tipo){
	//cout << "2: " << destino << endl;
	strcpy(this->usrAutor, autor);
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	this->idObjeto = id;
	this->x = x;
	this->y = y;
	this->spX = spx;
	this->spY = spy;
	this->flip = flip;

	tipoMensaje = tipo;
}

mensajeClass::mensajeClass(int tipo, int destino, string mensajeEnviado){
	//cout << "3: " << destino << endl;
	this->mensaje = string(mensajeEnviado);
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	tipoMensaje = tipo;
}

mensajeClass::mensajeClass(int tipo, int destino, int autor, int score){
	//cout << "4: " << destino << endl;
	this->idusrAutor = autor;
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	this->tipoMensaje = tipo;
	this->score = score;
}

mensajeClass::mensajeClass(int tipo, int destino){
	//cout << "5: " << destino << endl;
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	tipoMensaje = tipo;
}

mensajeClass::mensajeClass(int aux, int destino, int id, int x, int y, int spx, int spy, char flip, int tipo){
	//cout << "6: " << destino << endl;
	//strcpy(this->usrDestino, destino);
	this->usrDestino = destino;
	this->idObjeto = id;
	this->x = x;
	this->y = y;
	this->spX = spx;
	this->spY = spy;
	this->flip = flip;
	this->avanzar = aux;

	tipoMensaje = tipo;
}

int mensajeClass::nombreDestinatario(){
	return usrDestino;
}

string mensajeClass::nombreAutor(){
	return this->usrAutor;
}

int mensajeClass::numAutor(){
	return this->idusrAutor;
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

int mensajeClass::getScore(){
	return this->score;
}

int mensajeClass::getEnergia(){
	return this->score;
}

int mensajeClass::getidObjeto(){
	return this->idObjeto;
}

int mensajeClass::getBoolAvanzar(){
	return this->avanzar;
}
