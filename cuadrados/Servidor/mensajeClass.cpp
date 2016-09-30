#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(){}


mensajeClass::mensajeClass(char * autor, char * destino, char * mensajeEnviado, int id){
	strcpy(this->usrAutor, autor);
	strcpy(this->usrDestino, destino);
	this->mensaje = string(mensajeEnviado);
	this->idObjeto = id;
}

string mensajeClass::nombreDestinatario(){
	return this->usrDestino;
}

string mensajeClass::nombreAutor(){
	return this->usrAutor;
}

string mensajeClass::getMensaje(){
	return this->mensaje;
}

int mensajeClass::getidObjeto(){
	return this->idObjeto;
}
