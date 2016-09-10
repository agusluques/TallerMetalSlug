#include <iostream> 
#include "mensajeClass.hpp"
#include <cstring>

using namespace std;

mensajeClass::mensajeClass(char * autor, char * destino, char * mensajeEnviado){
	strcpy(this->usrAutor, autor);
	strcpy(this->usrDestino, destino);
	strcpy(this->mensaje, mensajeEnviado);
}
