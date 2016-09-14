#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(){}


mensajeClass::mensajeClass(char * autor, char * destino, char * mensajeEnviado){
	strcpy(this->usrAutor, autor);
	strcpy(this->usrDestino, destino);
	this->mensaje = string(mensajeEnviado);
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