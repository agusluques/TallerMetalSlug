#include <iostream> 
#include "mensajeClass.hpp"
using namespace std;

mensajeClass::mensajeClass(char * autor, char * destino, char * mensajeEnviado){
	this->usrAutor = autor;
	this->usrDestino = destino;
	this->mensaje = mensajeEnviado;
}